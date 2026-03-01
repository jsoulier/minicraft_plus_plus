@tool extends ActionLeaf

func tick(actor: Node, blackboard: Blackboard) -> int:
	var bot: Bot = actor
	var previous_known_actors: Array[Bot] = blackboard.get_value("known_actors")
	var investigate_locations: Array[Vector3] = blackboard.get_value("investigate_locations")
	var new_known_actors: Array[Bot] = []
	var level = actor.get_parent()
	for child in level.get_children():
		if child == self:
			continue
		if not child is Bot:
			continue
		var other_bot: Bot = child
		if not bot.has_line_of_sight(other_bot):
			continue
		new_known_actors.append(child)
	if new_known_actors.is_empty():
		for previous_known_actor in previous_known_actors:
			# TODO: we should be getting new_known_actor's previous frame position
			if not previous_known_actor in new_known_actors:
				investigate_locations.append(previous_known_actor.global_position)
	else:
		investigate_locations.clear()
	blackboard.set_value("known_actors", new_known_actors)
	blackboard.set_value("investigate_locations", investigate_locations)
	return SUCCESS
