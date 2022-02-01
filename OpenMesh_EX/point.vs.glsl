#version 410 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat3 um3n2;
uniform mat4 um4mv2;
uniform mat4 um4p2;
uniform mat4 um4u2;
uniform bool drawTexCoord2;
out VertexData
{
	vec3 vNormal;
	vec3 vViewPos;
	vec2 vTexCoord;
} vertexOut;

void main()
{
	vec4 pos;
	if (drawTexCoord2)
	{
		vec2 offset = vec2(-0.5, -0.5);
		pos = um4u2 * vec4(texCoord + offset, 0.0, 1.0);
	}
	else
	{
		pos = vec4(vertex, 1.0);
	}

	vec4 newTexCoord = um4u2 * vec4(texCoord, 0.0, 1.0);
	vec4 viewModelPos = um4mv2 * vec4(vertex, 1.0);
	vertexOut.vNormal = normalize(um3n2 * normal);
	vertexOut.vViewPos = viewModelPos.xyz;
	vertexOut.vTexCoord = newTexCoord.xy;
	gl_PointSize = 10;
	gl_Position = um4p2 * um4mv2 * pos;
}
