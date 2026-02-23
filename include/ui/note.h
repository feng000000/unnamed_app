#ifndef __UI_NOTE_H__
#define __UI_NOTE_H__

#include <cstdint>
#include <memory>
#include <string>
#include <map>

#include "imgui.h"
#include "ui/window.h"

namespace ui::note
{

// content block type
enum class NoteContentType
{
    Paragraph,
    Header1,
    Header2,
    Header3,
    Header4,
    Separator,
    Bullet,
    Ordered,
};

using NoteContentID = uint64_t;

// content block
class NoteContent
    : public std::enable_shared_from_this<NoteContent>
{
public:
    NoteContentID id;
    std::string data;
    ImGuiItemFlags flags;
    NoteContentType type;
    std::weak_ptr<NoteContent> parent;
    std::map<NoteContentID, std::shared_ptr<NoteContent>>
        children;

    NoteContent(std::string data, ImGuiItemFlags flags);

    std::weak_ptr<NoteContent>
    find_ancestor(NoteContentID id, bool recursion);

    std::weak_ptr<NoteContent>
    find_child(NoteContentID id, bool recursion) const;

    bool
    add_child(const std::shared_ptr<NoteContent>& child);

    std::shared_ptr<NoteContent>
    remove_child(NoteContentID id, bool recursion);

    bool
    move_child(
        NoteContentID id,
        const std::shared_ptr<NoteContent>& parent,
        bool recursion
    );
};

// the window that render the content node tree
class NoteWindow : public Window
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
