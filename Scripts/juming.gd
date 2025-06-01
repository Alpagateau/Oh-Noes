extends Ability

@export var jump_force:float = 200
@export var downward_accel = 1
@export var buffer_timer:Timer 
@export var coyote_timer:Timer
@export var long_jump_factor:float = 15

@export var jump_divider:float = 4

var can_jump:bool = true
var was_grounded:bool = true
var dsh:Ability

signal start_jumping

func start():
	super.start()
	dsh = player.get_ability("Dash")

func update(delta: float) -> void:
	#Buffered jump
	if not enabled:
		return 
	
	if player.is_on_floor():
		can_jump = true
		if !buffer_timer.is_stopped():
			jump(true)
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
		jump(true)
	was_grounded = player.is_on_floor()
	
func jump(wvdsh:bool):
	if can_jump:
		buffer_timer.stop()
		if dsh && wvdsh:
			if dsh.dashing:
				if abs(player.velocity.x) > 0.2:
					player.create_shadow(0.6)
					dsh.dashing = false
					dsh.DASH_TIMER.stop()
					dsh._on_dash_timer_timeout()
					var d = sign(player.velocity.x)
					player.velocity.x = dsh.DASH_SPEED * d * long_jump_factor
					dsh.dash_remaining += 1
		player.velocity.y = -jump_force
		can_jump = false
		$JumpSound.play()
	else:
		buffer_timer.start()

func _on_coyote_time_timeout() -> void:
	can_jump = false
	pass # Replace with function body.
