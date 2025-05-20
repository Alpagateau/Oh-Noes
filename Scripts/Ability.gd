class_name Ability extends Node

@export var is_physics:bool
@export var enabled:bool = true

var player:PlayerController

var locks:Array[String] = []

func lock(caller):
	if caller.name in locks:
		return 
	locks += [caller.name]
	enabled = false 
	print(name,locks)

func unlock(caller):
	if caller.name not in locks:
		return
	locks.erase(caller.name)
	enabled = (locks == [])
	print(name,locks)

func start() -> void:
	player = get_parent()
	pass 

func update(delta: float) -> void:
	pass 

func activate() -> void:
	pass
