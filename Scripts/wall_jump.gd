extends Ability

@export var jump_force:float = 200
@export var bounce:float = 100
@export var downward_accel = 1
@export var buffer_timer:Timer 
@export var coyote_timer:Timer
@export var jump_divider:float = 4

var can_jump:bool = true
var was_grounded:bool = true

var wall_grip:Ability
var jump_ab:Ability

func start() -> void:
	super.start()
	
	wall_grip = player.get_ability("Wall Grip")
	jump_ab = player.get_ability("Jump")
	
	if wall_grip == null:
		print("Can't Wall Jump without Wall Grip")
	if jump_ab == null:
		print("No jump, it'll be hard to wall jump")

func update(delta: float) -> void:
	#Buffered jump
	if wall_grip.gripped:
		can_jump = !player.is_on_floor()
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
	was_grounded = wall_grip.gripped
	
func jump():
	if can_jump:
		var norm:Vector2 = player.get_wall_normal()
		var new_vel = norm * bounce + Vector2.UP * jump_force
		buffer_timer.stop()
		wall_grip.gripped = false
		player.velocity = new_vel
		can_jump = false
		if jump_ab:
			jump_ab.can_jump = false
		$JumpSound.play()
		$PostJump.start()
		player.get_ability("Air Control").lock(self)
		wall_grip.lock(self)
	else:
		if !player.is_on_floor():
			buffer_timer.start()

func _on_coyote_time_timeout() -> void:
	can_jump = false

func _on_post_jump_timeout() -> void:
	player.get_ability("Air Control").unlock(self)
	wall_grip.unlock(self)
