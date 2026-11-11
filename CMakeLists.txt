cmake_minimum_required(VERSION 3.21)
project(OpenStudio VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_path(NDI_INCLUDE_DIR 
    NAMES Processing.NDI.Lib.h
    PATHS 
        "$ENV{NDI_SDK_DIR}/Include"
        "C:/Program Files/NDI/NDI 6 SDK/Include"
        "C:/Program Files/NDI/NDI 5 SDK/Include"
)

# 2. Search the system for the NDI static library file (.lib)
find_library(NDI_LIBRARY 
    NAMES Processing.NDI.Lib.x64 Processing.NDI.Lib.x86
    PATHS 
        "$ENV{NDI_SDK_DIR}/Lib/x64"
        "$ENV{NDI_SDK_DIR}/Lib/x86"
        "C:/Program Files/NDI/NDI 6 SDK/Lib/x64"
        "C:/Program Files/NDI/NDI 5 SDK/Lib/x64"
)

if(NOT NDI_INCLUDE_DIR OR NOT NDI_LIBRARY)
    message(FATAL_ERROR "NDI SDK not found! Please download and install the NDI SDK, or set the NDI_SDK_DIR environment variable.")
endif()

# Replace the path below with your actual Qt6 installation path
list(APPEND CMAKE_PREFIX_PATH "C:/Qt/6.12.0/msvc2022_64/lib/cmake/Qt6")


find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)

add_executable(OpenStudio
    src/main.cpp
    src/engine/ProductionEngine.cpp
#    src/engine/sources/TestPatternSource.cpp
    src/ui/MainWindow.cpp
 "src/engine/sources/NDIVideoSource.h" "src/engine/sources/NDIVideoSource.cpp" "src/graphics/GraphicsDevice.h" "src/graphics/GraphicsDevice.cpp" "src/graphics/D3D11SwapChain.h" "src/graphics/D3D11SwapChain.cpp" "src/graphics/VideoSurface.h" "src/graphics/VideoSurface.cpp" "src/graphics/D3D11Texture.cpp" "src/graphics/D3D11Shader.h" "src/graphics/D3D11Shader.cpp" "src/graphics/D3D11VideoRenderer.h" "src/graphics/D3D11VideoRenderer.cpp" "src/engine/video/NDI/NDIMain.cpp")

#target_compile_definitions(OpenStudio PRIVATE NDILIB_DYNAMIC_LOAD)

add_custom_command(TARGET OpenStudio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:OpenStudio>/shaders"
)

set_property(TARGET OpenStudio PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:OpenStudio>"
)

target_include_directories(OpenStudio PRIVATE 
    src
    ${NDI_INCLUDE_DIR}
)

target_link_libraries(OpenStudio PRIVATE 
    Qt6::Widgets
    ${NDI_LIBRARY}
    delayimp

    d3d11
    dxgi
    d3dcompiler
)

if(MSVC)
    target_compile_options(OpenStudio PRIVATE /W4 /permissive-)
    target_link_options(OpenStudio PRIVATE "/DELAYLOAD:Processing.NDI.Lib.x64.dll")
else()
    target_compile_options(OpenStudio PRIVATE -Wall -Wextra -Wpedantic)
endif()
