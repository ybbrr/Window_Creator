cmake_minimum_required(VERSION 3.25)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

project(Window_Creator)

set(CMAKE_CXX_STANDARD 17)

if(EDITOR_BUILD)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
    set(CMAKE_INSTALL_PREFIX ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /O2 /MD")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /O2 /MD /arch:AVX2")
    endif ()

    if (UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native")
    endif ()
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /Od /MDd")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /Od /MDd /arch:AVX2")
    endif ()

    if (UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g -Wall -ggdb")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g -Wall -ggdb -march=native")
    endif ()
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

# Third Party Includes
include_directories(${THIRD_PARTY_FOLDER}/glad/include)
include_directories(${THIRD_PARTY_FOLDER}/glfw/windows/include)

# GLAD
set(GLAD_SOURCE_FILE ${THIRD_PARTY_FOLDER}/glad/source/glad.c)

if (MSVC)
    # GLFW
    add_compile_definitions(GLFW_EXPOSE_NATIVE_WIN32)
    link_directories(${THIRD_PARTY_FOLDER}/glfw/windows/lib/${CMAKE_BUILD_TYPE})
    set(GLFW_TARGET
        glfw3.lib
        uxtheme
        dwmapi)
endif ()

if (UNIX)
    # Third Party Includes
    include_directories(${THIRD_PARTY_FOLDER}/glfw/linux/include)
    include_directories(${THIRD_PARTY_FOLDER}/glm/linux)

    # GLFW
    set(GLFW_BUILD_WIN32 OFF)
    set(GLFW_LIBRARY_TYPE STATIC)
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_INSTALL OFF)
    add_subdirectory(${THIRD_PARTY_FOLDER}/glfw/linux)
    set(GLFW_TARGET glfw)
endif ()

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

    ${GLFW_TARGET})

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_PREFIX})

if(NOT BUILD_EXAMPLE AND NOT EDITOR_BUILD)
    set(OUTPUT_DIR ${CMAKE_INSTALL_PREFIX})
    set(FILE_TO_COPY ${CMAKE_SOURCE_DIR}/include/window.hpp)
    set(OUTPUT_FILE ${OUTPUT_DIR}/include/window.hpp)

    # Add a custom command to copy the directory
    add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${FILE_TO_COPY} ${OUTPUT_FILE}
    COMMENT "Copying file ${FILE_TO_COPY} to ${OUTPUT_FILE}")
endif()


# end of file
