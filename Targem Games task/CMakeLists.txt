cmake_minimum_required(VERSION 3.11)

project(HelloCMake CXX)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

add_executable("String" "src/main.cpp" "src/String.h" "src/String.cpp")