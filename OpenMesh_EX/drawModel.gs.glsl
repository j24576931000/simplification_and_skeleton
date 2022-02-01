//#version 410 core
//
//layout(triangles) in;
//layout(triangle_strip, max_vertices = 3) out;
//
//in VertexData
//{
//	vec3 vNormal;
//	vec3 vViewPos;
//	vec2 vTexCoord;
//} vertexIn[];
//
//out VertexData
//{
//	vec3 vNormal;
//	vec3 vViewPos;
//	vec2 vTexCoord;
//	vec3 barycentric;
//} vertexOut;
//
//void main()
//{
//	vertexOut.vNormal = vertexIn[0].vNormal;
//	vertexOut.vViewPos = vertexIn[0].vViewPos;
//	vertexOut.vTexCoord = vertexIn[0].vTexCoord;
//	vertexOut.barycentric = vec3(1, 0, 0);
//	gl_Position = gl_in[0].gl_Position;
//	gl_PrimitiveID = gl_PrimitiveIDIn;
//	EmitVertex();
//
//	vertexOut.vNormal = vertexIn[1].vNormal;
//	vertexOut.vViewPos = vertexIn[1].vViewPos;
//	vertexOut.vTexCoord = vertexIn[1].vTexCoord;
//	vertexOut.barycentric = vec3(0, 1, 0);
//	gl_Position = gl_in[1].gl_Position;
//	gl_PrimitiveID = gl_PrimitiveIDIn;
//	EmitVertex();
//
//	vertexOut.vNormal = vertexIn[2].vNormal;
//	vertexOut.vViewPos = vertexIn[2].vViewPos;
//	vertexOut.vTexCoord = vertexIn[2].vTexCoord;
//	vertexOut.barycentric = vec3(0, 0, 1);
//	gl_Position = gl_in[2].gl_Position;
//	gl_PrimitiveID = gl_PrimitiveIDIn;
//	EmitVertex();
//
//	EndPrimitive();
//}