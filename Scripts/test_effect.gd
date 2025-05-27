extends Effect

@export var tile:Vector2i

func start() -> void:
	var grnd:Array[Vector2i] = world.Tilemap.get_used_cells_by_id(-1,tile, -1)
	for i in grnd:
		tilemap.erase_cell(i)
	pass 

func update(delta: float) -> void:
	pass 
