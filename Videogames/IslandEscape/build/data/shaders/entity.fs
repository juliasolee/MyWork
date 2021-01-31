
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D texture_entity;
uniform float u_time;
uniform vec4 u_color;

void main()
{
	vec2 uv = v_uv;
	vec4 color = u_color * texture2D( texture_entity, uv );
	if(color.a < 0.01)
		discard;
	gl_FragColor = color;
	
}
