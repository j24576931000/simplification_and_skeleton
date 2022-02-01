#version 410 core

out vec4 fragColor;

in VertexData
{
	vec3 vNormal;
	vec3 vViewPos;
	vec2 vTexCoord;
	//vec3 barycentric;
} vertexIn;

//uniform sampler2D texImage;
uniform bool useLighting;
//uniform bool drawWireframe;
//uniform bool drawPoint;
//uniform bool drawTexture;
uniform vec4 faceColor;
//uniform vec4 wireColor;
//uniform vec4 PointColor;
//float edgeFactor()
//{
//	vec3 d = fwidth(vertexIn.barycentric);
//	vec3 a3 = smoothstep(vec3(0.0), d * 1.5, vertexIn.barycentric);
//	return min(min(a3.x, a3.y), a3.z);
//}

void main(void) 
{
	//vec4 faceColor = vec4(vec3(1.0) * lightIntense, 1.0);
	vec4 newFaceColor = faceColor;
	/*if (drawTexture)
	{
		vec4 texColor = texture(texImage, vertexIn.vTexCoord);
		newFaceColor = texColor;
	}*/

	if (useLighting)
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

	

	//vec4 color = newFaceColor;
	////vec4 color = faceColor;
	/*if (drawWireframe)
	{
		float ef = edgeFactor();
		color = mix(wireColor, newFaceColor, ef);
	}*/
	//if (drawPoint)
	//{
	//	//float ef = edgeFactor();
	//	//color = mix(wireColor, newFaceColor, ef);
	//	//color = mix(PointColor,wireColor, newFaceColor, ef);
	//}

	fragColor = newFaceColor;
	//fragColor = color;
}