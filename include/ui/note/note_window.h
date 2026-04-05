#ifndef __UI_NOTE_WINDOW_H__
#define __UI_NOTE_WINDOW_H__

#include "imgui.h"
#include "ui/window_base.h"
#include "ui/note/note_content.h"

namespace ui::note
{
// the window that render the content node tree
class NoteWindow : public WindowBase
{
public:
    NoteWindow(
        const char* name,
        bool showing,
        std::shared_ptr<NoteContent> data
    );

    ~NoteWindow() = default;

    bool
    update(ImGuiID dock_node_id);

protected:
    std::shared_ptr<NoteContent> data;
};

};  // namespace ui::Note

#endif
