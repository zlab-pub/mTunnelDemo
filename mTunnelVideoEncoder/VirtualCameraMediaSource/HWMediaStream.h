//
// Copyright (C) Microsoft Corporation. All rights reserved.
//

#ifndef  HWMEDIASTREAM_H
#define  HWMEDIASTREAM_H

#pragma once
#include "HWMediaSource.h"
/*
struct MYMediaTypeHandler : IMFMediaTypeHandler {
public:
    MYMediaTypeHandler(IMFMediaTypeHandler* devTypeHandler) :m_devTypeHandler(devTypeHandler) {};

    virtual HRESULT GetCurrentMediaType(IMFMediaType** ppMediaType) { return m_devTypeHandler->GetCurrentMediaType(ppMediaType); }
    HRESULT GetMajorType(GUID* pguidMajorType) { return m_devTypeHandler->GetMajorType(pguidMajorType); };
    HRESULT GetMediaTypeCount(DWORD* pdwTypeCount) { *pdwTypeCount = 1; return S_OK; };
    HRESULT GetMediaTypeByIndex(DWORD dwIndex, IMFMediaType** ppType) { return m_devTypeHandler->GetMediaTypeByIndex(m_supportedMediaType, ppType); };
    HRESULT IsMediaTypeSupported(IMFMediaType* pMediaType, IMFMediaType** ppMediaType) { return m_devTypeHandler->GetMediaTypeByIndex(m_supportedMediaType, ppMediaType); }; //Need to be fixed. Only one supported type available //m_devTypeHandler->SetCurrentMediaType(pMediaType);
    HRESULT SetCurrentMediaType(IMFMediaType* pMediaType) { m_devTypeHandler->SetCurrentMediaType(pMediaType); };

private:
    int m_supportedMediaType = 2;
    IMFMediaTypeHandler* m_devTypeHandler;
};

struct MYStreamDescriptor : IMFStreamDescriptor {
public:
    //MYStreamDescriptor(IMFStreamDescriptor* devStreamDesc) :m_devStreamDesc(devStreamDesc) { devStreamDesc->GetMediaTypeHandler( dynamic_cast<IMFMediaTypeHandler**>(&m_TypeHandler)); };
    HRESULT GetMediaTypeHandler(IMFMediaTypeHandler** ppMediaTypeHandler) { *ppMediaTypeHandler = m_TypeHandler; return S_OK; };
    HRESULT GetStreamIdentifier(DWORD* pdwStreamIdentifier) { return m_devStreamDesc->GetStreamIdentifier(pdwStreamIdentifier); };

private:
    IMFStreamDescriptor* m_devStreamDesc;
    MYMediaTypeHandler* m_TypeHandler;
};
*/

namespace winrt::WindowsSample::implementation
{
  
    struct HWMediaStream : winrt::implements<HWMediaStream, IMFMediaStream2>
    {
        friend struct HWMediaSource;

    public:
        ~HWMediaStream();

        // IMFMediaEventGenerator
        IFACEMETHODIMP BeginGetEvent(IMFAsyncCallback* pCallback, IUnknown* punkState) override;
        IFACEMETHODIMP EndGetEvent(IMFAsyncResult* pResult, IMFMediaEvent** ppEvent) override;
        IFACEMETHODIMP GetEvent(DWORD dwFlags, IMFMediaEvent** ppEvent) override;
        IFACEMETHODIMP QueueEvent(MediaEventType met, REFGUID guidExtendedType, HRESULT hrStatus, const PROPVARIANT* pvValue) override;

        // IMFMediaStream
        IFACEMETHODIMP GetMediaSource(IMFMediaSource** ppMediaSource) override;
        IFACEMETHODIMP GetStreamDescriptor(IMFStreamDescriptor** ppStreamDescriptor) override;
        IFACEMETHODIMP RequestSample(IUnknown* pToken) override;

        // IMFMediaStream2
        IFACEMETHODIMP SetStreamState(MF_STREAM_STATE state) override;
        IFACEMETHODIMP GetStreamState(_Out_ MF_STREAM_STATE* pState) override;

        // Non-interface methods.
        HRESULT Initialize(_In_ HWMediaSource* pSource, _In_ IMFStreamDescriptor* pStreamDesc, _In_ DWORD dwWorkQueue);
        HRESULT ProcessSample(_In_ IMFSample* inputSample);
        HRESULT Shutdown();
        DWORD StreamIdentifier() { return m_dwStreamIdentifier; };
        HRESULT SetMediaStream(_In_ IMFMediaStream* pMediaStream);

        HRESULT OnMediaStreamEvent(_In_ IMFAsyncResult* pResult);

    protected:
        HRESULT _CheckShutdownRequiresLock();
        _Requires_lock_held_(m_Lock) HRESULT _SetStreamAttributes(_In_ IMFAttributes* pAttributeStore);

        wil::com_ptr_nothrow<CAsyncCallback<HWMediaStream>> m_xOnMediaStreamEvent;

        winrt::slim_mutex  m_Lock;

        wil::com_ptr_nothrow<IMFMediaSource> m_parent;
        wil::com_ptr_nothrow<IMFMediaStream> m_spDevStream;
        wil::com_ptr_nothrow<IMFStreamDescriptor> m_spStreamDesc;
        wil::com_ptr_nothrow<IMFMediaEventQueue> m_spEventQueue;

        bool m_isShutdown = false;
        DWORD m_dwStreamIdentifier = 0;
        DWORD m_dwSerialWorkQueueId = 0;

        std::vector<wil::com_ptr_nothrow<IMFMediaType>> m_mediaTypeCache;
        wil::com_ptr_nothrow<IMFAttributes> m_spAttributes;
    };
}

#endif
