#include "ui/window.h"

bool UI::Window::is_showing()
{
    return showing;
}
void UI::Window::hide()
{
    showing = false;
}
