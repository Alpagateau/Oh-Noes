extends Effect
var layer:TileMapLayer = null

# Called when the node enters the scene tree for the first time.
@export var eraseTile:Vector2i = Vector2i(0, 7)

func _ready() -> void:
	for child in get_children():
		if child is TileMapLayer:
			layer = child
			break
	if layer == null:
		push_error("This scene requires a TileMapLayer")
	
	layer.visible = false
	layer.collision_enabled = false 
	
	pass # Replace with function body.

func start() -> void:
	super.start()
	var cs := layer.get_used_cells()
	print("Debug 1")
	for p in cs:
		#adds the tile in the main tilemap 
		var i   := layer.get_cell_source_id(p)
		var coo := layer.get_cell_atlas_coords(p)
		if coo == eraseTile:
			tilemap.set_cell(p, -1,Vector2i(-1, -1))
		else:
			tilemap.set_cell(p,i,coo)
		print(p, i)
	
	#layer.queue_free()
