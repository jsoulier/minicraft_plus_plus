@tool extends ConditionLeaf

func tick(actor: Node, _blackboard: Blackboard) -> int:
	var bot: Bot = actor
	if bot.is_facing_target():
		return SUCCESS
	return FAILURE
