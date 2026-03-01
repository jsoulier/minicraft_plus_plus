@tool extends ActionLeaf

func tick(actor: Node, _blackboard: Blackboard) -> int:
	var bot: Bot = actor
	if bot.is_at_target():
		return SUCCESS
	bot.set_target_facing(bot._navigation_agent.get_next_path_position())
	return RUNNING
