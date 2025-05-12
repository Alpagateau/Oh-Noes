class_name Ability extends Node

@export var is_physics:bool
@export var enabled:bool = true

var player:PlayerController

func start() -> void:
	player = get_parent()
	pass 

func update(delta: float) -> void:
	pass 

func activate() -> void:
	pass
