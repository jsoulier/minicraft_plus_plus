class_name Bot extends CharacterBody3D

@export var move_speed: float = 5.0
@export var rotate_speed: float = 0.5
@export var max_health: float = 100
var _health: float = 0.0
var _total_impulse: Vector3 = Vector3.ZERO
var _velocity: Vector3 = Vector3.ZERO
var _target_direction: Vector3 = Vector3.ZERO

func _ready():
	_health = max_health
	on_damage(0.0, Vector3.ZERO)

func _set_material(parent):
	for child in parent.get_children():
		if child.get_child_count() > 0:
			_set_material(child)
		if not child is MeshInstance3D:
			continue
		var mesh_instance = child
		mesh_instance.set_instance_shader_parameter("health", _health)
		mesh_instance.set_instance_shader_parameter("max_health", max_health)

func on_damage(damage: float, impulse: Vector3) -> void:
	_health -= damage
	_total_impulse += impulse
	if _health <= 0.0:
		queue_free()
		return
	_set_material(self)

func _physics_process(delta: float) -> void:
	velocity = _velocity + _total_impulse
	_velocity = Vector3.ZERO
	_total_impulse = Vector3.ZERO
	move_and_slide()
	var current_direction = -global_basis.z
	var delta_rotation = current_direction.angle_to(_target_direction)
	if delta_rotation > 0.001:
		var cross = current_direction.cross(_target_direction)
		rotate_y(signf(cross.y) * min(delta_rotation, rotate_speed * delta))

func face_target(target_direction: Vector3) -> void:
	_target_direction = target_direction
