#pragma once

#include <godot_cpp/variant/rect2i.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class WinAPI : public Object
{
	GDCLASS(WinAPI, Object);

	static WinAPI *singleton;

protected:
	static void _bind_methods();

public:
	static WinAPI *get_singleton();

	WinAPI();
	~WinAPI();

	uint64_t window_get_vscode_hwnd() const;
	void window_set_rect(uint64_t hwnd, Rect2i rect, uint64_t flags);
	uint64_t window_get_parent(uint64_t child) const;
	void window_set_parent(uint64_t child, uint64_t new_parent);
	void window_set_visible(uint64_t child, bool visible);
	void window_allow_resizing(uint64_t child, bool resizing);
};
