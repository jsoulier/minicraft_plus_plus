@tool extends ActionLeaf

func tick(actor: Node, blackboard: Blackboard) -> int:
	var bot: Bot = actor
	var investigate_locations: Array[Vector3] = blackboard.get_value("investigate_locations")
	assert(not investigate_locations.is_empty())
	var current_investigate_location: Vector3 = investigate_locations[0]
	investigate_locations.remove_at(0)
	bot.set_target(current_investigate_location)
	blackboard.set_value("investigate_locations", investigate_locations)
	return SUCCESS
