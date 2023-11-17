#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out Surface{
	vec2 UV;
	vec3 worldPos;
	vec3 worldNormal;
}vs_out;

uniform mat4 _Model;
uniform mat4 _ViewProjection;

void main(){
	vs_out.UV = vUV;

	// converts vertex position to world space
	vec4 tempPos = _Model * vec4(vPos, 1.0);
	vs_out.worldPos = tempPos.xyz;

	// converts vertex normal to world space
	vs_out.worldNormal = transpose(inverse(mat3(_Model))) * vNormal;

	gl_Position = _ViewProjection * _Model * vec4(vPos,1.0);
}