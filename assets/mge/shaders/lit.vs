#version 330

layout (location = 0) in vec3 dPosition;
layout (location = 1) in vec3 dNormal;
layout (location = 2) in vec2 dTexCoords;

#define MAXHEIGHT = 2;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 directionalLightVector;


uniform mat4 mvpMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out float diffuseIntensity;

out vec3 fNormal;
out vec3 fPosition;
out vec2 texCoord;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 lightSpaceMatrix;

void main (void) {
	gl_Position = mvpMatrix * (vec4(vertex,1.f)) ;
	texCoord = uv;

	fNormal = vec3 (modelMatrix * vec4 (normal, 0));
	fPosition = vec3(modelMatrix * vec4(vertex, 1.0f));

	//Shadow stuff
	vs_out.FragPos = fPosition;
	vs_out.Normal = transpose(inverse(mat3(modelMatrix))) * normal;
	vs_out.TexCoords = uv;

}
