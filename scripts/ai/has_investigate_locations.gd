@tool extends ConditionLeaf

func tick(actor: Node, blackboard: Blackboard) -> int:
	var _bot: Bot = actor
	var investigate_locations: Array[Vector3] = blackboard.get_value("investigate_locations")
	if not investigate_locations.is_empty():
		return SUCCESS
	return FAILURE
