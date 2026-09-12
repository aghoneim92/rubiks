cmake_minimum_required(VERSION 3.16)
project(Cube CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

# ---------------------------------------------------------------------------
# Bake the GLSL sources into a header, so the program needs no data files at
# runtime (the web build has no filesystem to read them from).
# ---------------------------------------------------------------------------
set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
file(READ ${SHADER_DIR}/vshader.glsl VSHADER_SOURCE)
file(READ ${SHADER_DIR}/fshader.glsl FSHADER_SOURCE)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/embedded_shaders.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/embedded_shaders.h
    @ONLY)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    ${SHADER_DIR}/vshader.glsl
    ${SHADER_DIR}/fshader.glsl)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------
set(SOURCES
    src/animations.cpp
    src/circle.cpp
    src/cube.cpp
    src/cubelet.cpp
    src/drawable.cpp
    src/drawablegroup.cpp
    src/globalvariables.cpp
    src/initfunctions.cpp
    src/inputfunctions.cpp
    src/light_source.cpp
    src/main.cpp
    src/material.cpp
    src/picking.cpp
    src/rectangle.cpp
    src/renderer.cpp
    src/rubikscube.cpp
    src/shader.cpp)

if(EMSCRIPTEN)
    list(APPEND SOURCES src/platform_web.cpp)
else()
    list(APPEND SOURCES src/platform_glut.cpp)
endif()

add_executable(Cube ${SOURCES})
target_include_directories(Cube PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/generated)

# ---------------------------------------------------------------------------
# Platform wiring
# ---------------------------------------------------------------------------
if(EMSCRIPTEN)
    set_target_properties(Cube PROPERTIES OUTPUT_NAME index SUFFIX ".html")
    target_link_options(Cube PRIVATE
        -sMIN_WEBGL_VERSION=2
        -sMAX_WEBGL_VERSION=2
        -sALLOW_MEMORY_GROWTH=1
        -sEXIT_RUNTIME=0
        -sENVIRONMENT=web
        --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/web/shell.html)
    set_property(TARGET Cube APPEND PROPERTY LINK_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/web/shell.html)
elseif(APPLE)
    target_link_libraries(Cube PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework GLUT")
else()
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)
    find_package(GLEW REQUIRED)
    target_include_directories(Cube PRIVATE
        ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
    target_link_libraries(Cube PRIVATE
        ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
endif()
