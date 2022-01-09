######################## NAUTY
include_directories(${PROJECT_SOURCE_DIR})

execute_process(COMMAND "./configure"
		WORKING_DIRECTORY ./
		RESULT_VARIABLE CONF_RESULT
		OUTPUT_VARIABLE CONF_OUTPUT)

file(GLOB FILENAMES *.c)

list(REMOVE_ITEM FILENAMES poptest.c)
list(REMOVE_ITEM FILENAMES sorttemplates.c)
list(REMOVE_ITEM FILENAMES testg.c)
list(REMOVE_ITEM FILENAMES splay.c)
list(REMOVE_ITEM FILENAMES sumlines.c)

if (UNIX)
    set(CMAKE_C_FLAGS "-w -Wmisleading-indentation -Wunused-but-set-variable")
endif()

add_library(nauty INTERFACE)
target_include_directories(nauty INTERFACE ${PROJECT_SOURCE_DIR}/include)

######################## PERCY
add_library(percy INTERFACE)

target_include_directories(percy INTERFACE ${PROJECT_SOURCE_DIR}/include)

if (${PERCY_BUILD_CMS})
    target_include_directories(percy INTERFACE ${CMS_INCLUDE_DIR})
    add_dependencies(percy libcryptominisat5)
endif()

if (${PERCY_SATOKO})
    target_compile_definitions(percy INTERFACE USE_SATOKO)
    target_link_libraries(percy INTERFACE satoko)
else()
    target_compile_definitions(percy INTERFACE DISABLE_SATOKO)
endif()

target_link_libraries(percy INTERFACE kitty libabcsat)

if (${DISABLE_NAUTY})
    target_compile_definitions(percy INTERFACE DISABLE_NAUTY)
else()
    target_link_libraries(percy INTERFACE nauty)
endif()
if (UNIX)
    if (${PERCY_USE_SYRUP})
        target_compile_definitions(percy INTERFACE USE_SYRUP)
        target_link_libraries(percy INTERFACE syrup pthread)
    elseif(${PERCY_USE_GLUCOSE})
        target_compile_definitions(percy INTERFACE USE_GLUCOSE)
        target_link_libraries(percy INTERFACE glucose)
    endif()
endif()

add_library(kitty INTERFACE)
target_include_directories(kitty INTERFACE ${PROJECT_SOURCE_DIR}/include)

######################## ABC
add_subdirectory(abc)
target_compile_definitions(libabcsat PUBLIC "LIN64" ABC_NAMESPACE=pabc ABC_NO_USE_READLINE)
if(ABC_USE_NAMESPACE)
    set_source_files_properties(${ABC_SRC} PROPERTIES LANGUAGE CXX)
endif()
include_directories(abc)

######################## SYRUP
find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
add_subdirectory(syrup)
target_compile_definitions(percy INTERFACE USE_SYRUP)
target_link_libraries(percy INTERFACE syrup pthread)

######################## SATOKO
add_subdirectory(satoko)
target_compile_definitions(percy INTERFACE USE_SATOKO)
target_link_libraries(percy INTERFACE satoko)

######################## LORINA
add_library(lorina INTERFACE)
target_include_directories(lorina INTERFACE ${PROJECT_SOURCE_DIR}/include)
if (ENABLE_COVERAGE)
  target_link_libraries(lorina INTERFACE fmt gcov)
else()
  target_link_libraries(lorina INTERFACE fmt)
endif()