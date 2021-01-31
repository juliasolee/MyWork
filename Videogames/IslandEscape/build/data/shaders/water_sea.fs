
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform sampler2D u_texture;
uniform sampler2D u_sky_texture;
uniform sampler2D u_normal_water;
uniform float u_time;
uniform vec4 u_color;
uniform vec3 u_cam_pos;

void main()
{
	vec2 uv = v_uv;
	uv = v_world_position.xz * 0.01;
	
	vec3 N = vec3(0.0,1.0,0.0);

	vec3 npixel = texture2D(u_normal_water, uv + vec2(u_time * 0.01)).xzy;
	vec3 npixel2 = texture2D(u_normal_water, uv + vec2(u_time * -0.01, u_time * 0.15)).xzy;
	npixel = mix(npixel,npixel2,0.5);
	
	N = mix(N, (npixel * 2.0) - vec3(1.0),0.2);
	N = normalize(N);

	vec3 E = normalize(v_world_position - u_cam_pos);
	vec3 R = reflect( E, N );

	vec4 color = u_color * texture2D( u_texture, uv );

	//compute the yaw using arc tangent 
	float yaw = atan(R.x,R.z) / 6.28318531;
	yaw += -0.25; //in case the 0deg doesn’t match our Z axis, we can rotate 90 degrees

	//compute the pitch using arc sinus
	float pitch = asin(R.y) / 1.57079633; //0 means bottom, 1 means top

	//build the UV vector for hemisphere (in case pitch is negative, clamp it to 0)
	vec2 uv_reflection = vec2(yaw, clamp(pitch, 0.0, 1.0));

	//read the sky texture (ignoring mipmaps to avoid problems)
	vec4 sky_color = texture2D( u_sky_texture, uv_reflection );

	float fresnel = clamp(dot(R,N), 0.0, 1.0);
	color.a = 0.3;
	color = mix( color, sky_color, 1.0 - fresnel);
	//color.xyz = npixel;

	gl_FragColor = color;
	
}
