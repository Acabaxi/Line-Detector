cmake_minimum_required(VERSION 2.8)
project(LineDetector)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

FIND_PACKAGE( OpenMP REQUIRED)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Compiler 
if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()


add_library(EdgesSubPix EdgesSubPix.cpp EdgesSubPix.h)
target_link_libraries(EdgesSubPix ${OpenCV_LIBS})
add_library(GaussianDerivates GaussianDerivates.cpp GaussianDerivates.h)
target_link_libraries(GaussianDerivates ${OpenCV_LIBS})
add_executable(Edge test_EdgesSubPix.cpp)
target_link_libraries(Edge EdgesSubPix GaussianDerivates ${OpenCV_LIBS})


