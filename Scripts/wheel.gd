@tool
extends Node2D

@export var fxs:Array[Effect]
@export var colors:Array[Color]
@export var radius:float = 30
@export var font_settings:LabelSettings
@export var Res:Vector2i

@export var min_speed:float = 1 
@export var max_speed:float = 3
@export var deccel:float = 2

@export var mat:Material
@export var tex:Texture2D

@export_category("Debug")
@export_tool_button("Generate Wheel") var gen = self.generate
@export_tool_button("Rotate Wheel") var rot = self.launch

var is_turnin:bool = false
var speed:float = 0
var dtheta:float = 1

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	generate()
	launch()

func project(pos:Vector3) -> Vector3:
	var result:Vector3 = Vector3.ZERO
	result.x = cos((pos.x * dtheta) - dtheta/2) * pos.y * radius
	result.y = sin((pos.x * dtheta) - dtheta/2) * pos.y * radius
	return result
	
func generate():
	get_viewport().debug_draw = Viewport.DEBUG_DRAW_WIREFRAME
	print("Generating")
	var children = get_children()
	for child in children:
		child.free()
	var l:int = len(fxs)
	dtheta= 2*PI / l
	
	for i in range(l):
		var mesh := ArrayMesh.new()
		var st := SurfaceTool.new()
		st.begin(Mesh.PRIMITIVE_TRIANGLES)
		
		var cols:int = Res.x
		var rows:int = Res.y
	
		for y in range(rows):
			for x in range(cols):
			# Vertex positions
				var v0 = project(Vector3(    float(x)/cols,float(y)/rows, 0))
				var v1 = project(Vector3(  float(x+1)/cols,float(y)/rows, 0))
				var v2 = project(Vector3(  float(x+1)/cols,float(y+1)/rows, 0))
				var v3 = project(Vector3(    float(x)/cols ,float(y+1)/rows, 0))

				# UVs: Stretch one texture across the full mesh
				var uv0 = Vector2(float(x) / cols,     float(y) / rows)
				var uv1 = Vector2(float(x+1) / cols, float(y) / rows)
				var uv2 = Vector2(float(x+1) / cols, float(y+1) / rows)
				var uv3 = Vector2(float(x) / cols,     float(y+1) / rows)
				
				# UVs: Stretch one texture across the full mesh
				#var uv0 = Vector2(0,0)
				#var uv1 = Vector2(1,0)
				#var uv2 = Vector2(1,1)
				#var uv3 = Vector2(0,1)

			# First triangle (v0, v1, v2)
				st.set_uv(uv0)
				st.add_vertex(v0)
				st.set_uv(uv1)
				st.add_vertex(v1)
				st.set_uv(uv2)
				st.add_vertex(v2)

			# Second triangle (v2, v3, v0)
				st.set_uv(uv2)
				st.add_vertex(v2)
				st.set_uv(uv3)
				st.add_vertex(v3)
				st.set_uv(uv0)
				st.add_vertex(v0)

		st.generate_normals()
		st.index()
		st.commit(mesh)
		
		var mesh_instance := MeshInstance2D.new()
		mesh_instance.mesh = mesh
		mesh_instance.material = mat 
		mesh_instance.texture = tex
		var txt:Label = Label.new()
		txt.label_settings = font_settings
		txt.horizontal_alignment = HORIZONTAL_ALIGNMENT_RIGHT 
		txt.vertical_alignment = VERTICAL_ALIGNMENT_CENTER
		txt.size.x = radius * 2
		txt.size.y = radius * tan(dtheta) * 2
		txt.scale *= 0.5
		txt.position.y = -txt.size.y / 4
		#txt.position.x = txt.size.x / 4
		if fxs[i] != null:
			txt.text = fxs[i].name
		else:
			txt.text = "DEBUG"
		add_child(mesh_instance)
		mesh_instance.add_child(txt)
		mesh_instance.rotation = i * dtheta
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if is_turnin:
		rotate(speed * delta)
		speed -= deccel * delta
		if speed <= 0:
			speed = 0 
			is_turnin = false
	pass

func launch():
	speed = randf_range(min_speed, max_speed)
	is_turnin = true
	pass
