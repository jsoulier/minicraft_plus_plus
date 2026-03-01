extends Node

@export var mouse_sensitivity: float = 0.001
@export var camera_offset: Vector3 = Vector3.ZERO
@export var camera_smoothing: float = 0.1

@onready var _player: CharacterBody3D = $".."
@onready var _camera: Camera3D = $"../Camera3D"
@onready var _player_crosshair: ColorRect = $"../Control/PlayerCrosshair"
@onready var _camera_crosshair: ColorRect = $"../Control/CameraCrosshair"
@onready var _camera_raycast: RayCast3D = $"../Camera3D/RayCast3D"

func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_WINDOW_FOCUS_OUT:
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
	elif what == NOTIFICATION_APPLICATION_FOCUS_OUT:
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func _input(event: InputEvent) -> void:
	if Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
		if event is InputEventMouseMotion:
			_camera.rotate_y(-event.relative.x * mouse_sensitivity)
	if event is InputEventMouseButton:
		Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	elif event.is_action_pressed(&"unfocus"):
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func _process(_delta: float) -> void:
	if Input.is_action_pressed(&"shoot"):
		for child in _player.get_children():
			if not child is Weapon:
				continue
			child.fire()
	
func _physics_process(_delta: float) -> void:
	var movement_vector_2d = Input.get_vector("left", "right", "up", "down")
	var movement_vector = Vector3(movement_vector_2d.x, 0.0, movement_vector_2d.y).normalized()
	_player._velocity = _player.basis * movement_vector * _player.move_speed
	var target_direction = (_camera_raycast.get_collision_point() - _player.global_position).normalized()
	_player.face_target(target_direction)
	var target_camera_position = _player.global_position + _camera.global_basis * camera_offset 
	_camera.global_position = lerp(_camera.global_position, target_camera_position, camera_smoothing)
	var raycast_length = (_camera_raycast.get_collision_point() - _player.global_position).length()
	_player_crosshair.position = _camera.unproject_position(_player.global_position - _player.global_basis.z * raycast_length)
	_camera_crosshair.position = _camera.unproject_position(_camera.global_position - _camera.global_basis.z)
	_player_crosshair.position -= _player_crosshair.size / 2.0
	_camera_crosshair.position -= _camera_crosshair.size / 2.0
