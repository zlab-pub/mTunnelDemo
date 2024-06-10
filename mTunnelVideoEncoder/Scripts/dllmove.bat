@ echo off
%1 %2
ver|find "5.">nul&&goto :Admin
mshta vbscript:createobject("shell.application").shellexecute("%~s0","goto :Admin","","runas",1)(window.close)&goto :eof
:Admin
copy "C:\Users\lyl\Desktop\mTunnelDemo\Encoder\x64\Release\VirtualCameraMediaSource.dll" "C:\Program Files\Contoso\VirtualCamera_MSI\"
copy "C:\Users\lyl\Desktop\mTunnelDemo\Encoder\x64\Release\VirtualCameraMediaSource.pdb" "C:\Program Files\Contoso\VirtualCamera_MSI\"
PAUSE