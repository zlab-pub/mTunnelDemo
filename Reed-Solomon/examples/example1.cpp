/* Author: Mike Lubinets (aka mersinvald)
 * Date: 29.12.15
 *
 * See LICENSE */

#include <iostream>
//#include <cinttypes>
#include "../include/rs.hpp"
#include <vector>
#include <fstream>
#include <string>
using namespace std;

#define jpg_size 1000

float decode(const string idx) {
    std::ifstream infile("../../../encoded"+ idx, ios::binary);
    std::ofstream outfile("../../../deocded.jpg", ios::trunc);

    const int msglen = 50;
    const int ECC_LENGTH=50;

    RS::ReedSolomon<msglen, ECC_LENGTH> rs;
    char encoded[msglen + ECC_LENGTH];
    char repaired[msglen];
    int counter = 0;
    int err_counter = 0;
    cout << "idx: " << idx << endl;
    while (counter < jpg_size) {
        if (!(infile.read(encoded, sizeof(encoded)))) // read up to the size of the buffer
        {
            if (infile.eof()) {
//                cout << "EOF" << endl;
                break;
            }
        }
        rs.Decode(encoded, repaired);

        int write_t = msglen;
        if (counter+msglen>jpg_size)
            write_t = jpg_size-counter;
        int temp = err_counter;
        for (int i = 0; i <write_t; i++){
            for (unsigned j = 0; j < 8; j++){
            if (((encoded[i] >> j) & 1) !=((repaired[i] >> j) & 1))
                err_counter++;
            }
        }
//        cout << float(err_counter-temp)/write_t/8 << endl;
        std::ofstream outfile("../../../deocded.jpg", ios::binary|ios::app);
        outfile.write(repaired,write_t);
        counter+=msglen;
    }
    cout << float(err_counter)/jpg_size/8 << endl;
    return float(err_counter)/jpg_size/8;
}

//int main() {
//    double total = 215;
//    double temp = 0;
//    for (int i=10; i<total; i++ ) {
//        try {
//            temp += decode((std::to_string(i)));
//        }
//        catch( ...) {
//            temp += 0.3;
//        }
//    }
//    cout << "average: " << double(temp/total) << endl;
//    exit(0);

//    const char *file_name = "../FFT.jpg";
//    const char *ofile_name = "../FFT2.jpg";
//    const int msglen =223;
    const int ECC_LENGTH=8;

//    std::ifstream infile(file_name, std::ios::binary);
//
//    char buffer[msglen];
//
//    if (!(infile.read(buffer, sizeof(buffer)))) // read up to the size of the buffer
//    {
//        if (!infile.eof()) {
//            cout << "buffer size not long enough" << endl;
//        }
//    }
//
//    int buffer_len = (int(infile.gcount()));
//
//
//    cout << buffer_len<< "\t"<< (msglen) << endl;


//    char buffer[] = "Some very important message ought to be delivered";
//    const int msglen = sizeof(buffer);
//
//    char repaired[msglen];
//    char encoded[msglen + ECC_LENGTH];
//
//
//
//    RS::ReedSolomon<msglen, ECC_LENGTH> rs;
//    std::vector <uint8_t> data(&buffer[0], buffer + msglen);
//    rs.Encode(data.data(), encoded);
//
////     Corrupting first 8 bytes of message (any 8 bytes can be repaired)
//    for(unsigned i = 0; i < ECC_LENGTH / 2+1; i++) {
//        encoded[i] = 'E';
//    }
//
//
//
//    rs.Decode(encoded, repaired);
////    std::ofstream outfile(ofile_name, std::ios::binary);
//
////    outfile.write(repaired, buffer_len);
//    std::cout << "Original:  " << buffer  << std::endl;
//    std::cout << "Corrupted: " << encoded  << std::endl;
//    std::cout << "Repaired:  " << repaired << std::endl;
//
//    for (int i =0; i < msglen; i++) {
//        printf("%d\t", buffer[i]);
//        printf("%d\t", encoded[i]);
//        printf("%d\n", repaired[i]);
//    }
//
//    std::cout << ((memcmp(buffer, repaired, msglen) == 0) ? "SUCCESS" : "FAILURE") << std::endl;
//    return 0;
//}

#define ECC_LENGTH 10
#include <chrono>
int main() {

    char message[] = "Some very important message ought to be deliveredSome very important message ought to be delivered22";
    cout << strlen(message) << endl;
    const int msglen = sizeof(message);

    char repaired[msglen];
    char encoded[msglen + ECC_LENGTH];


    RS::ReedSolomon<msglen, ECC_LENGTH> rs;
    auto start_time = std::chrono::high_resolution_clock::now();
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    rs.Encode(message, encoded);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    cout << time/std::chrono::microseconds(1) << "us to run.\n";
    // Corrupting first 8 bytes of message (any 8 bytes can be repaired)
    for(int i = 0; i < ECC_LENGTH / 2; i++) {
        encoded[i] = 'E';
    }

    rs.Decode(encoded, repaired);

    std::cout << "Original:  " << message  << std::endl;
    std::cout << "Corrupted: " << encoded  << std::endl;
    std::cout << "Repaired:  " << repaired << std::endl;

    std::cout << ((memcmp(message, repaired, msglen) == 0) ? "SUCCESS" : "FAILURE") << std::endl;
    return 0;
}
//int main() {
//    const char *file_name = "../FFT.jpg";
//    const char *ofile_name = "../FFT2.jpg";
//
//    std::ifstream infile(file_name, std::ios::binary);
//
//    char buffer[100000];
//
//    if (!(infile.read(buffer, sizeof(buffer)))) // read up to the size of the buffer
//    {
//        if (!infile.eof()) {
//            cout << "buffer size not long enough" << endl;
//        }
//    }
//
//    auto buffer_len = unsigned(infile.gcount());
//
//    cout << (buffer_len) << endl;
//    std::vector <uint8_t> data(&buffer[0], buffer + buffer_len);
//    cout << (data.size()) << endl;
//
//    for (unsigned i_bit = 0; i_bit < 223; ++i_bit)
//        data.at(i_bit) = (uint8_t)(rand() % 2);
//
//
//    LdpcCode ldpc_code(648, buffer_len);
//
//    ldpc_code.load_wifi_ldpc(648, 3);
//
//    auto encoded = ldpc_code.encode(data);
//    encoded[2] = 1 - int(encoded[2]);
//    std::vector<double> modu;
//    cout << endl;
//    for (int i = 0; i < encoded.size(); i++) {
//        modu.push_back(encoded[i]);
//        cout << int(encoded[i]);
//    }
//
//    cout << endl;
//    cout << endl;
//
//    cout << endl;
//    auto decoded = ldpc_code.decode(modu, 20, false);
//    cout << encoded.size() << endl;
//    cout << decoded.size() << endl;
//    for (int i = 0; i < encoded.size(); i++) {
//        decoded[i] = 1 - int(decoded[i]);
//        cout << int(decoded[i]);
//    }
//    cout << endl;
////    cout << endl;
////    cout << endl;
//    std::ofstream outfile(ofile_name, std::ios::binary);
//
//    outfile.write(reinterpret_cast<char *>(decoded.data()), data.size());
//}
