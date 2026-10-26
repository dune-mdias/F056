cmake_minimum_required(VERSION 3.16)
project(ex02_sensitive_detector_scoring CXX)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/main.cc)

add_executable(ex02_sensitive_detector_scoring ${SOURCES})
target_link_libraries(ex02_sensitive_detector_scoring ${Geant4_LIBRARIES})

file(COPY vis.mac run.mac DESTINATION ${CMAKE_BINARY_DIR})
