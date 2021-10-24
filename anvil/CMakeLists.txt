cmake_minimum_required(VERSION 3.13)

add_subdirectory(reactphysics3d)
add_subdirectory(ecs)

project(anvil)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")
set(CMAKE_CXX_STANDARD 20)

add_executable(anvil
               anvil.m.cpp
               anvil.cpp
               camera.cpp
               inspector.cpp
               rendering.cpp
               scene_utils.cpp
               particle_system.cpp
               physics_system.cpp)

target_link_libraries(anvil PRIVATE Sprocket reactphysics3d anvil-ecs)
target_include_directories(anvil PUBLIC .)

add_executable(runtime
               runtime.m.cpp
               runtime.cpp
               rendering.cpp
               scene_utils.cpp
               particle_system.cpp
               physics_system.cpp)

target_link_libraries(runtime PRIVATE Sprocket reactphysics3d anvil-ecs)
target_include_directories(runtime PUBLIC .)