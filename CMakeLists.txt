cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)

project(Game)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(
    /usr/include/GL
    /usr/include/GLFW
    /usr/include/glm
)

# Add library directories (Optional, CMake usually finds these automatically)
link_directories(
    /usr/lib/x86_64-linux-gnu
    /usr/local/lib
)
  
file(GLOB IMAGE_FILES "${CMAKE_SOURCE_DIR}/Game/*.png")

# Loop through each file and copy it to the build directory
configure_file(${CMAKE_SOURCE_DIR}/container.png ${CMAKE_BINARY_DIR}/container.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/background.png ${CMAKE_BINARY_DIR}/background.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/bullet.png ${CMAKE_BINARY_DIR}/bullet.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/ending.png ${CMAKE_BINARY_DIR}/ending.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/starting.png ${CMAKE_BINARY_DIR}/starting.png COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/Gray2.png ${CMAKE_BINARY_DIR}/Gray2.png COPYONLY)


configure_file(${CMAKE_SOURCE_DIR}/shader.vs ${CMAKE_BINARY_DIR}/shader.vs COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shader.fs ${CMAKE_BINARY_DIR}/shader.fs COPYONLY)
# Manually set GLFW path if find_package fails
set(GLFW_DIR /usr/include/GLFW)

# Add executable
add_executable(cpp 
    glad.c 
    main.cpp 
    Bullet.h 
    main.h 
    std_img.h 
    Texture.h 
    Texture.cpp
    VertexArray.cpp 
    VertexArray.h 
    VertexBuffer.cpp 
    VertexBuffer.h 
    VertexBufferLayout.h
    VertexBufferLayout.cpp
    h.h
)

# Link OpenGL and GLFW manually
target_link_libraries(cpp 
/usr/lib/x86_64-linux-gnu/libGL.so
/usr/lib/x86_64-linux-gnu/libglfw.so
/usr/lib/x86_64-linux-gnu/libGLEW.so

)
