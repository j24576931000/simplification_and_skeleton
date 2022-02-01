#version 410 core

out vec4 fragColor;

in VertexData
{
	vec3 vNormal;
	vec3 vViewPos;
	vec2 vTexCoord;
} vertexIn;
uniform bool useLighting2;

uniform vec4 faceColor2;


void main(void) 
{
	vec4 newFaceColor = faceColor2;

	if (useLighting2)
	{
		vec3 viewVector = -vertexIn.vViewPos;
		vec3 lightDir = vec3(0, 0, -1);

		vec3 L = -lightDir;
		vec3 V = normalize(viewVector);
		vec3 N = normalize(vertexIn.vNormal);

		float ambient = 0.01;
		float diffuse = max(dot(N, L), 0);
		float specular = 0;
		if (diffuse > 0)
		{
			vec3 H = normalize(L + V);
			specular = pow(dot(N, H), 128);
		}

		newFaceColor = vec4(vec3(1.0) * (ambient + specular) + newFaceColor.xyz * diffuse, newFaceColor.a);
	}

	fragColor = newFaceColor;
}