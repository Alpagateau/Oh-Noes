extends Ability

@export var DASH_SPEED = 200
@export var DASH_COUNT = 3
@export var DASH_TIMER:Timer
@export var DASH_COOLDOWN:Timer
@export var DASH_PARTICLES:CPUParticles2D
@export var DASH_SOUND:AudioStreamPlayer2D
@export var CHAINED_DASH_SOUND:AudioStreamPlayer2D
@export var GRACE_TIMER:Timer

@export var crouch:bool = true
@export var crouch_collider_size:float = 0.3

var collider:CollisionShape2D
var dash_collider:CollisionShape2D

var current_coin:Node2D 
@export var coin_part:PackedScene

var dash_dir:Vector2

var dashing:bool = false
var dash_remaining = 2
var move:Ability
var air:Ability
var grav:Ability

signal start_dashing 
signal stop_dashing

func start():
	super.start()
	move = player.get_ability("Moving")
	air = player.get_ability("Air Control")
	grav = player.get_ability("Gravity")
	for c in player.get_children():
		if c is CollisionShape2D:
			collider = c
			break
	player.ANIMATOR.animation
	player.ANIMATOR.frame
	
#Either _process or _physics_process depending on the Is Physics value
func update(delta: float) -> void:
	if not enabled:
		return
	dashing = !DASH_TIMER.is_stopped()
	if  Input.is_action_just_pressed("Action2"):
		if !dashing:
			dash()
		else:
			$Timers/Buffer.start()
	if dashing:
		player.velocity = dash_dir
		if player.velocity.length() < DASH_SPEED:
			pass
	else:
		if player.is_on_floor():
			dash_remaining = DASH_COUNT

func dash() -> void:
	#print("Speed when dashing : ", player.velocity.length())
	var dir = clamp_dir(Vector2(
		Input.get_axis("D-Left", "D-Right"), 
		Input.get_axis("D-Up", "D-Down")
		)
		)
	
	if GRACE_TIMER.is_stopped():
		if dir == Vector2(0, 0):
			return
		if !DASH_TIMER.is_stopped():
			return
		if dash_remaining <= 0:
			return
		dash_remaining -= 1
	else: 
		CHAINED_DASH_SOUND.play()
		if current_coin != null: 
			player.position = current_coin.position
			var prt:CPUParticles2D = coin_part.instantiate()
			prt.position = current_coin.position
			prt.emitting = true
			current_coin = null
			add_sibling(prt)
		
	if dir != Vector2.ZERO:
		dash_dir = dir.normalized() * DASH_SPEED
	DASH_SOUND.play()
	DASH_PARTICLES.direction = -dash_dir
	DASH_PARTICLES.emitting = true
	DASH_TIMER.start()
	DASH_COOLDOWN.start()
	move.lock(self)
	grav.lock(self)
	air.lock(self)
	var shapeTween = get_tree().create_tween()
	shapeTween.tween_property(collider, "scale" ,Vector2(1, crouch_collider_size), 0.1)
	#print(shapeTween.is_valid())
	collider.scale.y = crouch_collider_size
	player.create_shadow(0.3)
	start_dashing.emit()
	
func _on_dash_timer_timeout() -> void:
	#print("Player speed :", player.velocity.length())
	move.unlock(self)
	grav.unlock(self)
	air.unlock(self)
	#collider.scale.y = 1
	var shapeTween = get_tree().create_tween()
	shapeTween.tween_property(collider, "scale", Vector2.ONE, 0.1)
	#print(shapeTween.is_valid())
	player.velocity.x /= 2
	player.velocity.y /= 2
	stop_dashing.emit()
	pass # Replace with function body.

func clamp_dir(odir:Vector2) -> Vector2:
	var lim:float = 0.5
	var output:Vector2 = Vector2.ONE 
	
	if abs(odir.x) > lim:
		output.x *= sign(odir.x)
	else:
		output.x = 0
	
	if abs(odir.y) > lim:
		output.y *= sign(odir.y)
	else:
		output.y = 0
	return output

func _on_buffer_timeout() -> void:
	if !dashing:
		dash()
	pass # Replace with function body.
