extends Ability

func update(delta: float) -> void:
	if not enabled:
		return
	if not player.is_on_floor():
			player.velocity += player.get_gravity() * delta
			
