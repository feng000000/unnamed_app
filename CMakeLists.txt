# >>>>>>>>>>>>>>>> Base Settings
# 生成 compile_commands.json 供 clangd 读取
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.16)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 20)
# DEBUG: set app name
set(PROJECT_NAME myapp)
project(${PROJECT_NAME} C CXX)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

# dependencies package
find_package(Vulkan REQUIRED)
# Base Settings <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> Main
add_executable(${PROJECT_NAME} src/Application.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
# Main <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> Project Headers
add_library(project_headers INTERFACE)
target_include_directories(project_headers INTERFACE
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/vendored/SDL/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE project_headers)
# Project Headers <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> SDL3
# - 裁剪 SDL3 模块
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)
set(SDL_AUDIO OFF CACHE BOOL "" FORCE)
set(SDL_JOYSTICK ON CACHE BOOL "" FORCE)
set(SDL_HAPTIC OFF CACHE BOOL "" FORCE)
set(SDL_SENSOR OFF CACHE BOOL "" FORCE)
set(SDL_HIDAPI OFF CACHE BOOL "" FORCE)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("[Release]: link static library SDL")
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
else()
    message(STATUS "[Debug/Other]: link dynamic library SDL")
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
endif()
# 确保 SDL 编译时包含 Vulkan 支持
set(SDL_VULKAN ON CACHE BOOL "" FORCE)
set(SDL_RENDER_VULKAN ON CACHE BOOL "" FORCE)
# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
# SDL3 <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> ImGui (depends SDL3)
add_library(imgui STATIC
# add_library(imgui SHARED
    vendored/imgui/imgui.cpp
    vendored/imgui/imgui_draw.cpp
    vendored/imgui/imgui_widgets.cpp
    vendored/imgui/imgui_tables.cpp
    # vendored/imgui/imgui_demo.cpp
    vendored/imgui/backends/imgui_impl_sdl3.cpp
    vendored/imgui/backends/imgui_impl_vulkan.cpp
)
target_include_directories(imgui PUBLIC
    "${CMAKE_SOURCE_DIR}/vendored/imgui"           # 核心头文件路径 (imgui.h)
    "${CMAKE_SOURCE_DIR}/vendored/imgui/backends"  # 后端头文件路径 (imgui_impl_sdl3.h)
)
target_link_libraries(imgui PRIVATE SDL3::SDL3 Vulkan::Vulkan)
target_link_libraries(imgui PUBLIC project_headers)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui Vulkan::Vulkan)
# ImGui <<<<<<<<<<<<<<<<




# >>>>>>>>>>>>>>>> Project Source (depends ImGui)
add_library(proj_source
    "src/ui/calendar/calendar.cpp"

    "src/ui/dock/dock_operation.cpp"

    "src/ui/note/note_content.cpp"
    "src/ui/note/note_window.cpp"

    "src/ui/render.cpp"

    "src/core/core_loop.cpp"

)
# 链接全局头文件
target_link_libraries(proj_source PRIVATE imgui)
target_link_libraries(proj_source PUBLIC project_headers)
target_link_libraries(${PROJECT_NAME} PRIVATE proj_source)
# Project Source <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> Compile Settings
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("[Release]: optimize binary file size")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

    # A. 开启 LTO (链接时优化)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT supported OUTPUT error)
    if(supported)
        set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
        message(STATUS "  [优化] LTO 开启成功")
    else()
        message(WARNING "  [警告] 当前编译器不支持 LTO: ${error}")
    endif()

    # B. 强制去除符号表 (Strip)
    if(UNIX)
        # 通过链接器参数 -s 在生成阶段直接剥离符号
        target_link_options(${PROJECT_NAME} PRIVATE -s)
        message(STATUS "  [优化] 已添加链接器 Strip 参数 (-s)")
    endif()
endif()
# Compile Settings <<<<<<<<<<<<<<<<
