@tool extends ConditionLeaf

func tick(actor: Node, blackboard: Blackboard) -> int:
	var _bot: Bot = actor
	var known_actors: Array[Bot] = blackboard.get_value("known_actors")
	if not known_actors.is_empty():
		return SUCCESS
	return FAILURE
