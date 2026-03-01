@tool extends SequenceComposite

func tick(actor: Node, blackboard: Blackboard) -> int:
	var _bot: Bot = actor
	var known_actors: Array[Bot] = blackboard.get_value("known_actors")
	var investigate_locations: Array[Vector3] = blackboard.get_value("investigate_locations")	
	if not known_actors.is_empty():
		return FAILURE
	if not investigate_locations.is_empty():
		return FAILURE
	return SUCCESS
