# *mTunnel* Video Decoder

### Requirement:

- Visual studio 2022
- FFmpeg 5.0+
- OpenCV 4.8.0+

This project captures the display buffer of a targeted window and decodes the 2D bar code(encoded by *mTunnel* Video Encoder) within it. It also recomposites and displays the decoded video streams over the targeted window.

### Getting started

Download the project

1. Build *DecoderDemo* (Release configuration)

2. Go to "DecoderDemo"->"Properties" -> "Debugging" and fill in the "Command-line arguments" with your targeted window, for example, Zoom Meeting.

3. This project utilizes *GdiGrab* to capture target windows, so hardware video acceleration should be disabled. You can also capture the whole desktop to avoid the issues. Video flip should also be disabled.

4. Run *DecoderDemo*


