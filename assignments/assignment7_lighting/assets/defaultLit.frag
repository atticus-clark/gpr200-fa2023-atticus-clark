#version 450
out vec4 FragColor;

in Surface{
	vec2 UV;
	vec3 worldPos;
	vec3 worldNormal;
}fs_in;

uniform sampler2D _Texture;
uniform vec3 _CamPos;

struct Light {
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

struct Material {
	float ambientK;
	float diffuseK;
	float specularK;
	float shininess;
};
uniform Material _Material;

void main(){
    vec3 normal = normalize(fs_in.worldNormal);
    vec4 color = texture(_Texture, fs_in.UV);

	vec3 lightColor = vec3(0.0);

	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		vec3 lightDir = normalize(_Lights[i].position - fs_in.worldPos);
		vec3 camDir = normalize(_CamPos - fs_in.worldPos);
		vec3 halfVector = normalize(lightDir + camDir);
    
		// diffuse
		float diffuse = max(dot(normal, lightDir), 0.0);
		vec3 diffuseCol = _Material.diffuseK * diffuse * _Lights[i].color;

		// specular
		float specular = pow(max(dot(normal, halfVector), 0.0), _Material.shininess);
		vec3 specularCol = _Material.specularK * specular * _Lights[i].color;

		// add together
		lightColor += (diffuseCol + specularCol);
	}
	lightColor += _Material.ambientK;

    FragColor = color * vec4(lightColor, 1.0);
}