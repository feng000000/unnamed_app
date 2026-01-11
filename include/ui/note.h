#ifndef __UI_NOTE_H__
#define __UI_NOTE_H__

#include "ui/window.h"

namespace UI::Note
{

// TODO: implement NoteContent
class NoteContent
{
};

class NoteWindow : public Window
{
public:
    NoteWindow(const char* name, bool showing, NoteContent data)
        : Window(name, showing), data(data)
    {
    }
    ~NoteWindow() = default;
    bool show(ImGuiID dock_node_id);

private:
    NoteContent data;
};

};  // namespace UI::Note

#endif
