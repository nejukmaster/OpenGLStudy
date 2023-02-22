#version 330 core
layout (location = 0) in vec3 aPos;
//텍스쳐 좌표를 받음
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 transform;

void main()
{
	//변환을 적용
	//변환 행렬 적용은 좌표에 행렬을 곱해주면 된다.
	gl_Position = transform * vec4(aPos, 1.0);
	texCoord = vec2(aTexCoord.x,aTexCoord.y);
}