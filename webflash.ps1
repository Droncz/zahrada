Invoke-WebRequest -Uri "http://192.168.1.51/upload" -Method POST -ContentType "application/octet-stream" -InFile "build/zahrada.bin"
Invoke-WebRequest -Uri "http://192.168.1.51/reboot" -TimeoutSec 4