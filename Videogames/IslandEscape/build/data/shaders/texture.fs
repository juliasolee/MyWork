
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D sand_texture;
uniform sampler2D water_texture;
uniform sampler2D texture_mask;

uniform float u_time;

void main()
{
	vec2 uv = v_uv;
	vec4 water = texture2D( water_texture, uv*10 );
	vec4 sand = texture2D( sand_texture, uv*30 );
	
	float mask_r = texture2D( texture_mask, uv ).x;	

	vec4 color = mix(water, sand, clamp(mask_r + 0.7, 0.0, 1.0));

	gl_FragColor = color;
}
