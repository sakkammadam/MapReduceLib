cmake_minimum_required(VERSION 3.23)
project(MapReduceLib)

set(CMAKE_CXX_STANDARD 17)

add_library(fpRawInput SHARED FileProcessor/FileProcessorInput.cpp)
add_library(fpMapOutput SHARED FileProcessor/FileProcessorMapOutput.cpp)
add_library(fpShufOutput SHARED FileProcessor/FileProcessorShufOutput.cpp)
add_library(fpRedOutput SHARED FileProcessor/FileProcessorRedOutput.cpp)
add_library(mapper SHARED Mapper/MapperImpl.cpp)
add_library(shuffler SHARED Shuffler/ShufflerImpl.cpp)
add_library(reducer SHARED Reducer/ReducerImpl.cpp)