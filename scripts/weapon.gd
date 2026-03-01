class_name Weapon extends Node3D

@export var projectile_scene: PackedScene = null
@export var cooldown: float = 1.0
var _cooldown: float = 0.0
@onready var _muzzle = $Muzzle
@onready var _barrel = $Barrel
@onready var _bot = $".."

func _process(delta: float) -> void:
	_cooldown -= delta

func fire() -> void:
	if _cooldown > 0.0:
		return
	_cooldown = cooldown
	var projectile: CharacterBody3D = projectile_scene.instantiate()
	assert(projectile.top_level)
	add_child(projectile)
	projectile.global_position = _muzzle.global_position
	projectile.global_rotation = _barrel.global_rotation
	_bot._total_impulse += projectile.global_basis.z * projectile.fire_impulse
