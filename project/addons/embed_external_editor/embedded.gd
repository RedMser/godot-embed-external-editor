@tool
extends Object


var is_attached = false
var last_visible = true
var last_size = Rect2i()
var hwnd = 0
var original_parent_hwnd = 0


func attach():
	if hwnd == 0 or is_attached:
		return
	original_parent_hwnd = WinAPI.window_get_parent(hwnd)
	WinAPI.window_set_parent(hwnd, DisplayServer.window_get_native_handle(DisplayServer.WINDOW_HANDLE))
	WinAPI.window_allow_resizing(hwnd, false)
	is_attached = true


func detach():
	if hwnd == 0 or not is_attached:
		return
	if original_parent_hwnd == 0:
		# fallback to desktop hwnd
		original_parent_hwnd = 0x00010010
	WinAPI.window_set_parent(hwnd, original_parent_hwnd)
	WinAPI.window_allow_resizing(hwnd, true)
	original_parent_hwnd = 0
	is_attached = false
	last_visible = true
	last_size = Rect2i()


func set_size(rect: Rect2i, activate = false) -> bool:
	if hwnd == 0:
		return false
	if last_size == rect:
		return false

	var flags = 0
	if not activate:
		flags = 0x0010 # SWP_NOACTIVATE
	WinAPI.window_set_rect(hwnd, rect, flags)
	last_size = rect
	return true


func set_visible(new_visible: bool) -> bool:
	if hwnd == 0:
		return false
	if last_visible == new_visible:
		return false

	WinAPI.window_set_visible(hwnd, new_visible)
	last_visible = new_visible
	return true
