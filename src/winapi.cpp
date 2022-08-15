#include "winapi.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <winuser.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

WinAPI *WinAPI::singleton = nullptr;

void WinAPI::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("window_get_parent", "child"), &WinAPI::window_get_parent);
	ClassDB::bind_method(D_METHOD("window_get_vscode_hwnd"), &WinAPI::window_get_vscode_hwnd);
	ClassDB::bind_method(D_METHOD("window_set_rect", "hwnd", "rect", "flags"), &WinAPI::window_set_rect);
	ClassDB::bind_method(D_METHOD("window_set_parent", "child", "new_parent"), &WinAPI::window_set_parent);
	ClassDB::bind_method(D_METHOD("window_set_visible", "window", "visible"), &WinAPI::window_set_visible);
	ClassDB::bind_method(D_METHOD("window_allow_resizing", "window", "resizing"), &WinAPI::window_allow_resizing);
}

WinAPI *WinAPI::get_singleton()
{
	return singleton;
}

WinAPI::WinAPI()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

WinAPI::~WinAPI()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

BOOL CALLBACK EnumExternalEditorCallback(HWND hwnd, LPARAM lparam) {
	SetLastError(0);
	int title_length = GetWindowTextLengthW(hwnd);
	if (title_length == 0) {
		if (GetLastError() != 0) {
			ERR_FAIL_V_MSG(TRUE, "Unable to retrieve window title length.");
		} else {
			return TRUE;
		}
	}

	// Trailing null byte
	title_length++;
	WCHAR *title_raw = (WCHAR *)memalloc(title_length * sizeof(WCHAR));
	title_length = GetWindowTextW(hwnd, title_raw, title_length);
	if (title_length == 0) {
		memfree(title_raw);
		if (GetLastError() != 0) {
			ERR_FAIL_V_MSG(TRUE, "Unable to retrieve window title length.");
		} else {
			return TRUE;
		}
	}
	String title = String::utf16((const char16_t *)title_raw);
	memfree(title_raw);

	ProjectSettings *ps = ProjectSettings::get_singleton();
	String res_path = /* ps->get_resource_path() */ ps->globalize_path("res://");
	if (res_path.is_empty()) {
		ERR_FAIL_V_MSG(TRUE, "res:// could not be resolved to a path.");
	}
	PackedStringArray res_segments = res_path.split("/", false);
	String res_name = res_segments[res_segments.size() - 1];
	// TODO: The spaces are supposed to help with false positives, but if you only have a folder open and no file then the first space causes detection to fail
	if (title.contains("Visual Studio Code") && title.contains(" " + res_name + " ")) {
		*(HWND *)lparam = hwnd;
		return FALSE;
	}

	return TRUE;
}

uint64_t WinAPI::window_get_vscode_hwnd() const {
	HWND editor = nullptr;
	// This only iterates over top-level windows, so already-captured editor is not included.

	// TODO: change lparam to a struct that has
	//	- output hwnd like right now
	//	- input resource path, so it's not computed on every callback
	//  - addon config (such as the currently hardcoded title search string)
	EnumWindows(EnumExternalEditorCallback, (LONG_PTR)&editor);
	return (uint64_t)editor;
}

void WinAPI::window_set_rect(uint64_t hwnd, Rect2i rect, uint64_t flags) {
	SetWindowPos((HWND)hwnd, (HWND)0, rect.position.x, rect.position.y, rect.size.x, rect.size.y, flags);
}

uint64_t WinAPI::window_get_parent(uint64_t child) const {
	return (uint64_t)GetParent((HWND)child);
}

void WinAPI::window_set_parent(uint64_t child, uint64_t new_parent) {
	SetParent((HWND)child, (HWND)new_parent);
}

void WinAPI::window_set_visible(uint64_t child, bool visible) {
	ShowWindowAsync((HWND)child, visible ? SW_SHOWNORMAL : SW_HIDE);
}

void WinAPI::window_allow_resizing(uint64_t child, bool resizing) {
	LONG current_style = GetWindowLongW((HWND)child, GWL_STYLE);
	// TODO: Remember old window long (e.g. native vs custom titleBarStyle of vscode)
	// TODO: When resizing=true, it does not refresh the window rendering until you minimize and restore
	SetWindowLongW((HWND)child, GWL_STYLE, resizing ? (current_style | WS_SIZEBOX | WS_CAPTION) : (current_style & ~(WS_SIZEBOX | WS_CAPTION)));
}
