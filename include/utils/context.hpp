#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include <functional>
#include <stdbool.h>

#include "spdlog/spdlog.h"

namespace utils::ctx
{
using TaskStack = std::vector<std::function<void()>>;

class Context
{
public:
    // 1. 构造函数：获取资源
    explicit Context(const char* name) : ctx_name(name)
    {
        spdlog::debug("[Context] Created: {}", ctx_name);
    }

    // 2. 析构函数：RAII 释放资源
    ~Context()
    {
        if (this->handle)
        {
            spdlog::debug("[Context] Destroyed: {}", ctx_name);
            this->exit();
        }
    }

    // 移动构造
    Context(Context&& other) noexcept : ctx_name(other.ctx_name)
    {
        other.handle = false;
        this->handle = true;
        spdlog::debug("[Context] Moved: {}", ctx_name);
    }

    // 移动赋值
    Context&
    operator=(Context&& other) noexcept
    {
        if (this != &other)
        {
            other.handle = false;
            this->handle = true;

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

    // 注册 退出上下文函数
    void
    push_exit_func(std::function<void()> func)
    {
        spdlog::debug("register exit_func");
        exit_stack.push_back(func);
    }

    // 注册 进入上下文函数
    void
    push_enter_func(std::function<void()> func)
    {
        spdlog::debug("register enter_func");
        enter_stack.push_back(func);
    }

    // 进入上下文
    void
    enter() noexcept
    {
        spdlog::debug("[Context] enter context {}", ctx_name);
        execute_func_stack("enter", enter_stack);
    }

protected:
    // 退出上下文
    void
    exit() noexcept
    {
        execute_func_stack("exit", exit_stack);
        this->handle = false;
    }

private:
    bool handle = false;
    const char* ctx_name;
    TaskStack enter_stack;
    TaskStack exit_stack;

    void
    execute_func_stack(
        const char* identify, TaskStack& stack
    ) noexcept
    {
        while (!stack.empty())
        {
            if (!stack.back())
                continue;
            try
            {
                stack.back()();
                stack.pop_back();
            }
            catch (const std::exception e)
            {
                spdlog::error(
                    "[Context] execute {} func failed: {}",
                    identify,
                    e.what()
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
