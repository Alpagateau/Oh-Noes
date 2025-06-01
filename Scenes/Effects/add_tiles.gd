extends Effect
var layer:TileMapLayer = null

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	for child in get_children():
		if child is TileMapLayer:
			layer = child
			break
	if layer == null:
		push_error("This scene requires a TileMapLayer")
	pass # Replace with function body.

func start() -> void:
	super.start()
	var cs := layer.get_used_cells()
	print("Debug 1")
	for p in cs:
		#adds the tile in the main tilemap 
		tilemap.set_cell(p,-1,layer.get_cell_atlas_coords(p))
	layer.queue_free()
