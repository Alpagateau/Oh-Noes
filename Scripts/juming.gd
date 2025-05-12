extends Ability

@export var jump_force:float = 200
@export var downward_accel = 1
@export var buffer_timer:Timer 
@export var coyote_timer:Timer

@export var jump_divider:float = 4

var can_jump:bool = true
var was_grounded:bool = true

func update(delta: float) -> void:
	#Buffered jump
	if player.is_on_floor():
		can_jump = true
		if !buffer_timer.is_stopped():
			jump()
	else:
		if player.velocity.y > 0:
			player.velocity.y += delta * downward_accel
		if was_grounded:
			coyote_timer.start()
	
	if !can_jump:
		if Input.is_action_just_released("Action"):
			if player.velocity.y < 0: 
				player.velocity.y /= jump_divider
	
	if Input.is_action_just_pressed("Action"):
		jump()
	was_grounded = player.is_on_floor()
	
func jump():
	if can_jump:
		buffer_timer.stop()
		player.velocity.y = -jump_force
		can_jump = false
		$JumpSound.play()
	else:
		buffer_timer.start()

func _on_coyote_time_timeout() -> void:
	can_jump = false
	pass # Replace with function body.
