cmake_minimum_required(VERSION 3.4.1)

add_library(
    native-lib        # Name of your native library
    SHARED
    native-lib.cpp    # Your source file(s)
    ../../../../../TextEncrypterCPP/TextEncrypter.cpp
)

target_include_directories(native-lib
        PRIVATE
        ../../../../../TextEncrypterCPP/ImageLib
)

find_library(
    log-lib
    log
)

target_link_libraries(
    native-lib
    ${log-lib}
)