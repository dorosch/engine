#version 420 core

struct Light {
	// Only for directional lighting
    vec3 direction;

    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;

	// Type of lighting
	int isDirection;
	int isPoint;
	int isSpot;
};

in vec3 Normal;
in vec3 FragPos;

uniform Light light;
uniform vec3 viewPosition;

out vec4 color;


vec3 calculateDirectionLight(Light light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	// Ambient
	vec3 ambient = light.ambient * light.color;

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * light.color);

	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = light.specular * (spec * light.color);  

	return ambient + diffuse + specular;
}


vec3 calculatePointLight() {
	return vec3(0.0f);
}


vec3 calculateSpotLight() {
	return vec3(0.0f);
}


void main() {
	vec3 objectColor = vec3(0.66f);

	vec3 result = vec3(0.0f);
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - FragPos);

	if (light.isDirection == 1) {
		result += calculateDirectionLight(light, norm, viewDir);
	}
	else if (light.isPoint == 1) {
		result += calculatePointLight();
	}
	else if (light.isSpot == 1) {
		result += calculateSpotLight();
	}

	color = vec4((objectColor * result) * light.intensity, 1.0f);
}
