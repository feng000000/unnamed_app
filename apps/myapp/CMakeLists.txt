# >>>>>>>>>>>>>>>> Base Settings
# 生成 compile_commands.json 供 clangd 读取
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.16)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into
# the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_CXX_STANDARD 20)
# NOTICE: set app name
set(PROJECT_NAME myapp)
project(${PROJECT_NAME} C CXX)

# NOTICE: 固定项目根目录路径 ${PROJECT_ROOT_PATH}
set(PROJECT_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../..)
message("PROJECT_ROOT_PATH: "${PROJECT_ROOT_PATH})

# NOTICE: 调试 / 生产
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

# dependencies package
find_package(Vulkan REQUIRED)
# Base Settings <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> Main
add_executable(${PROJECT_NAME} Application.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
# Main <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> Project Headers
add_library(project_headers INTERFACE)
target_include_directories(project_headers INTERFACE
# ${PROJECT_ROOT_PATH}/include
${PROJECT_ROOT_PATH}/src
)
target_link_libraries(${PROJECT_NAME} PRIVATE project_headers)
# Project Headers <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> spdlog (Header-only)
add_library(spdlog INTERFACE)
target_include_directories(spdlog INTERFACE
${PROJECT_ROOT_PATH}/vendored/spdlog
)
# target_compile_definitions(spdlog INTERFACE SPDLOG_COMPILED_LIB)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
# >>>>>>>>>>>>>>>>

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
add_subdirectory(
    ${PROJECT_ROOT_PATH}/vendored/SDL
    ${CMAKE_CURRENT_BINARY_DIR}/libs/example
    EXCLUDE_FROM_ALL
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
# SDL3 <<<<<<<<<<<<<<<<


# >>>>>>>>>>>>>>>> ImGui (depends SDL3)
add_library(imgui STATIC
# add_library(imgui SHARED
    ${PROJECT_ROOT_PATH}/vendored/imgui/imgui.cpp
    ${PROJECT_ROOT_PATH}/vendored/imgui/imgui_draw.cpp
    ${PROJECT_ROOT_PATH}/vendored/imgui/imgui_widgets.cpp
    ${PROJECT_ROOT_PATH}/vendored/imgui/imgui_tables.cpp
    # ${PROJECT_ROOT_PATH}/vendored/imgui/imgui_demo.cpp
    ${PROJECT_ROOT_PATH}/vendored/imgui/backends/imgui_impl_sdl3.cpp
    ${PROJECT_ROOT_PATH}/vendored/imgui/backends/imgui_impl_vulkan.cpp
)
target_include_directories(imgui PUBLIC
    ${PROJECT_ROOT_PATH}/vendored/imgui           # 核心头文件路径 (imgui.h)
    ${PROJECT_ROOT_PATH}/vendored/imgui/backends  # 后端头文件路径 (imgui_impl_sdl3.h)
)
target_link_libraries(imgui PRIVATE SDL3::SDL3 Vulkan::Vulkan)
# target_link_libraries(imgui PUBLIC project_headers)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui Vulkan::Vulkan)
# ImGui <<<<<<<<<<<<<<<<



# >>>>>>>>>>>>>>>> Project Source (depends ImGui)
add_library(proj_source
    ${PROJECT_ROOT_PATH}/src/ui/calendar/calendar.cpp

    ${PROJECT_ROOT_PATH}/src/ui/dock/dock_operation.cpp

    ${PROJECT_ROOT_PATH}/src/ui/note/note_content.cpp
    ${PROJECT_ROOT_PATH}/src/ui/note/note_window.cpp

    ${PROJECT_ROOT_PATH}/src/ui/menu/menu.cpp

    ${PROJECT_ROOT_PATH}/src/ui/render.cpp

    ${PROJECT_ROOT_PATH}/src/core/core_loop.cpp
    ${PROJECT_ROOT_PATH}/src/core/calendar/calendar_datetime.cpp
)
# 链接头文件
target_link_libraries(proj_source PRIVATE imgui)
target_link_libraries(proj_source PUBLIC spdlog)
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
