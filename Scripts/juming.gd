extends Ability

@export var jump_force:float = 200
@export var downward_accel = 1
@export var buffer_timer:Timer 
@export var coyote_timer:Timer
@export var long_jump_factor:float = 15
@export var jump_divider:float = 4
@export var min_jump_height:float = 14


var can_jump:bool = true
var was_grounded:bool = true
var dsh:Ability
var lst_jump_pos:float = 0
var stop_jump:bool = false

func start():
	super.start()
	dsh = player.get_ability("Dash")

func update(delta: float) -> void:
	#Buffered jump
	if not enabled:
		return 
	if player.is_on_floor():
		can_jump = true
		stop_jump = false
		if !buffer_timer.is_stopped():
			jump(true)
	else:
		if player.velocity.y > 0:
			player.velocity.y += delta * downward_accel
		if was_grounded:
			coyote_timer.start()
	
	if !can_jump:
		if Input.is_action_just_released("Action"):
			print("Request stop")
			stop_jump = true
	
	if stop_jump && !player.is_on_floor():
		var expected_height:float = abs( player.position.y - lst_jump_pos )
		print(expected_height)
		if player.velocity.y < 0 && expected_height >= min_jump_height:
			player.velocity.y /= jump_divider
			stop_jump = false
			print("Stopped jump")
	
	if Input.is_action_just_pressed("Action"):
		jump(true)
	was_grounded = player.is_on_floor()
	
func jump(wave_dash:bool):
	if can_jump:
		buffer_timer.stop()
		if dsh && wave_dash && dsh.dashing:
			if abs(player.velocity.x) > 0.2:
				player.create_shadow(0.6)
				dsh.dashing = false
				dsh.DASH_TIMER.stop()
				dsh._on_dash_timer_timeout()
				var d = sign(player.velocity.x)
				player.velocity.x = dsh.DASH_SPEED * d * long_jump_factor
				dsh.dash_remaining += 1
		lst_jump_pos = player.position.y
		player.velocity.y = -jump_force
		can_jump = false
		$JumpSound.play()
	else:
		buffer_timer.start()

func _on_coyote_time_timeout() -> void:
	can_jump = false

func get_parabolla_height():
	var spd:float = abs(player.velocity.y)
	var grv:float = player.get_gravity().y
	return (spd ** 2)/(2*grv)

func get_speed_for_height(h:float) -> float:
	return sqrt(2 * h * abs(player.get_gravity().y))
