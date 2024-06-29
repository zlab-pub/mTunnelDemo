# *mTunnel* Video Encoder

### Requirement:

- Minimum OS version: Windows 11 (10.0.22000.0)
- Windows SDK minimum version: [10.0.22000.0](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)
- Visual studio MSI project extension [link](https://marketplace.visualstudio.com/items?itemName=visualstudioclient.MicrosoftVisualStudio2017InstallerProjects)
- Latest VCRuntime redistributable installed [link](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist)
- OpenCV 4.8.0+ (do not install on user path)

This project create a virtual camera on Windows userspace([see API documentation](https://docs.microsoft.com/en-us/windows/win32/api/mfvirtualcamera/)) . The virtual camera wraps a physical camera and modulate the incoming video streams into 2D bar codes.

 This project consists of 3 parts:

1. **VirtualCameraMediaSource** <br>
   MediaSource for the virtual camera. This CustomMediaSource can run in 3 distinc modes: **SimpleMediaSource**, **HWMediaSource** and **AugmentedMediaSource**, but the encoder is only available in **HWMediaSource**. (So, please ignore the other two modes)
   
   **HWMediaSource** (also reffered to as *CameraWrapper* in application) will wrap an existing camera on the system but act as a passthrough.
   
   Documentation on media source: [link](https://docs.microsoft.com/en-us/windows-hardware/drivers/stream/frame-server-custom-media-source#custom-media-source-dll)

2. **VirtualCamera_Installer** <br>
   A packaged Win32 application installed with *VirtualCameraMSI* that handles adding and removing virtual camera instances that are based on the *VirtualCameraMediaSource*.

3. **VirtualCamera_MSI** <br>
   This project packages binaries from *VirtualCamera_Installer* and *VirtualCameraMediaSource* into MSI for deployment.
   The MSI is reponsible for deploying binaries, register media source dll, and removal of the media source and the associate virtual camera.

## Getting started

Download the project 

1. Build *VirtualCamera_Installer* (Release configuration)
2. Build *VirtualCameraMediaSource* (Release configuration)
3. Build *VirtualCamera_MSI*
4. Install *VirtualCamera_MSI*
5. Run *VirtualCamera_Installer* as adminstrator to install a **HWMediaSource** for a physical device.

## VirtualCamera_MSI

----

This project creates a msi that is used to register and remove VirtualCameraMediasource.dll.  On installation, the dll will be register on the system such that media source can be use to create virtual camera. On uninstallation, all virtual cameras created with the this VirtualCameraMediasource.dll will be uninstalled.

If you want to replace the old camera source, simply substitute a new media source for the old one in the installation path.

### FAQ

----

<b> 1. CoCreateInstance of the dll failed with MOD_NOT_FOUND ?</b> <br/>
Check to ensure all dependencies required by the mediasource are registered.  Use dumpbin to find all the dependencies of the dll.

* Open Visual Studio command prompt (In Visual Studio ->select menu "Tools" -> Command line -> Developer Command prompt)

* run dumpbin 
  
  ```
  dumpbin /DEPENDENTS mediasource.dll
  ```

<b> 2. How to debug issues with media source? </b> <br/>
During VirtualCamera creation the mediaSource is loaded in the FrameServerMonitor service.  If you encounter an issue prior to starting the virtual camera <br/>
you will need to attach the debugger to the FrameServerMonitor service.

* Open task manager -> view tab "Services" -> look for the FrameServerMonitor service
* Attach debugger to the process
* Alternative: use the powershell script provided: scripts\Debug-FrameServerMonitor.ps1 

In the case where  the virtualCamera is used by application or after VirtualCamera has started, the media source is loaded in the FrameServer service. To investigate an issue with the mediasource you will need to attach the debugger to the FrameServer service.

* Open task manager -> view tab "Services" -> look for the FrameServer service 
* Attach debugger to the process
* Alternative: use the powershell script provided: scripts\Debug-FrameServer.ps1

<b> 3. Can I create a virtual camera that wraps another existing virtual camera with this sample </b><br/>yes, but in this sample virtual camera inception is not currently supported.

<b> 4. Can 2 virtual cameras concurrently wrap the same existing camera on the system? </b><br/>
no, only 1 virtual camera at a time can wrap an existing camera. You cannot open an existing camera in sharing mode from within your virtual camera.

For more information, you may visit [microsoft/Windows-Camera: Tools and samples for camera related APIs on Windows (github.com)](https://github.com/microsoft/Windows-Camera).
