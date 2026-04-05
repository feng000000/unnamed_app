#ifndef __UI_WINDOW_H__
#define __UI_WINDOW_H__

#include "imgui.h"

namespace ui
{


class WindowBase
{
public:
    const char* name;
    ImGuiID id = -1;
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;

    WindowBase(const char* name, bool showing)
        : name(name), showing(showing)
    {
    }

    virtual ~WindowBase() = default;

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

}  // namespace ui

#endif
