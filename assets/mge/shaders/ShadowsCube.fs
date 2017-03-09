#version 330

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;
//vec3 lightPos = vec3(-2.0f, 4.0f, -1.0f);
//float far_plane = 25.0;

void main (void) {
	
	//Get the distance between fragment and light source
	float lightDistance = length(FragPos.xyz - lightPos);
	
	//Map to [0;1] range by dividing by far_plane
	lightDistance = lightDistance / far_plane;
	
	//Write this as modified depth
	gl_FragDepth = lightDistance;
}