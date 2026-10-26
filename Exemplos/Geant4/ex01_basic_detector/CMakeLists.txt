cmake_minimum_required(VERSION 3.16)
project(ex01_basic_detector CXX)

# Encontra o pacote Geant4 instalado no sistema
find_package(Geant4 REQUIRED ui_all vis_all)

# Inclui os cabecalhos do Geant4 e do diretorio include local
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Lista de fontes C++
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/main.cc)

# Define o executavel
add_executable(ex01_basic_detector ${SOURCES})
target_link_libraries(ex01_basic_detector ${Geant4_LIBRARIES})

# Copia os arquivos de macro vis.mac e run.mac para o diretorio de build
file(COPY vis.mac run.mac DESTINATION ${CMAKE_BINARY_DIR})
