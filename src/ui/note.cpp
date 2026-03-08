#include <cmath>
#include <memory>
#include <sys/types.h>
#include <utility>

#include "imgui.h"
#include "spdlog/spdlog.h"

#include "ui/manager.h"
#include "ui/note.h"
#include "ui/utils.hpp"

using namespace ui::note;
using namespace ui::utils;

NoteContent::NoteContent(std::string data, ImGuiItemFlags flags)
{
    this->id = gen_id();
    this->data = std::move(data);
    this->flags = flags;
}

std::weak_ptr<NoteContent>
NoteContent::find_ancestor(
    NoteContentID ancestor_id, bool recursion
)
{
    if (this->parent.expired())
        return {};

    auto _parent = this->parent.lock();

    if (_parent->id == ancestor_id)
        return this->parent;

    return _parent->find_ancestor(ancestor_id, recursion);
}

std::weak_ptr<NoteContent>
NoteContent::find_child(NoteContentID id, bool recursion) const
{
    // find in children
    if (auto it = this->children.find(id);
        it != this->children.end())
    {
        if (!recursion)
            return it->second;
    }

    // no recursion, just return {}
    if (!recursion)
        return {};

    // recursive search
    for (auto& [_, item] : this->children)
        if (auto res = item->find_child(id, recursion);
            !res.expired())
            return res;

    return {};
}

bool
NoteContent::add_child(const std::shared_ptr<NoteContent>& child)
{
    if (!this->find_child(child->id, true).expired())
        return false;

    child->parent = this->shared_from_this();
    this->children.insert({child->id, std::move(child)});

    return true;
}

std::shared_ptr<NoteContent>
NoteContent::remove_child(NoteContentID id, bool recursion)
{
    auto node_handler = this->children.extract(id);
    if (!node_handler.empty())
    {
        auto child = std::move(node_handler.mapped());
        if (child)
            child->parent.reset();
        return std::move(child);
    }

    if (recursion)
    {
        for (auto& [_, child] : this->children)
            if (auto res = child->remove_child(id, true); res)
                return res;
    }
    return {};
}

bool
NoteContent::move_child(
    NoteContentID child_id,
    const std::shared_ptr<NoteContent>& parent,
    bool recursion
)
{

    auto ancestor = this->find_ancestor(child_id, true);
    if (!ancestor.expired())
        return false;

    auto child = this->remove_child(child_id, recursion);
    parent->add_child(std::move(child));
    return true;
}

bool
NoteWindow::update(ImGuiID dock_node_id)
{
    if (!this->showing)
        return false;

    if (ui::FIRST_TIME)
    {
    }

    if (ImGui::Begin(name, &(this->showing), flags))
    {
        id = ImGui::GetID("");

        ImGui::Text("this is Note Window");

        if (ImGui::Button("note window button"))
            spdlog::debug("click note window button");

        ImGui::End();

        return true;
    }
    return false;
}

NoteWindow::NoteWindow(
    const char* name,
    bool showing,
    std::shared_ptr<NoteContent> data
)
    : Window(name, showing), data(std::move(data))
{
}