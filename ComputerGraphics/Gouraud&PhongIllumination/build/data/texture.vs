//global variables from the CPU
uniform mat4 model;//Aqui recibimos la model view projection desde la aplicacion
uniform mat4 mvp;
uniform vec3 Eye;//Pasem l'eye de la camara

uniform vec3 pos_light;
uniform vec3 pos_light2;

uniform vec3 ia, id, is;//Quantitat de llum rebuda per l'objecte
uniform vec3 id2, is2;
uniform vec3 ka, kd, ks;//Quantitat de llum que reflexa l'objecte
uniform float alpha; //factor de brillantor

//vars to pass to the pixel shader
varying vec2 v_coord;
varying vec3 normal;
varying vec3 v_color;//para enviar al pixel shader el color
varying vec3 V;//vector posicio de l'ull;
varying vec3 pos;

//here create uniforms for all the data we need here

void main()
{	

	//get the texture coordinates (per vertex) and pass them to the pixel shader
	v_coord = gl_MultiTexCoord0.xy;
	
	pos = (model * vec4( gl_Vertex.xyz, 1.0)).xyz;
	normal = (model * vec4( gl_Normal.xyz, 0.0)).xyz;
	
	V = (Eye - pos);
	V = normalize(V);//Normalitzem V

	//project the vertex by the model view projection 
	gl_Position = mvp * gl_Vertex; //output of the vertex shader. transformamos el vertice a screen space
}


