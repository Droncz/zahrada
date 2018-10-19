# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

add_custom_target(webflash
					COMMAND Invoke-WebRequest -Uri "http://192.168.1.51/upload" -Method POST -ContentType "application/octet-stream" -InFile "build/zahrada.bin"
					COMMAND Invoke-WebRequest -Uri "http://192.168.1.51/reboot" -TimeoutSec 4)

# Invoke-WebRequest -Uri "http://192.168.1.51/upload" -Method POST -ContentType "application/octet-stream" -InFile "build/zahrada.bin"
# Invoke-WebRequest -Uri "http://192.168.1.51/reboot" -TimeoutSec 4
					
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(zahrada)

