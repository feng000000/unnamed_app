#include <iostream>
#include <SDL3/SDL.h>

int main()
{
    // 1. 初始化 SDL 环境
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL 初始化失败: %s", SDL_GetError());
        return -1;
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "vulkan");

    // 2. 同时创建窗口和渲染器
    // SDL_CreateWindowAndRenderer 会自动处理后端选择
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer(
            "Hello SDL3", 800, 600, 0, &window, &renderer
        ))
    {
        SDL_Log("窗口或渲染器创建失败: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // 直接获取并打印后端名称
    const char* backend_name = SDL_GetRendererName(renderer);
    if (backend_name)
    {
        std::cout << "========================================"
                  << std::endl;
        std::cout << "当前激活的渲染后端: " << backend_name
                  << std::endl;
        std::cout << "========================================"
                  << std::endl;
    }
    else
    {
        std::cout << "无法获取渲染器名称" << std::endl;
    }

    // 3. 主循环
    bool quit = false;
    SDL_Event event;
    while (!quit)
    {
        // 处理事件
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }

        // 渲染逻辑
        SDL_SetRenderDrawColor(
            renderer, 100, 149, 237, 255
        );  // 设置背景色 (Cornflower Blue)
        SDL_RenderClear(renderer);    // 清屏
        SDL_RenderPresent(renderer);  // 提交显示
    }

    // 4. 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    std::cout << "Done" << std::endl;
    return 0;
}