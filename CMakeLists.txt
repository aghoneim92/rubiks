cmake_minimum_required (VERSION 2.6)
project (Cube)

file(GLOB SRC "src/*.cpp")

add_executable(Cube ${SRC})

include_directories(include)
include_directories(src)
include_directories(/usr/local/include)

target_link_libraries(Cube
# -lstdc++ \
  "-framework Cocoa"
  "-framework CoreVideo"
  "-framework IOKit"
  "-framework OpenGL"
  "-framework Glut"
)

configure_file(shaders/vshader.glsl vshader.glsl COPYONLY)
configure_file(shaders/fshader.glsl fshader.glsl COPYONLY)
