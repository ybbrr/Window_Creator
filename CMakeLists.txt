cmake_minimum_required(VERSION 3.25)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

project(Window_Creator)

set(CMAKE_CXX_STANDARD 17)

if(EDITOR_BUILD)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /O2 /MD")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /O2 /MD /arch:AVX2")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /Od /MDd")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /Od /MDd /arch:AVX2")
endif ()

set(WORKSPACE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PARTY_FOLDER ${WORKSPACE_FOLDER}/__third_parties__)

if (STATIC_EXPORT)
    add_definitions(-D STATIC_EXPORT)
    set(LIBRARY_TYPE "STATIC")
endif ()

if (SHARED_EXPORT AND (NOT STATIC_EXPORT))
    add_definitions(-D SHARED_EXPORT)
    set(LIBRARY_TYPE "SHARED")
endif ()

# Project Includes
include_directories(${WORKSPACE_FOLDER}/include)
include_directories(${WORKSPACE_FOLDER}/include/interface)

# Third Party Includes
include_directories(${THIRD_PARTY_FOLDER}/glad/include)
include_directories(${THIRD_PARTY_FOLDER}/glfw/windows/include)

# GLAD
set(GLAD_SOURCE_FILE ${THIRD_PARTY_FOLDER}/glad/source/glad.c)

# GLFW
add_compile_definitions(GLFW_EXPOSE_NATIVE_WIN32)
link_directories(${THIRD_PARTY_FOLDER}/glfw/windows/lib/${CMAKE_BUILD_TYPE})
set(GLFW_TARGET
    glfw3.lib
    uxtheme
    dwmapi)

if (NOT BUILD_EXAMPLE)
    add_library(
        ${PROJECT_NAME}

        ${LIBRARY_TYPE}

        ${GLAD_SOURCE_FILE}
        ${WORKSPACE_FOLDER}/source/window.cpp
        ${WORKSPACE_FOLDER}/source/window_properties.cpp)
endif ()

if (BUILD_EXAMPLE)
    add_executable(
        ${PROJECT_NAME}

        ${GLAD_SOURCE_FILE}
        ${WORKSPACE_FOLDER}/source/example_main.cpp
        ${WORKSPACE_FOLDER}/source/window.cpp
        ${WORKSPACE_FOLDER}/source/window_properties.cpp)
endif ()

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE

    ${GLFW_TARGET}
    )

# end of file
