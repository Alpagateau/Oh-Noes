extends Ability

var gripped:bool = false
var gripped_x:float = 0

var dsh:Ability

@export var gripping_speed:float = 0

func start() -> void:
	super.start()
	dsh = player.get_ability("Dash")

var norm:Vector2

func update(delta:float) -> void:
	if not enabled:
		return
	var dir:float = Input.get_axis("D-Left", "D-Right")
	
	if player.is_on_wall():
		norm = player.get_wall_normal()
		if !player.is_on_floor():
			if dir * norm.x <= -1:
				if !gripped:
					player.velocity.x = 0
				gripped = true
				$Timer.stop()
				gripped_x = player.position.x
	
	if player.is_on_floor():
		gripped = false
	
	if gripped:
		if !player.is_on_wall():
			gripped = false
		player.position.x = gripped_x
		if player.velocity.y > 0:
			player.velocity.y = gripping_speed * delta
		if dir * norm.x >= 0:
			if $Timer.is_stopped():
				$Timer.start()
		if dsh != null:
			dsh.dash_remaining = dsh.DASH_COUNT
	pass

func _on_timer_timeout() -> void:
	gripped = false
	pass # Replace with function body.
