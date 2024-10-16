
#CMAKE_HOST_SYSTEM_NAME

cmake_minimum_required(VERSION 3.10.0)

#Make fat binary for apple to work on both intel and arm
if(APPLE)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS -Ofast)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

if(EXISTS "${PARENT_DIR}/SrcExportDir/HvHeavy.h")
	set(SRC_INCLUDE "../SrcExportDir/*")
	set(SRC_INCLUDE_DIR "../SrcExportDir/")
	message(STATUS "c folder selected")
else()
	set(SRC_INCLUDE "../SrcExportDir/c/*")
	set(SRC_INCLUDE_DIR "../SrcExportDir/c/")
	message(STATUS "c nested")
endif()

file(GLOB HEAVYPD_FILES ${SRC_INCLUDE})
file(GLOB WRAPPER_FILES "../HeavyPdWrapper/*")

foreach(FILE_PATH ${HEAVYPD_FILES})
	get_filename_component(FILE_NAME ${FILE_PATH} NAME)
	string(REGEX MATCH "_([^_]+)\\.cpp$" FULL_NAME ${FILE_NAME})
	if(FULL_NAME)
		string(REGEX REPLACE "^.*_([^_]+)\\.cpp$" "\\1" PLUGIN_NAME ${FILE_NAME})
       		break()
    	endif()
endforeach()





project(${PLUGIN_NAME})

configure_file(${PARENT_DIR}/HeavyPdWrapper/HeavyPdWrapper.hpp.in ${CMAKE_BINARY_DIR}/HeavyPdWrapper/generated/HeavyPdWrapper.hpp)
file(GLOB GENERATED_FILES "${CMAKE_BINARY_DIR}/HeavyPdWrapper/generated/*")

#make library for potential multiple targets
add_library(oLib OBJECT ${WRAPPER_FILES} ${HEAVYPD_FILES} ${GENERATED_FILES})
set_target_properties(oLib PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:oLib>)

if(APPLE)
	add_library(${PROJECT_NAME}_bundle MODULE $<TARGET_OBJECTS:oLib>)
	set_target_properties(${PROJECT_NAME}_bundle PROPERTIES
		BUNDLE TRUE
		OUTPUT_NAME "${PROJECT_NAME}"
		LIBRARY_OUTPUT_DIRECTORY "../BuildProducts"
	)
endif()

if(APPLE OR UNIX)
 set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ../BuildProducts)
else()
 set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../BuildProducts)
endif()

target_include_directories(oLib PRIVATE ./inc/)
target_include_directories(oLib PRIVATE ${SRC_INCLUDE_DIR})
target_include_directories(oLib PRIVATE ../HeavyPdWrapper/)
target_include_directories(oLib PRIVATE ${CMAKE_BINARY_DIR}/HeavyPdWrapper/generated/)







