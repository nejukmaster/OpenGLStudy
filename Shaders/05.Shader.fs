#version 330 core

out vec4 FragColor;

in vec4 ourColor;
in vec2 texCoord;

//텍스쳐를 받는 Sampler2D형 uniform변수를 선언
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //mix함수는 두 텍스쳐를 선형보간으로 섞는다.
    //mix(texture1,texture2,선형보간계수)
    FragColor = mix(texture(texture1, texCoord),texture(texture2, texCoord),0.2);
}