@tool class_name UpdateAwareness extends ActionLeaf

func tick(actor: Node, blackboard: Blackboard) -> int:
	var _bot: Bot = actor
	blackboard.set_value("known_actors", [])
	var actors: Array[Bot] = []
	var level = actor.get_parent()
	for child in level.get_children():
		if child == self:
			continue
		if not child is Bot:
			continue
		actors.append(child)
	blackboard.set_value("known_actors", actors)
	return SUCCESS
