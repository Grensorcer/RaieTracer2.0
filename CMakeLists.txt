cmake_minimum_required(VERSION 3.10)

project(RaieTracer2.0 VERSION 0.1 LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG on)
find_package(Threads REQUIRED)

# gtest
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()
# end gtest

set(SRC src/matrix.cc src/texture.cc src/camera.cc src/image.cc src/light.cc src/object.cc src/ray.cc src/scene.cc src/scene_collection.cc src/utils.cc src/blob.cc)

add_executable(RaieTracer src/main.cc ${SRC})
target_compile_features(RaieTracer PUBLIC cxx_std_17)
target_compile_options(RaieTracer PUBLIC -Wall -Werror -Wextra -pedantic -lpthread -march=native -fopt-info-vec-optimized -fopt-info-vec-missed)
target_link_libraries(RaieTracer PRIVATE Threads::Threads)
target_include_directories(RaieTracer PUBLIC
                          "${PROJECT_SOURCE_DIR}/include")

add_executable(Test src/test.cc src/test_vec3.cc src/test_image.cc ${SRC})
target_compile_features(Test PUBLIC cxx_std_17)
target_compile_options(Test PUBLIC -Wall -Werror -Wextra -pedantic)
target_link_libraries(Test gtest_main)
target_include_directories(Test PUBLIC
                          "${PROJECT_SOURCE_DIR}/include")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

