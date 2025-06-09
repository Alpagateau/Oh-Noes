// NOTE: Shader automatically converted from Godot Engine 4.4.1.stable's CanvasItemMaterial.

shader_type canvas_item;
render_mode blend_mix;


uniform vec4 tint_color = vec4(1.0, 1.0, 1.0, 1.0);

void fragment() {
    COLOR = texture(TEXTURE, UV);
    if(COLOR.a < 0.5)
	{
		COLOR = tint_color;
	}
}
