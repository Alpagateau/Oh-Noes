extends Ability

@export var speed:float = 0.001
@export var look_ahead:float = 0.1

@export var cam:Node2D

func update(delta:float) -> void:
	var lp:Vector2 = player.position + player.velocity * look_ahead
	cam.position.x = lerp(cam.position.x, lp.x, speed * delta)
	cam.position.y = lerp(cam.position.y, lp.y, speed * delta * 0.1)
	pass
