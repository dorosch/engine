#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 lighting;
uniform vec3 viewPosition;
uniform vec3 lightingPosition;


void main() {
	vec3 object_color = vec3(0.66);

	// Ambient
	float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lighting;

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightingPosition - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lighting;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lighting;  

	vec3 result = (ambient + diffuse + specular) * object_color;
	color = vec4(result, 1.0f);
}
