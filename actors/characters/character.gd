class_name Character extends CharacterBody2D

var _controller: Controller = null

func on_possess(controller: Controller) -> void:
	_controller = controller
	
func on_unpossess() -> void:
	_controller = null
