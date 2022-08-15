@tool
extends Button


var base_control: Control


func _ready():
	flat = true
	hint_tooltip = "Attach/Detach External Editor"
	toggle_mode = true
	button_pressed = true
	toggled.connect(_on_toggle)
	update_icon(true)


func _on_toggle(attached: bool):
	update_icon(attached)


func update_icon(enabled: bool):
	if enabled:
		icon = base_control.get_theme_icon("Instance", "EditorIcons")
	else:
		icon = base_control.get_theme_icon("Unlinked", "EditorIcons")
