cmake_minimum_required(VERSION 3.27)
project(MTE_391_FinalProject)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)

add_executable(MTE_391_FinalProject
        main.cpp
        BallObjectHandler.cpp
        Camera.cpp
        CollisionHandler.cpp
        GLFW_Handler.cpp
        MatrixHandler.cpp
        Setters_Getters.cpp
        ShaderManager.cpp
        TextureManager.cpp
        WallObjectHandler.cpp
)

target_link_libraries(
        MTE_391_FinalProject PRIVATE
        ./SDL/lib/x86/SDL2.lib
        ./SDL2_image-2.8.1/lib/x64/SDL2_image.lib
        ./glew/lib/Release/x64/glew32.lib

)
target_include_directories(
        MTE_391_FinalProject PRIVATE
        ./glew/include
        ./glfw-3.3.9/deps/glad
        ./SDL/include
        ./SDL2_image-2.8.1/include
        ./glfw-3.3.9/include
        ./glm-master/glm
)
