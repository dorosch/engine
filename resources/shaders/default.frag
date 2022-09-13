#version 420 core

struct Light {
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

uniform Light light;


void main() {
	vec3 object_color = vec3(0.66);

	// Ambient
    vec3 ambient = light.ambient * light.color;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * light.color);

	// Specular
	vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = light.specular * (spec * light.color);  

	vec3 result = ambient + diffuse + specular;
	color = vec4((result * light.intensity) * object_color, 1.0f);
}
