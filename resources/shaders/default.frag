#version 420 core

struct Light {
	// Only for directional lighting
    vec3 direction;

	// Only for point lighting
    float constant;
    float linear;
    float quadratic;

	// Type of lighting
	int isDirection;
	int isPoint;
	int isSpot;

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

uniform Light light;
uniform vec3 viewPosition;


vec3 calculateDirectionLight(Light light, vec3 normal, vec3 viewDir);
vec3 calculatePointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight();


void main() {
	vec3 result;
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - FragPos);

	if (light.isDirection == 1) {
		result = calculateDirectionLight(light, norm, viewDir);
	}
	else if (light.isPoint == 1) {
		result = calculatePointLight(light, norm, FragPos, viewDir);
	}
	else if (light.isSpot == 1) {
		result = calculateSpotLight();
	}

	color = vec4(result * light.intensity, 1.0f);
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


vec3 calculateSpotLight() {
	return vec3(0.0f);
}
