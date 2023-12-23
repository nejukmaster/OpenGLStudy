#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    //ambient light 계산
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse light 계산
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Phong 스펙큘러 계산
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);   //reflect(A,B)는 B벡터에대한 A벡터의 반사벡터를 계산해준다. R = A+2B(-A·B)
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32/*이 값은 스펙큘러의 Smoothness입니다.*/);
    vec3 specular = specularStrength * spec * lightColor;

    //ambient light, diffuse light, specular를 적용
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}