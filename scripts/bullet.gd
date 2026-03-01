class_name Bullet extends CharacterBody3D

static var DAMAGE = 1

@export var speed: float = 100.0
@export var impact_impulse: float = 10.0
@export var fire_impulse: float = 20.0
	
func _physics_process(_delta: float) -> void:
	velocity = -global_basis.z * speed
	var direction = velocity.normalized()
	if move_and_slide():
		for i in range(get_slide_collision_count()):
			var collision: KinematicCollision3D = get_slide_collision(i)
			var collider = collision.get_collider()
			if not collider is Bot:
				continue
			collider.on_damage(DAMAGE, direction * impact_impulse)
		queue_free()
