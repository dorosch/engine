#version 420 core

#define MAX_LIGHT_SOURCES 32

#define DIRECTIONAL 0
#define POINT 		1
#define SPOT 		2

struct Light {
	// Only for directional lighting
    vec3 direction;

	// Only for point lighting
    float constant;
    float linear;
    float quadratic;

	// Only for spot lighting
	float cutOff;
	float outerCutOff;

	int type;
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 viewPosition;
uniform int lightSourcesCount;
uniform Light lights[MAX_LIGHT_SOURCES];


vec3 calculateDirectionLight(Light, vec3, vec3);
vec3 calculatePointLight(Light, vec3, vec3, vec3);
vec3 calculateSpotLight(Light, vec3, vec3, vec3);


void main() {
	vec3 result = vec3(0.0f);
	vec3 defaultColor = vec3(0.66f);
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - FragPos);

	for(int index = 0; index < lightSourcesCount; index++) {
		Light light = lights[index];

		if (light.type == DIRECTIONAL) {
			result += calculateDirectionLight(light, norm, viewDir) * light.intensity;
		}
		else if (light.type == POINT) {
			result += calculatePointLight(light, norm, FragPos, viewDir) * light.intensity;
		}
		else if (light.type == SPOT) {
			result += calculateSpotLight(light, norm, FragPos, viewDir) * light.intensity;
		}
	}

	color = vec4(result * defaultColor, 1.0f);
}


vec3 calculateDirectionLight(Light light, vec3 normal, vec3 viewDir) {
	// Ambient
	vec3 ambient = light.ambient * light.color;

	// Diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * light.color);

	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = light.specular * (spec * light.color);  

	return ambient + diffuse + specular;
}


vec3 calculatePointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// Ambient
	vec3 ambient = light.ambient * light.color;

	// Diffuse
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * light.color);

	// Specular
    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * (spec * light.color);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}


vec3 calculateSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// Ambient
	vec3 ambient = light.ambient * light.color;

	// Diffuse
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * light.color);

	// Specular
    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * (spec * light.color);

	// Spotlight - soft edges
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
