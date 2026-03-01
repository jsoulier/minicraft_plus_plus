@tool class_name Init extends ActionLeaf

func tick(actor: Node, blackboard: Blackboard) -> int:
	var _bot: Bot = actor
	var is_initialized = blackboard.get_value("is_initialized")
	if is_initialized:
		return SUCCESS
	var known_actors: Array[Bot] = []
	var investigate_locations: Array[Vector3] = []
	blackboard.set_value("is_initialized", true)
	blackboard.set_value("known_actors", known_actors)
	blackboard.set_value("investigate_locations", investigate_locations)
	return SUCCESS
