extends Effect

func start() -> void:
	var grnd:Array[Vector2i] = tilemap.get_used_cells_by_id(-1,Vector2i.ZERO)
	for i in grnd:
		tilemap.erase_cell(i)
		print(i)
	pass 

func update(delta: float) -> void:
	pass 
