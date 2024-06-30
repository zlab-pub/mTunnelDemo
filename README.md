# *mTunnel* Demo

### Brief

This project is a demo for paper **Enforcing End-to-end Security for Remote Conference Applications**. Please refer to [link](https://www.computer.org/csdl/proceedings-article/sp/2024/313000a202/1WPcYvAYYaA) for more information.

This project consists of 3 parts:

1. ***mTunnel* Video Encoder:** This project create a virtual camera on Windows. The virtual camera wraps a physical camera and modulate the incoming video streams into 2D bar codes.

2. ***mTunnel* Video Decoder:** This project captures the display buffer of a targeted window and decodes the 2D bar code(encoded by *mTunnel* Video Encoder) within it. It also recomposites and displays the decoded video streams over the targeted window.

3. **Reed-Solomon:** This part is forked from [GitHub - mersinvald/Reed-Solomon: Reed Solomon BCH encoder and decoder](https://github.com/mersinvald/Reed-Solomon). It is used for RS encoding and decoding in the above two projects.
