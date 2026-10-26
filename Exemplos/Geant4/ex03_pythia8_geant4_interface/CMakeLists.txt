cmake_minimum_required(VERSION 3.16)
project(ex03_pythia8_geant4_interface CXX)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# Tenta localizar o Pythia8 no sistema
find_library(PYTHIA8_LIBRARY NAMES pythia8 PATHS /usr/lib /usr/local/lib $ENV{PYTHIA8}/lib)
find_path(PYTHIA8_INCLUDE_DIR NAMES Pythia8/Pythia.h PATHS /usr/include /usr/local/include $ENV{PYTHIA8}/include)

if(PYTHIA8_INCLUDE_DIR AND PYTHIA8_LIBRARY)
    include_directories(${PYTHIA8_INCLUDE_DIR})
else()
    message(STATUS "Pythia8 nao encontrado no sistema padrao. Especifique -DPYTHIA8_DIR=/caminho/para/pythia")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/main.cc)

add_executable(ex03_pythia8_geant4_interface ${SOURCES})
target_link_libraries(ex03_pythia8_geant4_interface ${Geant4_LIBRARIES} ${PYTHIA8_LIBRARY})

file(COPY pythia_config.cmnd run.mac DESTINATION ${CMAKE_BINARY_DIR})
