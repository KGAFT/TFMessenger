cmake_minimum_required(VERSION 3.28)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
project(TFMessenger)


find_package(APR REQUIRED)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE DEF_FILES src/*.h*)
file(GLOB_RECURSE SRC_FILES src/*.c*)


add_executable(TFMessenger ${DEF_FILES} ${SRC_FILES} exec/HostUnit.cpp)

target_link_libraries(TFMessenger PUBLIC ${APR_LIBRARY})
target_include_directories(TFMessenger PUBLIC ${APR_INCLUDE_DIR} src/)
add_executable(TFExecTest ${DEF_FILES} ${SRC_FILES} exec/ClientTestUnit.cpp)

target_link_libraries(TFExecTest PUBLIC ${APR_LIBRARY})
target_include_directories(TFExecTest PUBLIC ${APR_INCLUDE_DIR} src/)