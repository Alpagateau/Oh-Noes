extends Area2D

@export var efx_manager:Node

@export var new_target:Node2D
@export var wheel:Node2D

enum end_state {NONE, PAN, SPIN, DIALOG}
@export var cur_state:end_state = end_state.NONE
@export var speed:float = 0.4

var new_cam:Camera2D = null

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

func _process(delta: float) -> void:
	if cur_state == end_state.NONE:
		return
	if cur_state == end_state.PAN:
		var np:Vector2 = lerp(new_cam.global_position, new_target.global_position, speed * delta)
		var d = (np - new_cam.global_position).length()
		new_cam.global_position = np
		if d < 0.01:
			cur_state = end_state.SPIN
			print("Goodbye")
func _on_body_entered(_body: Node2D) -> void:
	print("RESPAWN")
	if(cur_state == end_state.NONE):
		var player:PlayerController = _body
		player.get_ability("CameraController").lock(self)
		var cam:Camera2D = player.get_ability("CameraController").cam
		new_cam = cam.duplicate()
		new_cam.global_position = cam.global_position
		add_sibling(new_cam)
		new_cam.reparent(new_target)
		
		cam.enabled = false
		cur_state = end_state.PAN
	#efx_manager.command_restart_level()
	pass # Replace with function body.
