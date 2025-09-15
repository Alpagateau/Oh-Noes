extends Node

@export var Player:PlayerController
@export var Tilemap:TileMapLayer
@export var World:Node2D

var effects:Array[Effect]
var start_world:Node2D

func _ready() -> void:
	populate()
	Console.add_command("list_effects", command_list_effect)
	Console.add_command("enable", command_enable_effect, ["effect"])
	Console.add_command("disable", command_disable_effect, ["effect"])
	Console.add_command("reload", command_restart_level)
	start_world = World.duplicate()

func _process(delta: float) -> void:
	for a in effects:
		if !a.is_physics && a.enabled:
			a.update(delta)
	# TODO Add collider to spikes here

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
		a.player = World.Player 
		a.tilemap = World.Tilemap
		a.world = World
		if a.enabled:
			a.start()

func get_ability(id:String):
	if effects.size() == 0:
		populate()
	for a in effects:
		if a.name == id:
			return a

func command_list_effect():
	for e in effects:
		Console.print_line(e.name)

func command_enable_effect(element:String):
	for e in effects:
		if e.name == element:
			e.enabled = true
			return
	Console.print_error("Not an effect")

func command_disable_effect(element:String):
	for e in effects:
		if e.name == element:
			e.enabled = false
			return
	Console.print_error("Not an effect")

func command_restart_level():
	var nworld = start_world.duplicate()
	Player.reload()
	World.call_deferred("queue_free")
	World.call_deferred("add_sibling",nworld)
	World = nworld
	populate()
	pass
