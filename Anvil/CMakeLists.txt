cmake_minimum_required(VERSION 3.13)

project(Anvil)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

set(CMAKE_CXX_STANDARD 20)

add_executable(Anvil
               anvil.m.cpp
               Anvil.cpp
               Camera.cpp
               Inspector.cpp)

target_link_libraries(Anvil PRIVATE Sprocket)
target_include_directories(Anvil PUBLIC . ..)

add_executable(Runtime
               runtime.m.cpp
               Runtime.cpp)

target_link_libraries(Runtime PRIVATE Sprocket)
target_include_directories(Runtime PUBLIC .. .)