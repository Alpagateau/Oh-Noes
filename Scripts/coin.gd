extends Area2D

var enabled:bool = true

func _ready() -> void:
	pass 
	
func _process(delta) -> void:
	if enabled:
		$AnimatedSprite2D.modulate.a = 1
	else:
		$AnimatedSprite2D.modulate.a = 0.5

func _on_body_entered(body: Node2D) -> void:
	if enabled:
		enabled = false
		$Activation.start()
		var dash:Ability = body.get_ability("Dash")
		if dash != null:
			var t:Timer = dash.get_node("Timers/Grace")
			if t!=null:
				t.start()
				dash.current_coin = self

func _on_area_2d_body_exited(body: Node2D) -> void:
	if enabled:
		var dash:Ability = body.get_ability("Dash")
		if dash != null:
			var t:Timer = dash.get_node("Timers/Grace")
			if t != null:
				t.stop()
				if dash.current_coin == self:
					dash.current_coin = null

func _on_activation_timeout() -> void:
	enabled = true
