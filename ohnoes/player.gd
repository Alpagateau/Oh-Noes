extends CharacterBody2D


@export var ACCEL = 10.0
@export var MAXSPEED:float = 100.0
@export var DRAG:float = 100
@export var TURNACC:float = 15
@export var JUMP_VELOCITY = 400.0
@export var LIFT:float = 300
var speed:float = 0

func _physics_process(delta: float) -> void:
	# Add the gravity.
	if not is_on_floor():
		velocity += get_gravity() * delta

	# Handle jump.
	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
		velocity.y = -JUMP_VELOCITY
	if Input.is_action_pressed("ui_accept") and not is_on_floor():
		velocity.y -= LIFT * delta
		
	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var direction := Input.get_axis("ui_left", "ui_right")
	if direction:
		speed += direction * ACCEL * delta
		if sign(speed) != sign(direction):
			speed += TURNACC * delta * direction
		if abs(speed) >= MAXSPEED:
			speed = MAXSPEED * sign(speed)
		velocity.x = speed
	else:
		speed = move_toward(speed, 0, delta*ACCEL*DRAG)
		velocity.x = speed

	move_and_slide()
