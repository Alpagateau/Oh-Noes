extends Effect

@export var tiles:Array[Vector2i]
@export var rep:Array[Vector2i]

func start() -> void:
	super.start()
	
	var tm:TileMapLayer = world.tilemap
	var cells:Array[Vector2i] = tm.get_used_cells()
	for c in cells:
		if tm.get_cell_atlas_coords(c) in tiles:
			if len(rep) == 0:
				tm.erase_cell(c)
			else:
				tm.set_cell(c, -1, rep[randi_range(0, len(rep)-1)],0)
