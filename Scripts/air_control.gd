extends Ability

@export var speed:float = 300.0
@export var accel:float = 10
@export var turn:float = 15

var target_vel:float = 0

func update(delta: float) -> void:
	if not enabled:
		return
	if !player.is_on_floor():
		var direction := Input.get_axis("D-Left", "D-Right")
		if direction:
			if sign(direction) != sign(player.velocity.x):
				player.velocity.x += turn * direction * delta
			else:
				player.velocity.x += accel * direction * delta
			
			if abs(player.velocity.x) > speed: 
				player.velocity.x = speed * sign(player.velocity.x)
			
			player.ANIMATOR.play("Walk")
			player.ANIMATOR.flip_h = direction < 0
		else:
			player.velocity.x = move_toward(player.velocity.x, 0, turn)
			player.ANIMATOR.play("Idle")
