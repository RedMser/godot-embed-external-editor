@tool
extends EditorPlugin


var embed_manager
var toggle_button


func _enter_tree():
	var editor_interface: EditorInterface = get_editor_interface()
	var base_control: Control = editor_interface.get_base_control()
	var script_editor: ScriptEditor = editor_interface.get_script_editor()

	embed_manager = preload("embed_manager.gd").new()
	embed_manager.script_editor = script_editor
	add_child(embed_manager)

	# toggle button in scene tab bar
	toggle_button = preload("toggle_embed_button.gd").new()
	toggle_button.base_control = base_control

	toggle_button.toggled.connect(func(attached: bool):
		embed_manager.set_attach_enabled(attached)
	)

	script_editor.visibility_changed.connect(func():
		toggle_button.visible = script_editor.visible
	)
	toggle_button.visible = script_editor.visible

	var tabs_hbox = script_editor.get_node(^"../../../").get_child(0)
	tabs_hbox.add_child(toggle_button)


func _exit_tree():
	embed_manager.free()
	embed_manager = null

	toggle_button.free()
	toggle_button = null
