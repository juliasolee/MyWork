//global variables from the CPU
uniform mat4 model;//Aqui recibimos la model view projection desde la aplicacion
uniform mat4 mvp;
uniform vec3 Eye;//Pasem l'eye de la camara

uniform vec3 pos_light;
uniform vec3 pos_light2;

uniform vec3 ia, id, is;//Quantitat de llum rebuda per l'objecte
uniform vec3 id2, is2;//
uniform vec3 ka, kd, ks;//Quantitat de llum que reflexa l'objecte
uniform float alpha; //factor de brillantor

//vars to pass to the pixel shader
varying vec3 normal;
varying vec3 v_color;//para enviar al pixel shader el color
varying vec3 V;//vector posicio de l'ull

//here create uniforms for all the data we need here

void main()
{	
	vec3 pos = (model * vec4( gl_Vertex.xyz, 1.0)).xyz;
	normal = (model * vec4( gl_Normal.xyz, 0.0)).xyz;
	normal = normalize(normal);

	//calculem L
	vec3 L = (pos_light - pos);
	vec3 L2 = (pos_light2 - pos);
	
	L = normalize(L);//Normalitzem L
	L2 = normalize(L2);//Normalitzem L2
	
	V = (Eye - pos);
	V = normalize(V);//Normalitzem V
	
	vec3 R = -reflect(L, normal);//Calculem R
	vec3 R2 = -reflect(L2, normal);//Calculem R2
	R = normalize(R); //Normalitzem R
	R2 = normalize(R2); //Normalitzem R2

	//Ecuacio principal per trobar ip: ip = ambient + difusa + especular.
	//Els calcularem per separat
	
	vec3 ambient = ka*ia; //Calculem ambient: ambient = ka * ia;
	
	vec3 difusa = kd * clamp(dot(L, normal),0.0,1.0) * id; //Calculem difusa: difusa = kd * (L·N) * id
	vec3 difusa2 = kd * clamp(dot(L2, normal),0.0,1.0) * id2; //Calculem difusa2
	
	vec3 especular = ks * pow(clamp(dot(R,V),0.0,1.0), alpha) * is; //Calculem especular: especular = ks * (R·V)alpha * is;
	vec3 especular2 = ks * pow(clamp(dot(R2,V),0.0,1.0), alpha) * is2; //Calculem especular2
	
	vec3 ip1 = especular + difusa;
	vec3 ip2 = especular2 + difusa2;
	vec3 ip = ambient + ip1 + ip2;
	

	//pass it to the pixel shader as color (for debug)
	v_color = ip;

	//project the vertex by the model view projection 
	gl_Position = mvp * gl_Vertex; //output of the vertex shader. transformamos el vertice a screen space
}








