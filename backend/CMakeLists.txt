cmake_minimum_required(VERSION 3.16)
project(backend CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Drogon CONFIG REQUIRED)

add_subdirectory(../moteur moteur EXCLUDE_FROM_ALL)

aux_source_directory(controllers CTL_SRC)
aux_source_directory(services SVC_SRC)

add_executable(backend main.cc ${CTL_SRC} ${SVC_SRC})
target_include_directories(backend PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(backend PRIVATE Drogon::Drogon magic_io)

add_subdirectory(test)
