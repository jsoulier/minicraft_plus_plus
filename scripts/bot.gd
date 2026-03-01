class_name Bot extends CharacterBody3D

static var BOT_PHYSICS_LAYER = 1 << 0
static var WORLD_PHYSICS_LAYER = 1 << 2

@export var move_speed: float = 5.0
@export var rotate_speed: float = 0.5
@export var max_health: float = 100
@onready var _navigation_agent: NavigationAgent3D = get_node_or_null("NavigationAgent3D")
var _health: float = 0.0
var _total_impulse: Vector3 = Vector3.ZERO
var _move_direction: Vector3 = Vector3.ZERO
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

func shoot() -> void:
	for child in get_children():
		if not child is Weapon:
			continue
		child.fire()

func get_facing() -> Vector3:
	return -global_basis.z

func get_angle_to_target_facing() -> float:
	return get_facing().angle_to(_target_direction)

func is_facing_target() -> bool:
	return get_angle_to_target_facing() < 0.01

func set_target_facing(target_direction: Vector3) -> void:
	_target_direction = target_direction

func set_movement_vector(move_direction: Vector3) -> void:
	assert(not _navigation_agent)
	_move_direction = move_direction

func set_target(target_position: Vector3) -> void:
	assert(_navigation_agent)
	_navigation_agent.target_position = target_position

func is_at_target() -> bool:
	assert(_navigation_agent)
	return _navigation_agent.is_navigation_finished()

func _physics_process(delta: float) -> void:
	if _navigation_agent and not is_at_target():
		_move_direction = (_navigation_agent.get_next_path_position() - global_position).normalized()
	velocity = _move_direction * move_speed + _total_impulse
	_move_direction = Vector3.ZERO
	_total_impulse = Vector3.ZERO
	move_and_slide()
	var delta_rotation = get_angle_to_target_facing()
	if not is_facing_target():
		var cross = get_facing().cross(_target_direction)
		rotate_y(signf(cross.y) * min(delta_rotation, rotate_speed * delta))

func has_line_of_sight(bot: Bot) -> bool:
	var parameters: PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.new()
	parameters.from = global_position
	parameters.to = bot.global_position
	parameters.exclude = [self]
	parameters.collision_mask = BOT_PHYSICS_LAYER | WORLD_PHYSICS_LAYER
	var space_state = get_world_3d().direct_space_state
	var result = space_state.intersect_ray(parameters)
	return result and result.collider == bot
