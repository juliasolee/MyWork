//global variables from the CPU
uniform mat4 model;//Aqui recibimos la model view projection desde la aplicacion
uniform mat4 mvp;

//vars to pass to the pixel shader
varying vec3 normal;
varying vec3 pos;

//here create uniforms for all the data we need here

void main()
{	
	pos = (model * vec4( gl_Vertex.xyz, 1.0)).xyz;
	normal = (model * vec4( gl_Normal.xyz, 0.0)).xyz;

	//project the vertex by the model view projection 
	gl_Position = mvp * gl_Vertex; //output of the vertex shader. transformamos el vertice a screen space
}






