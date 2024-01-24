cmake_minimum_required(VERSION 3.24)
project(final2)

set(CMAKE_CXX_STANDARD 17)

# Find the required packages
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2_image REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} )
link_libraries(${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} )

add_executable(final2 mte391_final/main.cpp mte391_final/Camera.cpp mte391_final/Setters_Getters.cpp mte391_final/MatrixHandler.cpp mte391_final/CollisionHandler.cpp mte391_final/WallObjectHandler.cpp mte391_final/ShaderManager.cpp mte391_final/BallObjectHandler.cpp mte391_final/TextureManager.cpp mte391_final/GLFW_Handler.cpp mte391_final/GLFW_Handler.h)

target_link_libraries(
        final2 PRIVATE
        /Library/Frameworks/SDL2.framework/Versions/A/SDL2
        /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image
        /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib
        /usr/local/Cellar/glfw-3.3.9.bin.MACOS/lib-universal/libglfw.3.dylib
)
target_include_directories(
        final2 PRIVATE
        /usr/local/Cellar/glew/2.2.0_1/include
        /usr/local/Cellar/glm/0.9.9.8/include
        /Library/Frameworks/SDL2.framework/Versions/A/Headers
        /Library/Frameworks/SDL2_image.framework/Versions/A/Headers
        /usr/local/Cellar/glfw-3.3.9.bin.MACOS/include
        /usr/local/Cellar/glad/include
)