//
// Copyright (C) Microsoft Corporation. All rights reserved.
//

#include "pch.h"
#include "HWMediaStream.h"


#include <Wincodecsdk.h>

#include "errno.h"
#include <stdlib.h>
#include <Wincodec.h>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <random>
#include <algorithm>

#include "rs.hpp"
 

namespace cv {
    static void crop_and_resize(const Mat & image, Mat & result, int width, int height) {
        int crop_height = 576;
        int crop_width = 1024;

        Mat cropped = image(Rect((width - crop_width) / 2, (height - crop_height) / 2, crop_width, crop_height));

        Mat resized(216, 384, image.type());

        resize(cropped, resized, resized.size(), 0, 0, INTER_LINEAR);
        result = resized;
    }


    static int JPEG_encode(Mat& resized) {
        //JPEG encode
        std::vector<uchar> buff;
        const std::vector<int> para = { IMWRITE_JPEG_QUALITY, 8 };

        imencode(".jpg", resized, buff, para);

        unsigned jpgSize = buff.size();

 
        return jpgSize;
    }


    using namespace std;
    Mat qrCodeGenerator(const Mat& image, int width, int height) {
        //decrease frame rate -> 10 fps
        static Mat last(height, width, image.type(), Scalar(255, 255, 255));
        static int counter = 6;
        counter++;
        if (counter >= 6) {
            counter = 0;
        }
        else {
            return last;
        }


        // Allocated for the tunnel
        vector <uint8_t> dBitsArray;


        // Crop and resize the original figure to derive more redundant space
        int crop_height = 576;
        int crop_width = 1024;
        Mat cropped = image(Rect((width - crop_width) / 2, (height - crop_height) / 2, crop_width, crop_height));
        Mat resized(324, 576, image.type()); 
        resize(cropped, resized, resized.size(), 0, 0, INTER_LINEAR);



        // Compress the figure
        int quality = 50;
        unsigned jpgSize = 0;
        std::vector<uchar> buff;
        while (quality > 0) {
            const std::vector<int> para = { IMWRITE_JPEG_QUALITY, quality };
            imencode(".jpg", resized, buff, para);
            jpgSize = buff.size();
            if (jpgSize < (width - 32)* (height - 64)*3/8/8/4-2)
                break;
            quality--;
            buff.clear();
        }

        // ReCompress it if overflow


        // Should encrypt the data here

        // Allocate 16 bits for jpg size 
        for (int i = 15; i >= 0; i -= 1) {
            uint8_t temp = (jpgSize >> i) & 0b1;
            dBitsArray.push_back(temp);
        }


        // ECC encoding
        const int msglen = 100;     // plain-text length
        const int ecclen = 4;  // encryption length
        RS::ReedSolomon<msglen, ecclen> rs;
        char encoded[msglen + ecclen];

        unsigned buf_len = jpgSize;
        auto mod = buf_len % msglen;
        unsigned padding = 0;
        if (mod > 0)
            padding = msglen - mod;
        while (padding > 0) {
            padding--;
            buff.push_back(rand()%256);
        }


        // Encode bytes into bit array
        uchar* buf = buff.data();
        int buf_ptr = 0;

        while (buf_len > buf_ptr) {
            rs.Encode(buf + buf_ptr, encoded);
            buf_ptr += msglen;
            for (int i = 0; i <= msglen + ecclen - 1; i++) {
                auto byte = encoded[i];
                for (int j = 7; j >= 0; j -= 1) {
                    auto temp = (byte >> j) & 0b1;
                    dBitsArray.push_back(temp);
                }
            }
        }

        // Padding
        while (dBitsArray.size() < 19608*8) {
            dBitsArray.push_back(rand() % 2);
        }

        // modulate bits in dBitsArray -> qrCode
        Mat qrCode(height, width, image.type(), Scalar(255, 255, 255));

        Mat roi = qrCode(Rect(16, 32, width - 32, height - 64));

        const int block_size = 4;   // set blocksize as 4*4 pixels
        const int color_size = 2;   // use 1 channel to indicate 1 bit
        const unsigned rgb[color_size] = { 0,  255 };   // { 0, 85, 170, 255 };

        Mat QRdot = Mat((height - 64) / block_size, (width - 32)/ block_size, image.type(), Scalar(255, 255, 255));

        int w = 0;
        int h = 0;

        int crop_h = roi.size[0];
        int crop_w = roi.size[1];


        for (int i = 0; i <= dBitsArray.size() - 3; i += 3) {
            Vec3b& color = QRdot.at<Vec3b>(Point(w, h));
            color[0] = rgb[dBitsArray[i]];
            color[1] = rgb[dBitsArray[i + 1]];
            color[2] = rgb[dBitsArray[i + 2]];
            w += 1;
            if (w >= crop_w/block_size) {
                w = 0;
                h += 1;
            }
            if (h  >= crop_h/block_size) {  // ignore overflow bits
                DEBUG_MSG(L"Exceed %d bytes limitation\n", i)
                    break;
            }
        }

        Mat core = Mat(height - 64, width - 32, image.type(), Scalar(255, 255, 255));
        resize(QRdot, core, core.size(),  0, 0, INTER_NEAREST);
        core.copyTo(roi);
       

        qrCode.copyTo(last);
        return qrCode;
    }
}



namespace winrt::WindowsSample::implementation
{
    HWMediaStream::~HWMediaStream()
    {
        Shutdown();
    }

    HRESULT CloneMediaType(_In_ IMFMediaType* pSourceMediaType, _Out_ IMFMediaType** ppDestMediaType)
    {
        wil::com_ptr_nothrow<IMFVideoMediaType> spVideoMT;

        RETURN_HR_IF_NULL(E_INVALIDARG, pSourceMediaType);
        RETURN_HR_IF_NULL(E_POINTER, ppDestMediaType);

        *ppDestMediaType = nullptr;

        if (SUCCEEDED(pSourceMediaType->QueryInterface(IID_PPV_ARGS(&spVideoMT))))
        {
            wil::com_ptr_nothrow<IMFVideoMediaType> spClonedVideoMT;

            RETURN_IF_FAILED(MFCreateVideoMediaType(spVideoMT->GetVideoFormat(), &spClonedVideoMT));
            RETURN_IF_FAILED(spVideoMT->CopyAllItems(spClonedVideoMT.get()));
            RETURN_IF_FAILED(spClonedVideoMT->QueryInterface(IID_PPV_ARGS(ppDestMediaType)));
        }
        else
        {
            wil::com_ptr_nothrow<IMFMediaType> spClonedMT;

            RETURN_IF_FAILED(MFCreateMediaType(&spClonedMT));
            RETURN_IF_FAILED(pSourceMediaType->CopyAllItems(spClonedMT.get()));
            RETURN_IF_FAILED(spClonedMT->QueryInterface(IID_PPV_ARGS(ppDestMediaType)));
        }

        return S_OK;
    }

    _Requires_lock_held_(m_Lock)
    HRESULT HWMediaStream::_SetStreamAttributes(
        _In_ IMFAttributes* pAttributeStore
    )
    {
        RETURN_HR_IF_NULL(E_INVALIDARG, pAttributeStore);

        // adjust accordingly if other types of stream are wrapped
        RETURN_IF_FAILED(pAttributeStore->SetGUID(MF_DEVICESTREAM_STREAM_CATEGORY, PINNAME_VIDEO_CAPTURE));
        RETURN_IF_FAILED(pAttributeStore->SetUINT32(MF_DEVICESTREAM_STREAM_ID, 0)); // to be fixed
        RETURN_IF_FAILED(pAttributeStore->SetUINT32(MF_DEVICESTREAM_FRAMESERVER_SHARED, 1));
        RETURN_IF_FAILED(pAttributeStore->SetUINT32(MF_DEVICESTREAM_ATTRIBUTE_FRAMESOURCE_TYPES, MFFrameSourceTypes::MFFrameSourceTypes_Color));

        return S_OK;
    }

    HRESULT HWMediaStream::Initialize(_In_ HWMediaSource* pSource, _In_ IMFStreamDescriptor* pStreamDesc, DWORD dwWorkQueue)
    {
        DEBUG_MSG(L"Initialize enter");
        RETURN_HR_IF_NULL(E_INVALIDARG, pSource);
        m_parent = pSource;
        //m_spStreamDesc = pStreamDesc;
        
        RETURN_HR_IF_NULL(E_INVALIDARG, pStreamDesc);
        wil::com_ptr_nothrow<IMFStreamDescriptor> spStreamDesc = pStreamDesc;

        RETURN_IF_FAILED(spStreamDesc->GetStreamIdentifier(&m_dwStreamIdentifier));


        wil::com_ptr_nothrow<IMFMediaTypeHandler> spSourceStreamMediaTypeHandler;
        RETURN_IF_FAILED(spStreamDesc->GetMediaTypeHandler(&spSourceStreamMediaTypeHandler));

        ULONG ulMediaTypeCount = 0;
        ULONG validMediaTypeCount = 0;
        RETURN_IF_FAILED(spSourceStreamMediaTypeHandler->GetMediaTypeCount(&ulMediaTypeCount));
        wil::unique_cotaskmem_array_ptr<wil::com_ptr_nothrow<IMFMediaType>> sourceStreamMediaTypeList =
            wilEx::make_unique_cotaskmem_array<wil::com_ptr_nothrow<IMFMediaType>>(ulMediaTypeCount);
        RETURN_IF_NULL_ALLOC(sourceStreamMediaTypeList.get());
        
        for (DWORD i = 0; i < ulMediaTypeCount; i++)
        {
            wil::com_ptr_nothrow<IMFMediaType> spMediaType;
            RETURN_IF_FAILED(spSourceStreamMediaTypeHandler->GetMediaTypeByIndex(i, &spMediaType));

            UINT width = 0, height = 0;
            UINT32 numerator, denominator;
            MFGetAttributeSize(spMediaType.get(), MF_MT_FRAME_SIZE, &width, &height);
            MFGetAttributeSize(spMediaType.get(), MF_MT_FRAME_RATE, &numerator,&denominator);

            // Select the desired stream format here
             if (width == 1280 && height == 720 && numerator/(double)denominator > 11)
            {
                DEBUG_MSG(L"Found a valid and compliant Mediatype=%u", i);

                // copy the MediaType and cache it
                wil::com_ptr_nothrow<IMFMediaType> spMediaTypeCopy;
                RETURN_IF_FAILED(CloneMediaType(spMediaType.get(), &spMediaTypeCopy));

                m_mediaTypeCache.push_back(spMediaTypeCopy);
                sourceStreamMediaTypeList[validMediaTypeCount] = spMediaTypeCopy.detach();
                validMediaTypeCount++;
            }
        }

        if (validMediaTypeCount == 0)
        {
            DEBUG_MSG(L"Did not find any valid compliant Mediatype");
            return MF_E_INVALIDMEDIATYPE;
        }
        wil::unique_cotaskmem_array_ptr<wil::com_ptr_nothrow<IMFMediaType>> m_mediaTypeList =
            wilEx::make_unique_cotaskmem_array<wil::com_ptr_nothrow<IMFMediaType>>(validMediaTypeCount);
        RETURN_IF_NULL_ALLOC(m_mediaTypeList.get());

        for (DWORD i = 0; i < validMediaTypeCount; i++)
        {
            m_mediaTypeList[i] = sourceStreamMediaTypeList[i];
        }

        RETURN_IF_FAILED(MFCreateAttributes(&m_spAttributes, 10));
        RETURN_IF_FAILED(spStreamDesc->CopyAllItems(m_spAttributes.get()));
        RETURN_IF_FAILED(_SetStreamAttributes(m_spAttributes.get()));

        wil::com_ptr_nothrow<IMFMediaTypeHandler> spTypeHandler;

        // create our stream descriptor
        RETURN_IF_FAILED(MFCreateStreamDescriptor(
            0, //StreamId
            validMediaTypeCount, //MT count
            m_mediaTypeList.get(), &m_spStreamDesc));
        RETURN_IF_FAILED(m_spStreamDesc->GetMediaTypeHandler(&spTypeHandler));
        RETURN_IF_FAILED(spTypeHandler->SetCurrentMediaType(m_mediaTypeList[0]));

        // set same attributes on stream descriptor as on stream attribute store
        RETURN_IF_FAILED(spStreamDesc->CopyAllItems(m_spStreamDesc.get()));
        RETURN_IF_FAILED(_SetStreamAttributes(m_spStreamDesc.get())); 
        
        RETURN_IF_FAILED(MFCreateEventQueue(&m_spEventQueue));
        //RETURN_IF_FAILED(m_spStreamDesc->GetStreamIdentifier(&m_dwStreamIdentifier));
        m_dwSerialWorkQueueId = dwWorkQueue;

        auto ptr = winrt::make_self<CAsyncCallback<HWMediaStream>>(this, &HWMediaStream::OnMediaStreamEvent, m_dwSerialWorkQueueId);
        m_xOnMediaStreamEvent.attach(ptr.detach());

        DEBUG_MSG(L"Initialize exit, streamId: %d", m_dwStreamIdentifier);
        return S_OK;
    }

    // IMFMediaEventGenerator
    IFACEMETHODIMP HWMediaStream::BeginGetEvent(
            _In_ IMFAsyncCallback* pCallback,
            _In_ IUnknown* punkState
        )
    {
        winrt::slim_lock_guard lock(m_Lock);

        RETURN_IF_FAILED(_CheckShutdownRequiresLock());
        RETURN_IF_FAILED(m_spEventQueue->BeginGetEvent(pCallback, punkState));

        return S_OK;
    }

    IFACEMETHODIMP HWMediaStream::EndGetEvent(
            _In_ IMFAsyncResult* pResult,
            _COM_Outptr_ IMFMediaEvent** ppEvent
        )
    {
        winrt::slim_lock_guard lock(m_Lock);

        RETURN_IF_FAILED(_CheckShutdownRequiresLock());
        RETURN_IF_FAILED(m_spEventQueue->EndGetEvent(pResult, ppEvent));

        return S_OK;
    }

    IFACEMETHODIMP HWMediaStream::GetEvent(
            DWORD dwFlags,
            _COM_Outptr_ IMFMediaEvent** ppEvent
        )
    {
        // NOTE:
        // GetEvent can block indefinitely, so we don't hold the lock.
        // This requires some juggling with the event queue pointer.
        wil::com_ptr_nothrow<IMFMediaEventQueue> spQueue;

        {
            winrt::slim_lock_guard lock(m_Lock);

            RETURN_IF_FAILED(_CheckShutdownRequiresLock());
            spQueue = m_spEventQueue;
        }

        // Now get the event.
        RETURN_IF_FAILED(spQueue->GetEvent(dwFlags, ppEvent));

        return S_OK;
    }

    IFACEMETHODIMP HWMediaStream::QueueEvent(
            MediaEventType eventType,
            REFGUID guidExtendedType,
            HRESULT hrStatus,
            _In_opt_ PROPVARIANT const* pvValue
        )
    {
        winrt::slim_lock_guard lock(m_Lock);

        RETURN_IF_FAILED(_CheckShutdownRequiresLock());
        RETURN_IF_FAILED(m_spEventQueue->QueueEventParamVar(eventType, guidExtendedType, hrStatus, pvValue));

        return S_OK;
    }

    // IMFMediaStream
    IFACEMETHODIMP HWMediaStream::GetMediaSource(
            _COM_Outptr_ IMFMediaSource** ppMediaSource
        )
    {
        winrt::slim_lock_guard lock(m_Lock);

        RETURN_HR_IF_NULL(E_POINTER, ppMediaSource);
        *ppMediaSource = nullptr;

        RETURN_IF_FAILED(_CheckShutdownRequiresLock());

        RETURN_IF_FAILED(m_parent.copy_to(ppMediaSource));

        return S_OK;
    }

    IFACEMETHODIMP HWMediaStream::GetStreamDescriptor(
            _COM_Outptr_ IMFStreamDescriptor** ppStreamDescriptor
        )
    {
        winrt::slim_lock_guard lock(m_Lock);

        RETURN_HR_IF_NULL(E_POINTER, ppStreamDescriptor);
        *ppStreamDescriptor = nullptr;

        RETURN_IF_FAILED(_CheckShutdownRequiresLock());

        if (m_spStreamDesc != nullptr)
        {
            RETURN_IF_FAILED(m_spStreamDesc.copy_to(ppStreamDescriptor));
        }
        else
        {
            return E_UNEXPECTED;
        }

        return S_OK;
    }

    IFACEMETHODIMP HWMediaStream::RequestSample(
            _In_ IUnknown* pToken
        )
    {
        winrt::slim_lock_guard lock(m_Lock);
        RETURN_IF_FAILED(_CheckShutdownRequiresLock());

        RETURN_IF_FAILED(m_spDevStream->RequestSample(pToken));

        return S_OK;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // IMFMediaStream2
    IFACEMETHODIMP HWMediaStream::SetStreamState(MF_STREAM_STATE state)
    {
        winrt::slim_lock_guard lock(m_Lock);

        RETURN_IF_FAILED(_CheckShutdownRequiresLock());
        DEBUG_MSG(L"[%d] SetStreamState %d", m_dwStreamIdentifier, state);

        wil::com_ptr_nothrow<IMFMediaStream2> spStream2;
        RETURN_IF_FAILED(m_spDevStream->QueryInterface(IID_PPV_ARGS(&spStream2)));
        RETURN_IF_FAILED(spStream2->SetStreamState(state));

        return S_OK;
    }
    
    IFACEMETHODIMP HWMediaStream::GetStreamState(_Out_ MF_STREAM_STATE* pState)
    {
        winrt::slim_lock_guard lock(m_Lock);

        RETURN_HR_IF_NULL(E_POINTER, pState);
        RETURN_IF_FAILED(_CheckShutdownRequiresLock());

        wil::com_ptr_nothrow<IMFMediaStream2> spStream2;
        RETURN_IF_FAILED(m_spDevStream->QueryInterface(IID_PPV_ARGS(&spStream2)));
        RETURN_IF_FAILED(spStream2->GetStreamState(pState));

        return S_OK;
    }

    HRESULT HWMediaStream::OnMediaStreamEvent(_In_ IMFAsyncResult* pResult)
    {
        // Forward deviceStream event
        wil::com_ptr_nothrow<IUnknown> spUnknown;
        RETURN_IF_FAILED(pResult->GetState(&spUnknown));

        wil::com_ptr_nothrow<IMFMediaStream> spMediaStream;
        RETURN_IF_FAILED(spUnknown->QueryInterface(IID_PPV_ARGS(&spMediaStream)));

        wil::com_ptr_nothrow<IMFMediaEvent> spEvent;
        RETURN_IF_FAILED(spMediaStream->EndGetEvent(pResult, &spEvent));
        RETURN_HR_IF_NULL(MF_E_UNEXPECTED, spEvent);

        MediaEventType met;
        RETURN_IF_FAILED(spEvent->GetType(&met));
        DEBUG_MSG(L"[%d] OnMediaStreamEvent, streamId: %d, met:%d ", m_dwStreamIdentifier, met);

        // This shows how to intercept sample from physical camera
        // and do custom processin gon the sample.
        // 
        bool bForwardEvent = true;
        if (met == MEMediaSample)
        {
            wil::com_ptr_nothrow<IMFSample> spSample;
            wil::com_ptr_nothrow<IUnknown> spToken;
            wil::com_ptr_nothrow<IUnknown> spSampleUnk;

            wil::unique_prop_variant propVar = {};
            RETURN_IF_FAILED(spEvent->GetValue(&propVar));
            if (VT_UNKNOWN != propVar.vt)
            {
                RETURN_HR(MF_E_UNEXPECTED);
            }
            spSampleUnk = propVar.punkVal;
            RETURN_IF_FAILED(spSampleUnk->QueryInterface(IID_PPV_ARGS(&spSample)));

            RETURN_IF_FAILED(ProcessSample(spSample.get()));
            bForwardEvent = false;
        }

        {
            winrt::slim_lock_guard lock(m_Lock);
            if (SUCCEEDED(_CheckShutdownRequiresLock()))
            {
                // Forward event
                if (bForwardEvent)
                {
                    RETURN_IF_FAILED(m_spEventQueue->QueueEvent(spEvent.get()));
                }

                // Continue listening to source event
                RETURN_IF_FAILED(spMediaStream->BeginGetEvent(m_xOnMediaStreamEvent.get(), m_spDevStream.get()));
            }
        }
        
        DEBUG_MSG(L"[%d] OnMediaStreamEvent exit", m_dwStreamIdentifier);
        return S_OK;
    }


    static long long toInteger(LARGE_INTEGER const& integer)
    {
#ifdef INT64_MAX // Does the compiler natively support 64-bit integers?
        return integer.QuadPart;
#else
        return (static_cast<long long>(integer.HighPart) << 32) | integer.LowPart;
#endif
    }

    // signed long long to LARGE_INTEGER object:

    static LARGE_INTEGER toLargeInteger(long long value)
    {
        LARGE_INTEGER result;

#ifdef INT64_MAX // Does the compiler natively support 64-bit integers?
        result.QuadPart = value;
#else
        result.high_part = value & 0xFFFFFFFF00000000;
        result.low_part = value & 0xFFFFFFFF;
#endif
        return result;
    }


    unsigned long long ProcessPixels(BYTE* pbuf, DWORD bufferLength,  DWORD newBufferLength) {

        // Initialize COM.
        //HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

        IWICImagingFactory* piFactory = NULL;
        IWICBitmapDecoder* piDecoder = NULL;
        IWICStream* piStream1;
        IWICStream* piStream2;
        BYTE* out = (BYTE*)malloc(newBufferLength * sizeof(BYTE)); // output buffer

        // Create the COM imaging factory.
        //if (SUCCEEDED(hr))
        //{
        HRESULT   hr = CoCreateInstance(CLSID_WICImagingFactory,
                NULL, CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&piFactory));
        //}



        if (SUCCEEDED(hr))
        {
            piFactory->CreateStream(&piStream1);
        }

        // Initialize our origin stream.
        if (SUCCEEDED(hr))
        {
            hr = piStream1->InitializeFromMemory((unsigned char*)pbuf, bufferLength);
        }


        if (SUCCEEDED(hr))
        {
            piFactory->CreateStream(&piStream2);
        }

        // Initialize our new stream.
        if (SUCCEEDED(hr))
        {
            hr = piStream2->InitializeFromMemory((unsigned char*)out, newBufferLength);
        }



        // Create the decoder.
        if (SUCCEEDED(hr))
        {
            hr = piFactory->CreateDecoderFromStream(piStream1, NULL, WICDecodeMetadataCacheOnDemand, &piDecoder);
        }


        // Variables used for encoding.
        IWICBitmapEncoder* piEncoder = NULL;
        IWICMetadataBlockWriter* piBlockWriter = NULL;
        IWICMetadataBlockReader* piBlockReader = NULL;

        WICPixelFormatGUID pixelFormat = { 0 };
        UINT count = 0;
        double dpiX, dpiY = 0.0;
        UINT width = 0, height = 0;

        // Create the encoder.
        if (SUCCEEDED(hr))
        {
            hr = piFactory->CreateEncoder(GUID_ContainerFormatJpeg, NULL, &piEncoder);
        }


        // Initialize the encoder
        if (SUCCEEDED(hr))
        {
            hr = piEncoder->Initialize(piStream2, WICBitmapEncoderNoCache);
        }

        if (SUCCEEDED(hr))
        {
            hr = piDecoder->GetFrameCount(&count);
        }


        if (SUCCEEDED(hr))
        {
            // Process each frame of the image.
            for (UINT i = 0; i < count && SUCCEEDED(hr); i++)
            {
                // Frame variables.
                IWICBitmapFrameDecode* piFrameDecode = NULL;
                IWICBitmapFrameEncode* piFrameEncode = NULL;
                IWICMetadataQueryReader* piFrameQReader = NULL;
                IWICMetadataQueryWriter* piFrameQWriter = NULL;



                // Get and create the image frame.
                if (SUCCEEDED(hr))
                {
                    hr = piDecoder->GetFrame(i, &piFrameDecode);
                }
                if (SUCCEEDED(hr))
                {
                    hr = piEncoder->CreateNewFrame(&piFrameEncode, NULL);
                }


                // Initialize the encoder.
                if (SUCCEEDED(hr))
                {
                    hr = piFrameEncode->Initialize(NULL);
                }


                // Get and set the size.
                if (SUCCEEDED(hr))
                {
                    hr = piFrameDecode->GetSize(&width, &height);
                }
                if (SUCCEEDED(hr))
                {
                    hr = piFrameEncode->SetSize(width, height);
                }
                // Get and set the resolution.
                if (SUCCEEDED(hr))
                {
                    piFrameDecode->GetResolution(&dpiX, &dpiY);
                }
                if (SUCCEEDED(hr))
                {
                    hr = piFrameEncode->SetResolution(dpiX, dpiY);
                }
                // Set the pixel format.
                if (SUCCEEDED(hr))
                {
                    piFrameDecode->GetPixelFormat(&pixelFormat);
                }
                if (SUCCEEDED(hr))
                {
                    hr = piFrameEncode->SetPixelFormat(&pixelFormat);
                }


                bool formatsEqual = FALSE;
                if (SUCCEEDED(hr))
                {
                    GUID srcFormat;
                    GUID destFormat;

                    hr = piDecoder->GetContainerFormat(&srcFormat);
                    if (SUCCEEDED(hr))
                    {
                        hr = piEncoder->GetContainerFormat(&destFormat);
                    }
                    if (SUCCEEDED(hr))
                    {
                        if (srcFormat == destFormat)
                            formatsEqual = true;
                        else
                            formatsEqual = false;
                    }
                }

                // Process on pixels

                IWICBitmap* Bitmap;

                WICRect Rect = { 0, 0, (int)width, (int)height };
                IWICBitmapLock* Lock;
                UINT buff_size = 0;
                BYTE* buf = nullptr;

                piFactory->CreateBitmapFromSource(piFrameDecode, WICBitmapCacheOnDemand, &Bitmap);
                Bitmap->Lock(&Rect, WICBitmapLockRead | WICBitmapLockWrite, &Lock);
                Lock->GetDataPointer(&buff_size, &buf);

                cv::Mat rawData(height, width, CV_8UC3, (void*)buf);

                //cv::Mat encoded = encode(rawData, (int)width, (int)height);

                cv::Mat encoded = qrCodeGenerator(rawData, (int)width, (int)height);

                memcpy(buf, encoded.data, encoded.total()* encoded.elemSize());//buff_size);
                
                Lock->Release();


                if (SUCCEEDED(hr))
                {
                    /*hr = piFrameEncode->WriteSource(
                        static_cast<IWICBitmapSource*> (piFrameDecode),
                        NULL); // Using NULL enables JPEG loss-less encoding.*/

                    hr = piFrameEncode->WriteSource(
                        static_cast<IWICBitmapSource*> (Bitmap),
                        NULL);
                }

                // Commit the frame.
                if (SUCCEEDED(hr))
                {
                    hr = piFrameEncode->Commit();
                }

                if (piFrameDecode)
                {
                    piFrameDecode->Release();
                }

                if (piFrameEncode)
                {
                    piFrameEncode->Release();
                }

                if (piFrameQReader)
                {
                    piFrameQReader->Release();
                }

                if (piFrameQWriter)
                {
                    piFrameQWriter->Release();
                }

                if (Bitmap) {
                    Bitmap->Release();
                }
            }
        }




        if (SUCCEEDED(hr))
        {
            piEncoder->Commit();
        }

        
        unsigned long long currentLength = 0;
        if (SUCCEEDED(hr))
        {
            LARGE_INTEGER  dlibMove;
            ULARGE_INTEGER plibNewPosition;
            piStream2->Seek(toLargeInteger(0), STREAM_SEEK_CUR, &plibNewPosition);
            currentLength = plibNewPosition.QuadPart; // to do : May lose data
            memcpy(pbuf, out, currentLength);
            free(out);
        }

        if (SUCCEEDED(hr))
        {
            piStream1->Commit(STGC_DEFAULT);
            piStream2->Commit(STGC_DEFAULT);
        }

        if (piStream1)
        {
            piStream1->Release();
        }

        if (piStream2)
        {
            piStream2->Release();
        }

        if (piEncoder)
        {
            piEncoder->Release();
        }
        if (piBlockWriter)
        {
            piBlockWriter->Release();
        }
        if (piBlockReader)
        {
            piBlockReader->Release();
        }

        return currentLength;
    }


    HRESULT HWMediaStream::ProcessSample(_In_ IMFSample* inputSample)
    {
        winrt::slim_lock_guard lock(m_Lock);
         
         // Do custom processing on the sample from the physical camera
        wil::com_ptr_nothrow<IMFMediaBuffer> spBuffer;
        DWORD bufferLength = 0;
        DWORD pcbMaxLength = 0;
        BYTE * pbuf = nullptr;


        HRESULT hr = inputSample->ConvertToContiguousBuffer(&spBuffer);
        spBuffer->Lock(&pbuf, &pcbMaxLength, &bufferLength);
        auto currentLen = ProcessPixels(pbuf, bufferLength, pcbMaxLength);
        spBuffer->SetCurrentLength(currentLen);
        spBuffer->Unlock();
         
        
        if (SUCCEEDED(_CheckShutdownRequiresLock()))
        {
            // queue event
            RETURN_IF_FAILED(m_spEventQueue->QueueEventParamUnk(
                MEMediaSample,
                GUID_NULL,
                S_OK,
                inputSample));
        }
        return S_OK;
    }

    HRESULT HWMediaStream::Shutdown()
    {
        HRESULT hr = S_OK;
        winrt::slim_lock_guard lock(m_Lock);

        m_isShutdown = true;
        m_parent.reset();
        m_spDevStream.reset();

        if (m_spEventQueue != nullptr)
        {
            hr = m_spEventQueue->Shutdown();
            m_spEventQueue.reset();
        }

        m_spStreamDesc.reset();

        m_spAttributes.reset();

        return hr;
    }

    HRESULT HWMediaStream::SetMediaStream(_In_ IMFMediaStream* pMediaStream)
    {
        winrt::slim_lock_guard lock(m_Lock);
        DEBUG_MSG(L"[%d] Set MediaStream %p ", m_dwStreamIdentifier, pMediaStream);

        RETURN_HR_IF_NULL(E_INVALIDARG, pMediaStream);
        RETURN_IF_FAILED(_CheckShutdownRequiresLock());

        m_spDevStream = pMediaStream;
        RETURN_IF_FAILED(m_spDevStream->BeginGetEvent(m_xOnMediaStreamEvent.get(), m_spDevStream.get()));

        return S_OK;
    }

    HRESULT HWMediaStream::_CheckShutdownRequiresLock()
    {
        if (m_isShutdown)
        {
            return MF_E_SHUTDOWN;
        }

        if (m_spEventQueue == nullptr)
        {
            return E_UNEXPECTED;

        }
        return S_OK;
    }
}


