cmake_minimum_required(VERSION 3.14)
project(cli11test CXX C)

# set(BOOST_ROOT ${_VCPKG_INSTALLED_DIR}/vcpkg_installed/x64-linux/include)
# #set(Boost_DIR ${_VCPKG_INSTALLED_DIR}) # don't set this one.
# set(Boost_NO_SYSTEM_PATHS ON) set(Boost_NO_BOOST_CMAKE  ON) # additional set
# this one

find_package(spdlog CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

set(CMAKE_BUILD_TYPE "Debug") # 编译类型(Debug, Release)
set(BINARY_NMAE ${PROJECT_NAME}.out)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/3rd)

# 指定最终的可执行文件的位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加编译参数
add_compile_options(-g -std=c++17 -Wall)
# 向工程添加多个特定的头文件搜索路径 --->相当于指定g++编译器的-I参数
include_directories(${LIBS_DIR}/include)
# 向工程添加多个特定的库文件搜索路径 --->相当于指定g++编译器的-L参数
link_directories(${LIBS_DIR}/libs)

message(Module Build : ${PROJECT_NAME})
# 定义SRC变量，其值为当前目录下所有的源代码文件
file(GLOB_RECURSE source_h_src ${LIBS_DIR}/include/*.h
     ${PROJECT_SOURCE_DIR}/*.h)

file(GLOB_RECURSE source_cpp_src ${LIBS_DIR}/include/*.cpp
     ${PROJECT_SOURCE_DIR}/*.cpp)

# aux_source_directory(. DIR_SRCS)
set(DIR_SRCS ${source_cpp_src} ${source_h_src})

# 编译SRC变量所代表的源代码文件，生成main可执行文件
add_executable(${BINARY_NMAE} ${DIR_SRCS} subcommand/subcommand.cpp)

# 为 target 添加需要链接的库 --->相同于指定g++编译器-l参数
target_link_libraries(${BINARY_NMAE} spdlog::spdlog CLI11::CLI11)
