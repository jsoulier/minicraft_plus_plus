class_name Bot extends CharacterBody3D

@export var move_speed: float = 5.0
@export var rotate_speed: float = 0.5
@export var max_health: float = 100
var _health: float = 0.0

func _ready():
	_health = max_health
	on_damage(0.0)

func _set_material(parent):
	for child in parent.get_children():
		if child.get_child_count() > 0:
			_set_material(child)
		if not child is MeshInstance3D:
			continue
		var mesh_instance = child
		mesh_instance.set_instance_shader_parameter("health", _health)
		mesh_instance.set_instance_shader_parameter("max_health", max_health)

func on_damage(damage: float) -> void:
	_health -= damage
	if _health <= 0.0:
		queue_free()
		return
	_set_material(self)
