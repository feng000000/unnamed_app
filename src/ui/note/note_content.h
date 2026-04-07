#ifndef __UI_NOTE_CONTENT_H__
#define __UI_NOTE_CONTENT_H__

#include <cstdint>
#include <memory>
#include <string>
#include <map>

#include "imgui.h"

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

};  // namespace ui::note

#endif
