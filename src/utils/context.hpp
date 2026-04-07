#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include <functional>
#include <stdbool.h>
#include <vector>

#include "spdlog/spdlog.h"

namespace utils::ctx
{
using TaskList = std::vector<std::function<void()>>;


// TODO: Context 设计有点烂
class Context
{
public:
    explicit Context(const char* name) : ctx_name(name)
    {
        spdlog::debug("[Context] Created: {}", ctx_name);
    }

    ~Context()
    {
        if (this->handle)
        {
            // 释放资源
            this->exit();
            spdlog::debug("[Context] Destroyed: {}", ctx_name);
        }
    }

    // 移动构造
    Context(Context&& other) noexcept : ctx_name(other.ctx_name)
    {
        this->handle = other.handle;
        other.handle = false;
        spdlog::debug("[Context] Moved: {}", ctx_name);
    }

    // 移动赋值
    Context&
    operator=(Context&& other) noexcept
    {
        if (this != &other)
        {
            this->handle = other.handle;
            other.handle = false;

            ctx_name = other.ctx_name;
            spdlog::debug("[Context] Moved: {}", ctx_name);
        }
        return *this;
    }

    // 禁用拷贝构造
    Context(const Context&) = delete;

    // 禁用拷贝赋值
    Context&
    operator=(const Context&) = delete;

    // 注册 退出上下文函数 FIFO
    void
    push_exit_func(std::function<void()> func)
    {
        spdlog::debug("register exit_func");
        exit_list.push_back(func);
    }

    // 注册 进入上下文函数, FIFO
    void
    push_enter_func(std::function<void()> func)
    {
        spdlog::debug("register enter_func");
        enter_list.push_back(func);
    }

    // 进入上下文
    void
    enter() noexcept
    {
        handle = true;
        spdlog::debug("[Context] enter context {}", ctx_name);
        execute_func_list("enter", enter_list);
    }

protected:
    // 退出上下文
    void
    exit() noexcept
    {
        this->handle = false;
        execute_func_list("exit", exit_list);
        spdlog::debug("[Context] exit context {}", ctx_name);
    }

private:
    bool handle = false;
    const char* ctx_name;
    TaskList enter_list;
    TaskList exit_list;

    void
    execute_func_list(const char* identify, TaskList& task_list) noexcept
    {
        for (auto& func : task_list)
        {
            if (!func)
                continue;
            try
            {
                func();
            }
            catch (const std::exception e)
            {
                spdlog::error(
                    "[Context] execute {} func failed: {}", identify, e.what()
                );
            }
            catch (...)
            {
                spdlog::error(
                    "[Context] enter {} func throw unknown "
                    "exception",
                    identify
                );
            }
        }
    }
};

};  // namespace utils::ctx

#endif
