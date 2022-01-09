cmake_minimum_required (VERSION 3.6)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)


project(teste LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if (UNIX)
    set(CMAKE_CXX_FLAGS "-Wall -pedantic -fPIC ${CMAKE_CXX_FLAGS} -DLIN64 -Wmisleading-indentation -Wunused-but-set-variable")
endif()

option(TRAVIS_BUILD "Use Travis build configuration" OFF)
option(PERCY_USE_SYRUP "Use glucose-syrup instead of glucose" ON)
option(PERCY_USE_GLUCOSE "Use glucose instead of glucose-syrup" OFF)
option(PERCY_BUILD_CMS "Enables support for CryptoMinisat" OFF)
option(STATIC_LIBABC "Builds libabc as a static library" ON)
option(PERCY_SATOKO "Enable support for the Satoko solver" ON)

if (${TRAVIS_BUILD})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTRAVIS_BUILD -Wno-sign-compare -Wno-parentheses")
endif()

if (${PERCY_BUILD_CMS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_CMS")
    set(ONLY_SIMPLE ON CACHE BOOL "Build simple version of CMS")
    set(ENABLE_PYTHON_INTERFACE OFF CACHE BOOL "Do not build Python inferface")
    set(IPASIR OFF CACHE BOOL "We do not need IPASIR")
    add_subdirectory(cryptominisat)
    SET(CMS_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/cryptominisat/cmsat5-src") 
    if (WIN32)
        link_directories("${PROJECT_BINARY_DIR}/cryptominisat/lib/Debug")
        link_directories("${PROJECT_BINARY_DIR}/cryptominisat/lib/Release")
    else()
        SET(CMS_LDIR "${CMAKE_CURRENT_BINARY_DIR}/cryptominisat/lib") 
        link_directories(${CMS_LDIR})
    endif()
endif()


######################## Faz a Main
add_subdirectory(include)
include_directories(include)

add_executable(main main.cpp)
target_link_libraries(main percy)
target_link_libraries(main ${ZLIB_LIBRARIES})
