extends Ability

@export var max_downspeed : float = 400

func update(delta: float) -> void:
	if not enabled:
		return
	if not player.is_on_floor():
			player.velocity += player.get_gravity() * delta
			if player.velocity.y >= max_downspeed:
				player.velocity.y = max_downspeed
