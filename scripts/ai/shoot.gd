@tool class_name Shoot extends ActionLeaf

func tick(actor: Node, _blackboard: Blackboard) -> int:
	var bot: Bot = actor
	bot.shoot()
	return SUCCESS
