class_name Bullet extends CharacterBody3D

@export var speed: float = 100.0
	
func _physics_process(_delta: float) -> void:
	velocity = -global_basis.z * speed
	if move_and_slide():
		queue_free()
