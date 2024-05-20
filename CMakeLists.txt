cmake_minimum_required(VERSION 3.28)

if(WIN32)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules/Windows)
else ()
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
endif()

project(TFMessenger)


find_package(APR REQUIRED libapr-2)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE DEF_FILES src/*.h*)
file(GLOB_RECURSE SRC_FILES src/*.c*)


add_executable(TFMessenger ${DEF_FILES} ${SRC_FILES} exec/HostUnit.cpp)

target_link_libraries(TFMessenger PUBLIC ${APR_LIBRARY})
target_include_directories(TFMessenger PUBLIC src/ ${APR_INCLUDE_DIR})
add_executable(TFExecTest ${DEF_FILES} ${SRC_FILES} exec/ClientTestUnit.cpp)

target_link_libraries(TFExecTest PUBLIC  ${APR_LIBRARY})
target_include_directories(TFExecTest PUBLIC src/ ${APR_INCLUDE_DIR})