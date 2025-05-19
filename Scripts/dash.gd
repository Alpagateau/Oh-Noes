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

var current_coin:Node2D 
@export var coin_part:PackedScene

var shapeTween:Tween
var posTween:Tween

var dash_dir:Vector2
var dashing:bool = false
var dash_remaining = 2
var move:Ability
var air:Ability
var grav:Ability



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
	if  Input.is_action_just_pressed("Action2"):
		if !dashing:
			dash()
		else:
			$Timers/Buffer.start()
	if dashing:
		player.velocity = dash_dir.normalized() * DASH_SPEED
		if DASH_TIMER.is_stopped():
			player.velocity.x /= 2
			player.velocity.y /= 2
			dashing = false
	else:
		if player.is_on_floor():
			dash_remaining = DASH_COUNT

func dash() -> void:
	var dir = Vector2(
		Input.get_axis("D-Left", "D-Right"), 
		Input.get_axis("D-Up", "D-Down")
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
		dash_dir = dir
	player.create_shadow(0.3)
	DASH_SOUND.play()
	DASH_PARTICLES.direction = -dash_dir
	dashing = true
	DASH_PARTICLES.emitting = true
	DASH_TIMER.start()
	DASH_COOLDOWN.start()
	move.enabled = false
	grav.enabled = false
	air.enabled = false
	collider.scale.y = crouch_collider_size
	
func _on_dash_timer_timeout() -> void:
	move.enabled = true
	grav.enabled = true
	air.enabled = true
	collider.scale.y = 1
	pass # Replace with function body.

func _on_buffer_timeout() -> void:
	if !dashing:
		dash()
	pass # Replace with function body.
