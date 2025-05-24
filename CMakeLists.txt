cmake_minimum_required (VERSION 2.8)
project (Cube)

file(GLOB SRC "src/*.cpp")

add_executable(Cube ${SRC})

include_directories(include)
include_directories(src)
include_directories(/usr/local/include)

if(APPLE)
    target_link_libraries(Cube
        "-framework Cocoa"
        "-framework CoreVideo"
        "-framework IOKit"
        "-framework OpenGL"
        "-framework GLUT"
    )
else()
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)
    find_package(GLEW REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
    target_link_libraries(Cube ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES})
endif()

configure_file(shaders/vshader.glsl vshader.glsl COPYONLY)
configure_file(shaders/fshader.glsl fshader.glsl COPYONLY)
