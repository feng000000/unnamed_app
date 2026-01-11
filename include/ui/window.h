#ifndef __UI_WINDOW_H__
#define __UI_WINDOW_H__

#include "imgui.h"

namespace UI
{

class Window
{
public:
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
    Window(const char* name, bool showing)
        : name(name), showing(showing)
    {
    }

    virtual ~Window() = default;

    virtual bool show(ImGuiID dock_node_id) = 0;

    // void show();
    bool is_showing();
    void hide();

protected:
    const char* name;
    ImGuiID id = -1;
    bool showing;
};

}  // namespace UI

#endif