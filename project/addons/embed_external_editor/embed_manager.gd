@tool
extends Node


signal embed_attached
signal embed_size_changed(rect: Rect2i)
signal embed_visible_changed(visible: bool)


const Embedded = preload("embedded.gd")
var embedded = Embedded.new()
var timer: Timer
var script_editor: ScriptEditor


func _enter_tree():
	timer = Timer.new()
	timer.autostart = true
	timer.wait_time = 1.0
	timer.timeout.connect(_check_embed)
	add_child(timer)

	script_editor.resized.connect(update_size)
	script_editor.visibility_changed.connect(update_visible)

	_check_embed()


func set_attach_enabled(enabled: bool):
	if enabled:
		_check_embed()
	else:
		timer.stop()
		embedded.detach()
		embedded.hwnd = 0


func _check_embed():
	var new_hwnd = WinAPI.window_get_vscode_hwnd()
	if new_hwnd == embedded.hwnd or new_hwnd == 0:
		# no new window found, or still same window id as before
		return

	embedded.detach()
	embedded.hwnd = new_hwnd
	embedded.attach()
	timer.stop()
	update_visible()
	update_size()
	embed_attached.emit()


func update_size():
	var new_rect = script_editor.get_global_rect()
	var size_changed = embedded.set_size(new_rect)
	if size_changed:
		embed_size_changed.emit(new_rect)


func update_visible():
	var new_visible = script_editor.visible
	var visibility_changed = embedded.set_visible(new_visible)
	if visibility_changed:
		embed_visible_changed.emit(new_visible)


func _exit_tree():
	script_editor.resized.disconnect(update_size)
	script_editor.visibility_changed.disconnect(update_visible)
	script_editor = null

	embedded.detach()
	embedded.free()
	embedded = null
