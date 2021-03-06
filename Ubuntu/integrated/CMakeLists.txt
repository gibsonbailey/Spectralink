SET(execName ZED_Depth_Sensing)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
PROJECT(${execName})

option(LINK_SHARED_ZED "Link with the ZED SDK shared executable" ON)

if (NOT LINK_SHARED_ZED AND MSVC)
    message(FATAL_ERROR "LINK_SHARED_ZED OFF : ZED SDK static libraries not available on Windows")
endif()

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 OLD)
	cmake_policy(SET CMP0015 OLD)
endif(COMMAND cmake_policy)

SET(EXECUTABLE_OUTPUT_PATH ".")
SET(SPECIAL_OS_LIBS "")
 
find_package(ZED 2 REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

IF(NOT WIN32)
    SET(SPECIAL_OS_LIBS "pthread" "X11")
    add_definitions(-Wno-write-strings -fpermissive)
ENDIF()

find_package(CUDA ${ZED_CUDA_VERSION} EXACT REQUIRED)

include_directories(${ZED_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_PATH})
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(/home/linuxbrew/.linuxbrew/Cellar/openal-soft/1.18.2/include/)
include_directories(/home/nvidia/Downloads/openal-testing/nas-1.9.4/lib/audio/)
include_directories(/home/nvidia/Downloads/openal-testing/nas0.9.4/lib/)
include_directories(/home/nvidia/Downloads/openal-testing/nas-1.9.4/include/)
include_directories(/usr/include/glib-2.0/)
include_directories(/usr/lib/aarch64-linux-gnu/glib-2.0/include/)
include_directories(/usr/include/gstreamer-1.0/)
include_directories(/usr/lib/aarch64-linux-gnu/gstreamer-1.0/include/)
include_directories(/usr/include/gstreamer-1.0/gst/audio/)
include_directories(/usr/include/audio/)
include_directories(/objects/)




link_directories(${ZED_LIBRARY_DIR})
link_directories(${GLEW_LIBRARY_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
link_directories(${CUDA_LIBRARY_DIRS})
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
link_directories(/usr/lib/libopencv_core.so.3.3.1)
link_directories(/usr/lib/libopencv_highgui.so.3.3.1)
link_directories(/usr/lib/libopencv_imgproc.so.3.3.1)


FILE(GLOB_RECURSE SRC_FILES src/*.cpp)
FILE(GLOB_RECURSE HDR_FILES include/*.hpp)

ADD_EXECUTABLE(${execName} ${HDR_FILES} ${SRC_FILES})
add_definitions(-std=c++11 -g -O3 -Wl,-rpath=./objects/ )

if (LINK_SHARED_ZED)
    SET(ZED_LIBS ${ZED_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY} ${CUDA_NPP_LIBRARIES_ZED})
else()
    SET(ZED_LIBS ${ZED_STATIC_LIBRARIES} ${CUDA_CUDA_LIBRARY} ${CUDA_LIBRARY})
endif()

TARGET_LINK_LIBRARIES(${execName} 
                        ${SPECIAL_OS_LIBS} 
                        ${ZED_LIBS} 
                        ${OPENGL_LIBRARIES}
                        ${GLUT_LIBRARY}
                        ${GLEW_LIBRARY}
                        /usr/lib/libopencv_core.so.3.3.1
                        /usr/lib/libopencv_imgproc.so.3.3.1
                        /usr/lib/libopencv_highgui.so.3.3.1 
                        /usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/integrated/objects/libopenal.so.1.16.0
                        /usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/integrated/objects/wave.o
                        /usr/local/zed/sample/Y/depth_sensing/Spectralink/Ubuntu/integrated/objects/fileutil.o )

