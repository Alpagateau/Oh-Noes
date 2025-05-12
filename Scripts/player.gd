class_name PlayerController extends CharacterBody2D
# Nodes 
@onready var ANIMATOR = $AnimatedSprite2D

var abilities:Array[Ability]

func _ready() -> void:
	populate()

func _process(delta: float) -> void:
	for a in abilities:
		if !a.is_physics && a.enabled:
			a.update(delta)

func _physics_process(delta: float) -> void:
	for a in abilities:
		if a.is_physics && a.enabled:
			a.update(delta)
	move_and_slide()

func populate():
	abilities = []
	for child in get_children():
		if child is Ability:
			abilities.append(child)
	
	for a in abilities:
		if a.enabled:
			a.start()

func get_ability(name:String):
	if abilities.size() == 0:
		populate()
	for a in abilities:
		if a.name == name:
			return a
