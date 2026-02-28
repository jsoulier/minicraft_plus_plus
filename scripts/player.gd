extends Node

@export var rotate_speed: float = 0.001
@export var camera_offset: Vector3 = Vector3.ZERO

@onready var _player: CharacterBody3D = $".."
@onready var _camera: Camera3D = $"../Camera3D"

func _notification(what: int) -> void:
	if what == NOTIFICATION_WM_WINDOW_FOCUS_OUT:
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)
	elif what == NOTIFICATION_APPLICATION_FOCUS_OUT:
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func _input(event: InputEvent) -> void:
	if Input.get_mouse_mode() == Input.MOUSE_MODE_CAPTURED:
		if event is InputEventMouseMotion:
			_camera.rotate_y(-event.relative.x * rotate_speed)
	if event is InputEventMouseButton:
		Input.set_mouse_mode(Input.MOUSE_MODE_CAPTURED)
	elif event.is_action_pressed(&"unfocus"):
		Input.set_mouse_mode(Input.MOUSE_MODE_VISIBLE)

func get_movement_vector(_bot: Bot) -> Vector2:
	return Input.get_vector("left", "right", "up", "down")

func _physics_process(_delta: float) -> void:
	var player_quat = _player.global_basis.get_rotation_quaternion()
	var camera_quat = _camera.global_basis.get_rotation_quaternion()
	var angle_to_camera = player_quat.angle_to(camera_quat)
	if angle_to_camera > 0.01:
		var frame_rotate_speed = _player.rotate_speed
		var alpha = min(frame_rotate_speed / angle_to_camera, 1.0)
		player_quat = player_quat.slerp(camera_quat, alpha)
		_player.global_transform.basis = Basis(player_quat)
	var movement_vector_2d = Input.get_vector("left", "right", "up", "down")
	var movement_vector = Vector3(movement_vector_2d.x, 0.0, movement_vector_2d.y).normalized()
	_player.velocity = _player.basis * movement_vector * _player.move_speed
	_player.move_and_slide()
	_camera.global_position = _player.global_position + _player.global_basis * camera_offset
