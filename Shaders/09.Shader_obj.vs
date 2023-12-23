#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    //오브젝트 공간의 노멀을 월드공간 노멀로 변환합니다.
    //이때 노멀은 방향벡터이고, 여기에 Model Matrix를 그대로 곱하게되면 노멀벡터가 왜곡될 수 있으므로.
    //법선벡터에 맞춰진 Model Matrix를 사용합니다.
    //이 새로운 매트릭스 M'은 (M^-1)^T의 좌측 상단 3x3부분으로 계산됩니다. 
    //증명 : https://stackoverflow.com/questions/13654401/why-transform-normals-with-the-transpose-of-the-inverse-of-the-modelview-matrix
    Normal = mat3(transpose(inverse(model))) * aNormal;     //transpose는 주어진행렬의 전치 행렬을, inverse는 주어진 행렬의 역행렬을 반환합니다.
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}