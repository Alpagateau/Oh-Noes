extends Ability

var dash:Ability
var jmp:Ability

var waving:bool = false

func start() -> void:
	super.start()
	dash = player.get_ability("Dash")
	jmp = player.get_ability("Jump")
	if ! dash.stop_dashing.is_connected(_stopped_dashing):
		dash.stop_dashing.connect(_stopped_dashing)

func update(_delta:float) -> void:
	if not enabled:
		return
	if Input.is_action_just_pressed("Action"):
		if dash.dashing:
			$Timer.start()
	pass

func _stopped_dashing():
	#print("Stopped Dashing")
	if not $Timer.is_stopped():
		if player.is_on_floor():
			if abs(player.velocity.x) < 0.2:
				player.velocity = Vector2.ZERO
				jmp.jump(false)
				player.velocity *= 1.7
				

func _on_timer_timeout() -> void:
	waving = false
	#print("Wave Finished")
	pass # Replace with function body.
