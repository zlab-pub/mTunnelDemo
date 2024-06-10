#include "session_record.h"
#include "session_state.h"
#include "session_builder.h"
#include "session_pre_key.h"
#include "session_cipher.h"
#include "key_helper.h"
#include "curve.h"
#include "ratchet.h"
#include "protocol.h"
#include "uthash.h"

#include "signal_protocol.h"

#include "test_common.h"


std::recursive_mutex global_mutex;
void test_lock(void* user_data)
{
    global_mutex.lock();
}
void test_unlock(void* user_data)
{
    global_mutex.unlock();
}


int64_t jenkins_hash(const char* key, size_t len)
{
    uint64_t hash, i;
    for (hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}


void setup_test_store_context(signal_protocol_store_context** context, signal_context* global_context)
{
    int result = 0;

    signal_protocol_store_context* store_context = 0;
    result = signal_protocol_store_context_create(&store_context, global_context);

    setup_test_session_store(store_context);
    setup_test_pre_key_store(store_context);
    setup_test_signed_pre_key_store(store_context);
    setup_test_identity_key_store(store_context, global_context);
    setup_test_sender_key_store(store_context, global_context);

    *context = store_context;
}

typedef struct {
    int64_t recipient_id;
    int32_t device_id;
} test_session_store_session_key;

typedef struct {
    test_session_store_session_key key;
    signal_buffer* record;
    UT_hash_handle hh;
} test_session_store_session;

typedef struct {
    test_session_store_session* sessions;
} test_session_store_data;

int test_session_store_load_session(signal_buffer** record, signal_buffer** user_record, const signal_protocol_address* address, void* user_data)
{
    test_session_store_data* data = (test_session_store_data*)user_data;

    test_session_store_session* s;

    test_session_store_session l;
    memset(&l, 0, sizeof(test_session_store_session));
    l.key.recipient_id = jenkins_hash(address->name, address->name_len);
    l.key.device_id = address->device_id;
    HASH_FIND(hh, data->sessions, &l.key, sizeof(test_session_store_session_key), s);

    if (!s) {
        return 0;
    }
    signal_buffer* result = signal_buffer_copy(s->record);
    if (!result) {
        return SG_ERR_NOMEM;
    }
    *record = result;
    return 1;
}
int test_session_store_get_sub_device_sessions(signal_int_list** sessions, const char* name, size_t name_len, void* user_data)
{
    test_session_store_data* data = (test_session_store_data*)user_data;

    signal_int_list* result = signal_int_list_alloc();
    if (!result) {
        return SG_ERR_NOMEM;
    }

    int64_t recipient_hash = jenkins_hash(name, name_len);
    test_session_store_session* cur_node;
    test_session_store_session* tmp_node;
    HASH_ITER(hh, data->sessions, cur_node, tmp_node) {
        if (cur_node->key.recipient_id == recipient_hash) {
            signal_int_list_push_back(result, cur_node->key.device_id);
        }
    }

    *sessions = result;
    return 0;
}
int test_session_store_store_session(const signal_protocol_address* address, uint8_t* record, size_t record_len, uint8_t* user_record_data, size_t user_record_len, void* user_data)
{
    test_session_store_data* data = (test_session_store_data*)user_data;

    test_session_store_session* s;

    test_session_store_session l;
    memset(&l, 0, sizeof(test_session_store_session));
    l.key.recipient_id = jenkins_hash(address->name, address->name_len);
    l.key.device_id = address->device_id;

    signal_buffer* record_buf = signal_buffer_create(record, record_len);
    if (!record_buf) {
        return SG_ERR_NOMEM;
    }

    HASH_FIND(hh, data->sessions, &l.key, sizeof(test_session_store_session_key), s);

    if (s) {
        signal_buffer_free(s->record);
        s->record = record_buf;
    }
    else {
        s = (test_session_store_session*)malloc(sizeof(test_session_store_session));
        if (!s) {
            signal_buffer_free(record_buf);
            return SG_ERR_NOMEM;
        }
        memset(s, 0, sizeof(test_session_store_session));
        s->key.recipient_id = jenkins_hash(address->name, address->name_len);
        s->key.device_id = address->device_id;
        s->record = record_buf;
        HASH_ADD(hh, data->sessions, key, sizeof(test_session_store_session_key), s);
    }

    return 0;
}
int test_session_store_contains_session(const signal_protocol_address* address, void* user_data)
{
    test_session_store_data* data = (test_session_store_data*)user_data;
    test_session_store_session* s;

    test_session_store_session l;
    memset(&l, 0, sizeof(test_session_store_session));
    l.key.recipient_id = jenkins_hash(address->name, address->name_len);
    l.key.device_id = address->device_id;

    HASH_FIND(hh, data->sessions, &l.key, sizeof(test_session_store_session_key), s);

    return (s == 0) ? 0 : 1;
}
int test_session_store_delete_session(const signal_protocol_address* address, void* user_data)
{
    int result = 0;
    test_session_store_data* data = (test_session_store_data*)user_data;
    test_session_store_session* s;

    test_session_store_session l;
    memset(&l, 0, sizeof(test_session_store_session));
    l.key.recipient_id = jenkins_hash(address->name, address->name_len);
    l.key.device_id = address->device_id;

    HASH_FIND(hh, data->sessions, &l.key, sizeof(test_session_store_session_key), s);

    if (s) {
        HASH_DEL(data->sessions, s);
        signal_buffer_free(s->record);
        free(s);
        result = 1;
    }
    return result;
}
int test_session_store_delete_all_sessions(const char* name, size_t name_len, void* user_data)
{
    int result = 0;
    test_session_store_data* data = (test_session_store_data*)user_data;

    int64_t recipient_hash = jenkins_hash(name, name_len);
    test_session_store_session* cur_node;
    test_session_store_session* tmp_node;
    HASH_ITER(hh, data->sessions, cur_node, tmp_node) {
        if (cur_node->key.recipient_id == recipient_hash) {
            HASH_DEL(data->sessions, cur_node);
            signal_buffer_free(cur_node->record);
            free(cur_node);
            result++;
        }
    }

    return result;
}
void test_session_store_destroy(void* user_data)
{
    test_session_store_data* data = (test_session_store_data*)user_data;

    test_session_store_session* cur_node;
    test_session_store_session* tmp_node;
    HASH_ITER(hh, data->sessions, cur_node, tmp_node) {
        HASH_DEL(data->sessions, cur_node);
        signal_buffer_free(cur_node->record);
        free(cur_node);
    }

    free(data);
}

void setup_test_session_store(signal_protocol_store_context* context)
{
    test_session_store_data* data = (test_session_store_data*)malloc(sizeof(test_session_store_data));
    memset(data, 0, sizeof(test_session_store_data));

    signal_protocol_session_store store = {
        store.load_session_func = test_session_store_load_session,
        store.get_sub_device_sessions_func = test_session_store_get_sub_device_sessions,
        store.store_session_func = test_session_store_store_session,
        store.contains_session_func = test_session_store_contains_session,
        store.delete_session_func = test_session_store_delete_session,
        store.delete_all_sessions_func = test_session_store_delete_all_sessions,
        store.destroy_func = test_session_store_destroy,
        store.user_data = data
    };

    signal_protocol_store_context_set_session_store(context, &store);
}


typedef struct {
    uint32_t key_id;
    signal_buffer* key_record;
    UT_hash_handle hh;
} test_pre_key_store_key;

typedef struct {
    test_pre_key_store_key* keys;
} test_pre_key_store_data;

int test_pre_key_store_load_pre_key(signal_buffer** record, uint32_t pre_key_id, void* user_data)
{
    test_pre_key_store_data* data = (test_pre_key_store_data*)user_data;

    test_pre_key_store_key* s;

    HASH_FIND(hh, data->keys, &pre_key_id, sizeof(uint32_t), s);
    if (s) {
        *record = signal_buffer_copy(s->key_record);
        return SG_SUCCESS;
    }
    else {
        return SG_ERR_INVALID_KEY_ID;
    }
}
int test_pre_key_store_store_pre_key(uint32_t pre_key_id, uint8_t* record, size_t record_len, void* user_data)
{
    test_pre_key_store_data* data = (test_pre_key_store_data*)user_data;

    test_pre_key_store_key* s;

    signal_buffer* key_buf = signal_buffer_create(record, record_len);
    if (!key_buf) {
        return SG_ERR_NOMEM;
    }

    HASH_FIND(hh, data->keys, &pre_key_id, sizeof(uint32_t), s);
    if (s) {
        signal_buffer_free(s->key_record);
        s->key_record = key_buf;
    }
    else {
        s = (test_pre_key_store_key*)malloc(sizeof(test_pre_key_store_key));
        if (!s) {
            signal_buffer_free(key_buf);
            return SG_ERR_NOMEM;
        }
        memset(s, 0, sizeof(test_pre_key_store_key));
        s->key_id = pre_key_id;
        s->key_record = key_buf;
        HASH_ADD(hh, data->keys, key_id, sizeof(uint32_t), s);
    }

    return 0;
}
int test_pre_key_store_contains_pre_key(uint32_t pre_key_id, void* user_data)
{
    test_pre_key_store_data* data = (test_pre_key_store_data*)user_data;

    test_pre_key_store_key* s;
    HASH_FIND(hh, data->keys, &pre_key_id, sizeof(uint32_t), s);

    return (s == 0) ? 0 : 1;
}
int test_pre_key_store_remove_pre_key(uint32_t pre_key_id, void* user_data)
{
    test_pre_key_store_data* data = (test_pre_key_store_data*)user_data;

    test_pre_key_store_key* s;
    HASH_FIND(hh, data->keys, &pre_key_id, sizeof(uint32_t), s);
    if (s) {
        HASH_DEL(data->keys, s);
        signal_buffer_free(s->key_record);
        free(s);
    }

    return 0;
}
void test_pre_key_store_destroy(void* user_data)
{
    test_pre_key_store_data* data = (test_pre_key_store_data*)user_data;

    test_pre_key_store_key* cur_node;
    test_pre_key_store_key* tmp_node;
    HASH_ITER(hh, data->keys, cur_node, tmp_node) {
        HASH_DEL(data->keys, cur_node);
        signal_buffer_free(cur_node->key_record);
        free(cur_node);
    }
    free(data);
}

void setup_test_pre_key_store(signal_protocol_store_context* context)
{
    test_pre_key_store_data* data = (test_pre_key_store_data*)malloc(sizeof(test_pre_key_store_data));
    memset(data, 0, sizeof(test_pre_key_store_data));

    signal_protocol_pre_key_store store = {
        store.load_pre_key = test_pre_key_store_load_pre_key,
        store.store_pre_key = test_pre_key_store_store_pre_key,
        store.contains_pre_key = test_pre_key_store_contains_pre_key,
        store.remove_pre_key = test_pre_key_store_remove_pre_key,
        store.destroy_func = test_pre_key_store_destroy,
        store.user_data = data
    };

    signal_protocol_store_context_set_pre_key_store(context, &store);
}


typedef struct {
    uint32_t key_id;
    signal_buffer* key_record;
    UT_hash_handle hh;
} test_signed_pre_key_store_key;

typedef struct {
    test_signed_pre_key_store_key* keys;
} test_signed_pre_key_store_data;


int test_signed_pre_key_store_load_signed_pre_key(signal_buffer** record, uint32_t signed_pre_key_id, void* user_data)
{
    test_signed_pre_key_store_data* data = (test_signed_pre_key_store_data*)user_data;
    test_signed_pre_key_store_key* s;

    HASH_FIND(hh, data->keys, &signed_pre_key_id, sizeof(uint32_t), s);
    if (s) {
        *record = signal_buffer_copy(s->key_record);
        return SG_SUCCESS;
    }
    else {
        return SG_ERR_INVALID_KEY_ID;
    }
}
int test_signed_pre_key_store_store_signed_pre_key(uint32_t signed_pre_key_id, uint8_t* record, size_t record_len, void* user_data)
{
    test_signed_pre_key_store_data* data = (test_signed_pre_key_store_data*)user_data;
    test_signed_pre_key_store_key* s;

    signal_buffer* key_buf = signal_buffer_create(record, record_len);
    if (!key_buf) {
        return SG_ERR_NOMEM;
    }

    HASH_FIND(hh, data->keys, &signed_pre_key_id, sizeof(uint32_t), s);
    if (s) {
        signal_buffer_free(s->key_record);
        s->key_record = key_buf;
    }
    else {
        s = (test_signed_pre_key_store_key*)malloc(sizeof(test_signed_pre_key_store_key));
        if (!s) {
            signal_buffer_free(key_buf);
            return SG_ERR_NOMEM;
        }
        memset(s, 0, sizeof(test_signed_pre_key_store_key));
        s->key_id = signed_pre_key_id;
        s->key_record = key_buf;
        HASH_ADD(hh, data->keys, key_id, sizeof(uint32_t), s);
    }

    return 0;
}
int test_signed_pre_key_store_contains_signed_pre_key(uint32_t signed_pre_key_id, void* user_data)
{
    test_signed_pre_key_store_data* data = (test_signed_pre_key_store_data*)user_data;

    test_signed_pre_key_store_key* s;
    HASH_FIND(hh, data->keys, &signed_pre_key_id, sizeof(uint32_t), s);

    return (s == 0) ? 0 : 1;
}
int test_signed_pre_key_store_remove_signed_pre_key(uint32_t signed_pre_key_id, void* user_data)
{
    test_signed_pre_key_store_data* data = (test_signed_pre_key_store_data*)user_data;

    test_signed_pre_key_store_key* s;
    HASH_FIND(hh, data->keys, &signed_pre_key_id, sizeof(uint32_t), s);
    if (s) {
        HASH_DEL(data->keys, s);
        signal_buffer_free(s->key_record);
        free(s);
    }

    return 0;
}
void test_signed_pre_key_store_destroy(void* user_data)
{
    test_signed_pre_key_store_data* data = (test_signed_pre_key_store_data*)user_data;

    test_signed_pre_key_store_key* cur_node;
    test_signed_pre_key_store_key* tmp_node;
    HASH_ITER(hh, data->keys, cur_node, tmp_node) {
        HASH_DEL(data->keys, cur_node);
        signal_buffer_free(cur_node->key_record);
        free(cur_node);
    }
    free(data);
}

void setup_test_signed_pre_key_store(signal_protocol_store_context* context)
{
    test_signed_pre_key_store_data* data = (test_signed_pre_key_store_data*)malloc(sizeof(test_signed_pre_key_store_data));
    memset(data, 0, sizeof(test_signed_pre_key_store_data));

    signal_protocol_signed_pre_key_store store = {
            store.load_signed_pre_key = test_signed_pre_key_store_load_signed_pre_key,
            store.store_signed_pre_key = test_signed_pre_key_store_store_signed_pre_key,
            store.contains_signed_pre_key = test_signed_pre_key_store_contains_signed_pre_key,
            store.remove_signed_pre_key = test_signed_pre_key_store_remove_signed_pre_key,
            store.destroy_func = test_signed_pre_key_store_destroy,
            store.user_data = data
    };

    signal_protocol_store_context_set_signed_pre_key_store(context, &store);
}


typedef struct {
    int64_t recipient_id;
    signal_buffer* identity_key;
    UT_hash_handle hh;
} test_identity_store_key;

typedef struct {
    test_identity_store_key* keys;
    signal_buffer* identity_key_public;
    signal_buffer* identity_key_private;
    uint32_t local_registration_id;
} test_identity_store_data;

int test_identity_key_store_get_identity_key_pair(signal_buffer** public_data, signal_buffer** private_data, void* user_data)
{
    test_identity_store_data* data = (test_identity_store_data*)user_data;
    *public_data = signal_buffer_copy(data->identity_key_public);
    *private_data = signal_buffer_copy(data->identity_key_private);
    return 0;
}
int test_identity_key_store_get_local_registration_id(void* user_data, uint32_t* registration_id)
{
    test_identity_store_data* data = (test_identity_store_data*)user_data;
    *registration_id = data->local_registration_id;
    return 0;
}
int test_identity_key_store_save_identity(const signal_protocol_address* address, uint8_t* key_data, size_t key_len, void* user_data)
{
    test_identity_store_data* data = (test_identity_store_data*)user_data;

    test_identity_store_key* s;

    signal_buffer* key_buf = signal_buffer_create(key_data, key_len);
    if (!key_buf) {
        return SG_ERR_NOMEM;
    }

    int64_t recipient_hash = jenkins_hash(address->name, address->name_len);

    HASH_FIND(hh, data->keys, &recipient_hash, sizeof(int64_t), s);
    if (s) {
        signal_buffer_free(s->identity_key);
        s->identity_key = key_buf;
    }
    else {
        s = (test_identity_store_key*)malloc(sizeof(test_identity_store_key));
        if (!s) {
            signal_buffer_free(key_buf);
            return SG_ERR_NOMEM;
        }
        memset(s, 0, sizeof(test_identity_store_key));
        s->recipient_id = recipient_hash;
        s->identity_key = key_buf;
        HASH_ADD(hh, data->keys, recipient_id, sizeof(int64_t), s);
    }

    return 0;
}
int test_identity_key_store_is_trusted_identity(const signal_protocol_address* address, uint8_t* key_data, size_t key_len, void* user_data)
{
    test_identity_store_data* data = (test_identity_store_data*)user_data;

    int64_t recipient_hash = jenkins_hash(address->name, address->name_len);

    test_identity_store_key* s;
    HASH_FIND(hh, data->keys, &recipient_hash, sizeof(int64_t), s);

    if (s) {
        uint8_t* store_data = signal_buffer_data(s->identity_key);
        size_t store_len = signal_buffer_len(s->identity_key);
        if (store_len != key_len) {
            return 0;
        }
        if (memcmp(key_data, store_data, key_len) == 0) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 1;
    }
}
void test_identity_key_store_destroy(void* user_data)
{
    test_identity_store_data* data = (test_identity_store_data*)user_data;

    test_identity_store_key* cur_node;
    test_identity_store_key* tmp_node;
    HASH_ITER(hh, data->keys, cur_node, tmp_node) {
        HASH_DEL(data->keys, cur_node);
        signal_buffer_free(cur_node->identity_key);
        free(cur_node);
    }
    signal_buffer_free(data->identity_key_public);
    signal_buffer_free(data->identity_key_private);
    free(data);
}

void setup_test_identity_key_store(signal_protocol_store_context* context, signal_context* global_context)
{
    test_identity_store_data* data = (test_identity_store_data*)malloc(sizeof(test_identity_store_data));
    memset(data, 0, sizeof(test_identity_store_data));

    ec_key_pair* identity_key_pair_keys = 0;
    curve_generate_key_pair(global_context, &identity_key_pair_keys);

    ec_public_key* identity_key_public = ec_key_pair_get_public(identity_key_pair_keys);
    ec_private_key* identity_key_private = ec_key_pair_get_private(identity_key_pair_keys);

    ec_public_key_serialize(&data->identity_key_public, identity_key_public);
    ec_private_key_serialize(&data->identity_key_private, identity_key_private);
    SIGNAL_UNREF(identity_key_pair_keys);

    data->local_registration_id = (rand() % 16380) + 1;

    signal_protocol_identity_key_store store = {
            store.get_identity_key_pair = test_identity_key_store_get_identity_key_pair,
            store.get_local_registration_id = test_identity_key_store_get_local_registration_id,
            store.save_identity = test_identity_key_store_save_identity,
            store.is_trusted_identity = test_identity_key_store_is_trusted_identity,
            store.destroy_func = test_identity_key_store_destroy,
            store.user_data = data
    };

    signal_protocol_store_context_set_identity_key_store(context, &store);
}



typedef struct {
    int64_t group_id;
    int64_t recipient_id;
    int32_t device_id;
} test_sender_key_store_key;

typedef struct {
    test_sender_key_store_key key;
    signal_buffer* record;
    UT_hash_handle hh;
} test_sender_key_store_record;

typedef struct {
    test_sender_key_store_record* records;
} test_sender_key_store_data;

int test_sender_key_store_store_sender_key(const signal_protocol_sender_key_name* sender_key_name, uint8_t* record, size_t record_len, uint8_t* user_record_data, size_t user_record_len, void* user_data)
{
    test_sender_key_store_data* data = (test_sender_key_store_data*)user_data;

    test_sender_key_store_record* s;

    test_sender_key_store_record l;
    memset(&l, 0, sizeof(test_sender_key_store_record));
    l.key.group_id = jenkins_hash(sender_key_name->group_id, sender_key_name->group_id_len);
    l.key.recipient_id = jenkins_hash(sender_key_name->sender.name, sender_key_name->sender.name_len);
    l.key.device_id = sender_key_name->sender.device_id;

    signal_buffer* record_buf = signal_buffer_create(record, record_len);
    if (!record_buf) {
        return SG_ERR_NOMEM;
    }

    HASH_FIND(hh, data->records, &l.key, sizeof(test_sender_key_store_key), s);

    if (s) {
        signal_buffer_free(s->record);
        s->record = record_buf;
    }
    else {
        s = (test_sender_key_store_record*)malloc(sizeof(test_sender_key_store_record));
        if (!s) {
            signal_buffer_free(record_buf);
            return SG_ERR_NOMEM;
        }
        memset(s, 0, sizeof(test_sender_key_store_record));
        s->key.group_id = jenkins_hash(sender_key_name->group_id, sender_key_name->group_id_len);
        s->key.recipient_id = jenkins_hash(sender_key_name->sender.name, sender_key_name->sender.name_len);
        s->key.device_id = sender_key_name->sender.device_id;
        s->record = record_buf;
        HASH_ADD(hh, data->records, key, sizeof(test_sender_key_store_key), s);
    }

    return 0;
}

int test_sender_key_store_load_sender_key(signal_buffer** record, signal_buffer** user_record, const signal_protocol_sender_key_name* sender_key_name, void* user_data)
{
    test_sender_key_store_data* data = (test_sender_key_store_data*)user_data;

    test_sender_key_store_record* s;

    test_sender_key_store_record l;
    memset(&l, 0, sizeof(test_sender_key_store_record));
    l.key.group_id = jenkins_hash(sender_key_name->group_id, sender_key_name->group_id_len);
    l.key.recipient_id = jenkins_hash(sender_key_name->sender.name, sender_key_name->sender.name_len);
    l.key.device_id = sender_key_name->sender.device_id;
    HASH_FIND(hh, data->records, &l.key, sizeof(test_sender_key_store_key), s);

    if (!s) {
        return 0;
    }
    signal_buffer* result = signal_buffer_copy(s->record);
    if (!result) {
        return SG_ERR_NOMEM;
    }
    *record = result;
    return 1;
}

void test_sender_key_store_destroy(void* user_data)
{
    test_sender_key_store_data* data = (test_sender_key_store_data*)user_data;

    test_sender_key_store_record* cur_node;
    test_sender_key_store_record* tmp_node;
    HASH_ITER(hh, data->records, cur_node, tmp_node) {
        HASH_DEL(data->records, cur_node);
        signal_buffer_free(cur_node->record);
        free(cur_node);
    }
    free(data);
}

void setup_test_sender_key_store(signal_protocol_store_context* context, signal_context* global_context)
{
    test_sender_key_store_data* data = (test_sender_key_store_data*)malloc(sizeof(test_sender_key_store_data));
    memset(data, 0, sizeof(test_sender_key_store_data));

    signal_protocol_sender_key_store store = {
        store.store_sender_key = test_sender_key_store_store_sender_key,
        store.load_sender_key = test_sender_key_store_load_sender_key,
        store.destroy_func = test_sender_key_store_destroy,
        store.user_data = data
    };

    signal_protocol_store_context_set_sender_key_store(context, &store);
}



typedef struct {
    const char* original_message;
    size_t original_message_len;
    signal_protocol_store_context* bob_store;
} test_basic_pre_key_v3_callback_data;


int test_basic_pre_key_v3_decrypt_callback(session_cipher* cipher, signal_buffer* plaintext, void* decrypt_context)
{
    test_basic_pre_key_v3_callback_data* callback_data = (test_basic_pre_key_v3_callback_data*)session_cipher_get_user_data(cipher);
    int callback_context = *(int*)decrypt_context;

    /* Make sure we got the same decrypt context value we passed in */
    std::cout << callback_context << 1234 << std::endl;
    ;

    /* Verify that the plaintext matches what we expected */
    uint8_t* plaintext_data = signal_buffer_data(plaintext);
    size_t plaintext_len = signal_buffer_len(plaintext);

    std::cout << callback_data->original_message_len << plaintext_len << std::endl;
    std::cout << memcmp(callback_data->original_message, plaintext_data, plaintext_len) << 0 << std::endl;

    /* Verify that Bob's session state has not yet been updated */
    //std::cout << signal_protocol_session_contains_session(callback_data->bob_store, &address)<< 0 << std::endl;

    return 0;
}