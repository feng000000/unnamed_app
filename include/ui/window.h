#ifndef __UI_WINDOW_H__
#define __UI_WINDOW_H__

#include "imgui.h"

namespace UI
{


class Window
{
public:
    const char* name;
    ImGuiID id = -1;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;

    Window(const char* name, bool showing)
        : name(name), showing(showing)
    {
    }

    virtual ~Window() = default;

    virtual bool update(ImGuiID dock_node_id) = 0;

    void show()
    {
        showing = true;
    }
    bool is_showing()
    {
        return showing;
    }
    void hide()
    {
        showing = false;
    }

protected:
    bool showing;
};

}  // namespace UI

#endif