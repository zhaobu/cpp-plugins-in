cmake_minimum_required(VERSION 3.14)
set(CMAKE_TOOLCHAIN_FILE "3rd/vcpkg/scripts/buildsystems/vcpkg.cmake"
  CACHE STRING "Vcpkg toolchain file")

project(cplusplus CXX C)


add_compile_options(-Wno-unused-function)

add_subdirectory(demo)
# add_subdirectory(src)