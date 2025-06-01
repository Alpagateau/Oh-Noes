extends Area2D

@export var efx_manager:Node

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

func _on_body_entered(_body: Node2D) -> void:
	print("RESPAWN")
	efx_manager.command_restart_level()
	pass # Replace with function body.
