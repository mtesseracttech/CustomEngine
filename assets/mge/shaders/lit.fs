#version 330

#define i0 0
#define i1 1
#define i2 2
#define i3 3
#define i4 4
#define i5 5
#define i6 6
#define i7 7
#define i8 8


struct Material {
	vec3 specular;
	float shininess;
};
uniform Material material;

struct DirectionalLight {
	vec3 direction;
	vec3 color;
	sampler2D dirDepthMap;
	mat4 LSM;
	vec3 position;
};
uniform DirectionalLight directionalLight[4];

struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;

	float farPlane;
	//samplerCube pointCubeDepthMap;
};
uniform PointLight pointLight[4];
uniform samplerCube pointCubeDepthMap[4];

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 color;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};
uniform SpotLight spotLight[4];

uniform vec3 cameraPosition;
uniform int directionalLightCount;
uniform int pointLightCount;
uniform int spotLightCount;
uniform sampler2D specularMap;
uniform sampler2D emissionMap;

in vec3 fNormal;
in vec3 fPosition;
in vec2 texCoord;

out vec4 fragmentColor;
//Function declarations
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight (PointLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);
vec3 CalculateSpotLight (SpotLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition);

uniform sampler2D depthMap;
uniform bool useTextures;
uniform sampler2D textureDiffuse;
uniform samplerCube depthCubeMap;
uniform float far_plane;

uniform bool useSpecular;
uniform bool useEmission;
uniform int lightType;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

vec4 texture = texture(textureDiffuse,texCoord);
vec3 spec = vec3(texture(specularMap,texCoord));
vec3 emission = vec3(texture(emissionMap,texCoord));

vec3 endResult = vec3 (0,0,0);
vec3 endResult1 = vec3 (0,0,0);
vec3 endResult2 = vec3 (0,0,0);

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * 1.0 * 7.5) / (7.5 + 1.0 - z * (7.5 - 1.0));
}

float ShadowCalculationCube(PointLight cubeLight, int check)//, int number)
{
					float	bias = 0.05f;
				    if (check==i0) {
										vec3 fragToLight = fs_in.FragPos - cubeLight.position;
										float closestDepth = texture(pointCubeDepthMap[i0], fragToLight).r;
										closestDepth *= cubeLight.farPlane;
										float currentDepth = length(fragToLight);
										float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
									return shadow;
					}

				    if (check==i1) {
											vec3 fragToLight = fs_in.FragPos - cubeLight.position;
												float closestDepth = texture(pointCubeDepthMap[i1], fragToLight).r;
												closestDepth *= cubeLight.farPlane;
												float currentDepth = length(fragToLight);
												float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
												return shadow;
					}

				    if (check==i2) {
										vec3 fragToLight = fs_in.FragPos - cubeLight.position;
										float closestDepth = texture(pointCubeDepthMap[i2], fragToLight).r;
										closestDepth *= cubeLight.farPlane;
										float currentDepth = length(fragToLight);
										float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
										return shadow;
					}

				  if (check==i3) {
									 vec3 fragToLight = fs_in.FragPos - cubeLight.position;
									 float  closestDepth = texture(pointCubeDepthMap[i3], fragToLight).r;
									  closestDepth *= cubeLight.farPlane;
									 float  currentDepth = length(fragToLight);
									 float  shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
									  return shadow;
				}
					return 0.0f;
}

float ShadowCalculation (DirectionalLight directLight) {

					vec4 FPLS = directLight.LSM * vec4(fs_in.FragPos, 1.0);

					//Perform perspective divide
					vec3 projCoords = FPLS.xyz / FPLS.w;
					//Transform to [0,1] range
					projCoords = projCoords * 0.5 + 0.5;
					//Get closest depth value from light's pespective (using [0,1] range fragPosLight as coords)
					float closestDepth = texture(directLight.dirDepthMap, projCoords.xy).r;
					//Get depth of current fragment from light's perspective
					float currentDepth = projCoords.z;

					vec3 norm = normalize(fs_in.Normal);
					vec3 lightDir = normalize(directLight.position - fs_in.FragPos);
					float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);
					vec2 texelSize = 1.0 / textureSize(directLight.dirDepthMap, 0);
					//Check whether current frag pos is in shadow
					float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
					for (int x = -1; x <= 1; ++x) {
							for (int y = -1; y <= 1; ++y) {
										float pcfDepth = texture(directLight.dirDepthMap, projCoords.xy + vec2 (x, y) * texelSize).r;
										shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
									}
					}
					shadow /= 9.0;
					// Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
				    if(projCoords.z > 1.0) {
				        shadow = 0.0;
					}
					return shadow;
}

vec3 AddPointShadows (vec3 inputp, int check) {

					if (check == i0) {
						float shadow = ShadowCalculationCube (pointLight[i0], i0);
						shadow = min(shadow, 0.75);
						vec3 outputp = inputp * (1 - shadow);
						return outputp;
					}

					if (check == i1) {
						float shadow = ShadowCalculationCube (pointLight[i1], i1);
						shadow = min(shadow, 0.75);
						vec3 outputp = inputp * (1 - shadow);
						return outputp;
					}

				  if (check == i2) {
				  	 float shadow = ShadowCalculationCube (pointLight[i2], i2);
				    	shadow = min(shadow, 0.75);
				    	vec3 outputp = inputp * (1 - shadow);
				    	return outputp;
				  }

				  if (check == i3) {
				   float shadow = ShadowCalculationCube (pointLight[i3], i3);
				    shadow = min(shadow, 0.75);
				    vec3 outputp = inputp * (1 - shadow);
				    return outputp;
				  }
					return inputp;
}

void main (void) {

					vec2 resolution = vec2(1920, 1080);
					vec2 center = -1.0f + 2.0f * texCoord;
					float cLength = length(center);
					vec3 pNormal = normalize(fs_in.Normal);
					vec3 pViewDirection = normalize(cameraPosition - fs_in.FragPos);
					vec3 restulr = vec3(0,0,0);
						//Directional Lights
						for (int i = 0; i < directionalLightCount; i++) {
							endResult += CalculateDirectionalLight (directionalLight[i], pNormal, pViewDirection);
							float shadow = ShadowCalculation (directionalLight[i]);
							shadow = min(shadow, 0.75);
							endResult = endResult * (1 - shadow);
						}
						//Point Lights
						for (int i = 0; i <= pointLightCount; i++) {
							endResult1 += CalculatePointLight (pointLight[i], pNormal, pViewDirection, fPosition);
							endResult1 += AddPointShadows (endResult1, i) ;
							//restulr  += CalculatePointLight (pointLight[i], pNormal, pViewDirection, fPosition);
						}
						//Spot Lights
						for (int i = 0; i < spotLightCount; i++) {
							endResult2 += CalculateSpotLight (spotLight[i], pNormal, pViewDirection, fPosition);
						}

							fragmentColor = (vec4 (endResult , 1) + vec4(endResult1 ,1)+vec4(endResult2,1) ) * vec4(texture);
}

vec3 CalculateDirectionalLight (DirectionalLight light, vec3 normal, vec3 viewDirection) {
					vec3 lightDirection = normalize(-light.direction);
					float pDiffuse = max(dot(lightDirection, normal), 0);

					//Get the reflection direction
					vec3 reflection = reflect (-lightDirection, normal);
					//Get the intensity
					float pSpecular = pow(max(dot(viewDirection, reflection), 0), 256);

					vec3 diffuse = light.color * pDiffuse ;//*vec3(texture) ;
					//vec3 specular = light.color * pSpecular;
				//vec3 speculars = pSpecular * light.specular * vec3(texture(specularMap,texCoord));
				vec3 speculars = vec3(0,0,0);

				if(useSpecular)
				{
					 speculars = material.specular*   spec * pSpecular ;
				}

				if(useEmission)
				{
						return (diffuse + speculars + emission );
				}
				else {
					return (diffuse + speculars );
				}
}

vec3 CalculatePointLight (PointLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition) {
				vec3 lightDirection = normalize (light.position - fragmentPosition);
				float pDiffuse = max(dot(lightDirection, normal), 0);
				vec3 reflection = reflect (-lightDirection, normal);
				float pSpecular = pow(max(dot(reflection, viewDirection), 0), 256);

				float pDistance = length (light.position - fragmentPosition);
				float attenuation = 1.0f / (light.constant + light.linear * pDistance + light.quadratic * (pDistance * pDistance));

				vec3 diffuse = light.color * pDiffuse * attenuation ;
				vec3 speculars = vec3(0,0,0);
				if(useSpecular)
				{
					speculars = material.specular*  spec * pSpecular ;
				}

				if(useEmission)
				{
						return (diffuse + speculars + emission);
				}
				else {
					return (diffuse + speculars );
				}

}

vec3 CalculateSpotLight (SpotLight light, vec3 normal, vec3 viewDirection, vec3 fragmentPosition) {
					vec3 lightDirection = normalize (light.position - fragmentPosition);
					// Spotlight intensity
				  float theta = dot(lightDirection, normalize(-light.direction));
					float epsilon = (light.cutOff - light.outerCutOff);
					float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

					float pDiffuse = max(dot(lightDirection, normal), 0);
					vec3 reflection = reflect (-lightDirection, normal);
					float pSpecular = pow(max(dot(viewDirection, reflection), 0), material.shininess);

					//Attenuation
					float pDistance = length (light.position - fragmentPosition);
					//float attenuation = 1.0f / (light.constant + light.linear * pDistance + light.quadratic * (pDistance * pDistance));

					vec3 diffuse = light.color * pDiffuse * vec3(texture) ;// * attenuation;
					vec3 specular = vec3(0,0,0);//* attenuation;
					if(useSpecular)
					{
						specular =   pSpecular * spec ;
						specular *= intensity;
					}

					diffuse *= intensity;


					if(useEmission)
					{
							return (diffuse + specular + emission);
					}
					else {
						return (diffuse + specular );
					}
}
