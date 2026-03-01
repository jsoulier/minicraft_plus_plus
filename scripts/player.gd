extends Node

@export var rotate_speed: float = 0.001
@export var camera_offset: Vector3 = Vector3.ZERO
@export var crosshair_length: float = 10.0

@onready var _player: CharacterBody3D = $".."
@onready var _camera: Camera3D = $"../Camera3D"
@onready var _player_crosshair: ColorRect = $"../Control/PlayerCrosshair"
@onready var _camera_crosshair: ColorRect = $"../Control/CameraCrosshair"

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
		
func _process(_delta: float) -> void:
	if Input.is_action_pressed(&"shoot"):
		for child in _player.get_children():
			if not child is Weapon:
				continue
			var weapon: Weapon = child
			weapon.fire()
	
func _get_screen_position(node: Node3D) -> Vector2:
	return _camera.unproject_position(node.global_position + -node.global_basis.z * crosshair_length)

func _physics_process(delta: float) -> void:
	var movement_vector_2d = Input.get_vector("left", "right", "up", "down")
	var movement_vector = Vector3(movement_vector_2d.x, 0.0, movement_vector_2d.y).normalized()
	_player.velocity = _player.basis * movement_vector * _player.move_speed
	var frame_rotate_speed = _player.rotate_speed * delta
	# TODO: we don't want to align with camera rotation. it should factor camera_offset into account
	var delta_angle = wrapf(_camera.rotation.y - _player.rotation.y, -PI, PI)
	_player.rotation.y += clamp(delta_angle, -frame_rotate_speed, frame_rotate_speed)
	_player.move_and_slide()
	_camera.global_position = _player.global_position + _player.global_basis * camera_offset
	_player_crosshair.position = _get_screen_position(_player)
	_camera_crosshair.position = _get_screen_position(_camera)
