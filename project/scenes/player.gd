extends Sprite2D


func _process(delta):
	position += Vector2.ONE * randf_range(-5, 5) * delta
