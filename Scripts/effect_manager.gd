extends Node

@export var Player:CharacterBody2D
@export var Tilemap:TileMapLayer
@export var World:Node2D

var effects:Array[Effect]

func _ready() -> void:
	populate()

func _process(delta: float) -> void:
	for a in effects:
		if !a.is_physics && a.enabled:
			a.update(delta)

func _physics_process(delta: float) -> void:
	for a in effects:
		if a.is_physics && a.enabled:
			a.update(delta)

func populate():
	effects = []
	for child in get_children():
		if child is Effect:
			effects.append(child)
			
	for a in effects:
		a.player = Player 
		a.tilemap = Tilemap
		a.world = World
		if a.enabled:
			a.start()

func get_ability(name:String):
	if effects.size() == 0:
		populate()
	for a in effects:
		if a.name == name:
			return a
