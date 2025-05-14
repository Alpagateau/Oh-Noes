extends Area2D

@export var tiles:Array[Vector2i]
@export var world:Node2D

@export var a:PackedScene
@export var g:PackedScene

func _on_area_entered(area: Area2D) -> void:
	print("Reload the thang")
	
	var new_child = null
	if randf() > 0.5:
		new_child = a.instantiate() 
		new_child.abi = world.player.get_random_ab()
		print("Removed some ab")
	else:
		new_child = g.instantiate()
		new_child.tiles = [tiles[randi_range(0, len(tiles)-1)]]
		new_child.rep = [tiles[randi_range(0, len(tiles)-1)]]
		print("Replacing")
	new_child
	pass # Replace with function body.
