cmake_minimum_required(VERSION 3.10.0)

project(synacor)

file(GLOB_RECURSE synacor_SRC "src/*.c" "src/*.h")

add_executable(synacor ${synacor_SRC})

set(EXECUTABLE_NAME "synacor")

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(synacor PUBLIC DEBUG=1)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_definitions(synacor PUBLIC DEBUG=0)
endif()

install(TARGETS synacor DESTINATION bin)