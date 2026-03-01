@tool extends ActionLeaf

func tick(actor: Node, blackboard: Blackboard) -> int:
	var bot: Bot = actor
	var known_actors: Array[Bot] = blackboard.get_value("known_actors")
	assert(known_actors)
	assert(not known_actors.is_empty())
	var target_actor = known_actors[0]
	var target_direction = (target_actor.global_position - actor.global_position).normalized()
	bot.set_target_facing(target_direction)
	return SUCCESS
