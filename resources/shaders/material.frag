#version 420 core

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

	// TODO: Investigate to change on boolean
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

struct Material {
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 Normal;
in vec3 FragPos;

uniform Light light;
uniform Material material;
uniform vec3 viewPosition;

out vec4 color;


vec3 calculateDirectionLight(Light, Material, vec3, vec3);
vec3 calculatePointLight(Light, Material, vec3, vec3, vec3);
vec3 calculateSpotLight(Light, Material, vec3, vec3, vec3);


void main() {
    vec3 result;
	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPosition - FragPos);

	if (light.isDirection == 1) {
		result = calculateDirectionLight(light, material, norm, viewDir);
	}
	else if (light.isPoint == 1) {
		result = calculatePointLight(light, material, norm, FragPos, viewDir);
	}
	else if (light.isSpot == 1) {
		result = calculateSpotLight(light, material, norm, FragPos, viewDir);
	}

	color = vec4((result * material.color) * light.intensity, 1.0f);
}


vec3 calculateDirectionLight(Light light, Material material, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	// Ambient
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	return ambient + diffuse + specular;
}


vec3 calculatePointLight(Light light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// Ambient
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular
    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}


vec3 calculateSpotLight(Light light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir) {
	// Ambient
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular
    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

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
