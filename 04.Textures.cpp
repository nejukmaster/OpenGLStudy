#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.cpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//너비, 높이, 컬러 채널의 수
	int width, height, nrChannels;
	//stbi_load는 이미지데이터를 문자열로 반환하고, 이미지의 각 정보를 지정한 주소값에 저장해줍니다.
	unsigned char* data = stbi_load("Resources/container.jpg", &width, &height, &nrChannels, 0);

	//텍스쳐의 아이디를 저장할 변수 texture 생성
	unsigned int texture1;
	//glGenTexture(텍스쳐 개수(int), 아이디를 담을 주소)
	glGenTextures(1, &texture1);

	//texture 아이디에 GL_TEXTURE_2D로 텍스쳐를 바인딩
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//로드한 이미지 뒤집기
	stbi_set_flip_vertically_on_load(true);
	//실제 이미지 데이터 data가 유효할경우 실행
	if (data) {
		//glTexImage2D는 실제 텍스쳐를 생성합니다.
		//glTexImage2D(텍스쳐 타깃, 텍스쳐의 mipmap레벨(베이스 : 0), 텍스쳐 포멧, 너비, 높이, 무조건 0, 원본 이미지의 포멧, 원본 이미지의 데이터 타입, 실제 이미지 데이터) 
		//텍스쳐의 타깃은 텍스쳐를 적용할 타깃입니다. 아래 코드에선 GL_TEXTURE_2D로 바인딩된 객체에만 적용한다는 뜻입니다.
		//MipMap에 대한 설명: https://drehzr.tistory.com/666
		//텍스쳐의 포멧을 Alpha값을 사용하지 않는 RGB채널로 설정
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	//이후 이미지 로드및 텍스쳐 생성을 위해 사용한 메모리를 반환해줍니다.
	stbi_image_free(data);

	//두번째 텍스쳐 바인딩
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("Resources/Smile.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		//두번째 텍스쳐 원본이미지의 확장자가 png이므로 7번째 파라미터에 RGBA를 넣어준다.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//텍스쳐 좌표를 추가한 사각형 버텍스 데이터 생성
	float vertices[] = {
		// 위치              // 컬러             // 텍스처 좌표
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 우측 상단
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 우측 하단
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 좌측 하단
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // 좌측 상단
	};

	unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
	0, 1, 3,   // 첫 번째 삼각형
	1, 2, 3    // 두 번째 삼각형
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Shader ourShader("04.Shader.vs", "04.Shader.fs");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//새로운 버텍스 포멧에 맞게 어트리뷰트 수정
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//텍스쳐 좌표는 2차원이므로 2번째 파라미터를 2로, 새로운 축의 데이터 이미로 location을 2로 설정
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // 직접 설정
	ourShader.setInt("texture2", 1); // 혹은 shader 클래스를 활용

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//텍스쳐 유닛을 활성화
		//GL_TEXTURE0는 기본 텍스쳐 유닛
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//두번째 텍스쳐 유닛 활성화
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}