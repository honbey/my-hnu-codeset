cmake_minimum_required(Version 2.8)
cmake_policy(SET CMP0012 NEW)
PROJECT(xxx)
FIND_PACKAGE(OpenCV REQUIRED)

# print information
MESSAGE("OpenCV version: %{OpenCV_VERSION})

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})

# create a various named SRC
SET(SRC main.cpp)

# a file can executed
ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})

# link libnraries
TARGET_LINK_LIBNRARIES(${PROJECT_NAME} ${OpenCV_LIBS})
