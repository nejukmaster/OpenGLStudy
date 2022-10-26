#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.cpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos; //위치 변수는 attribute position 0을 가집니다.\n"
"layout (location = 1) in vec3 aColor; //색상 변수는 attribute position 1을 가집니다.\n"
//쉐이더에서 out 형 변수는 다음 쉐이더에 이 변수의 값을 넘겨줍니다.
"out vec3 vertexColor; // fragment shader를 위한 컬러 출력을 지정\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	vertexColor = aColor;\n"
"}\0";

const char* fragmentShaderSource_ = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor; // Vertex Shader로 부터 받은 입력 변수(똑같은 이름, 똑같은 타입)\n"
"void main()\n"
"{\n"
"   FragColor = vertexColor;\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
//uniform은 Shader Program내에서 전역변수로 사용됩니다.
//또한 Shader Program외부 코드에서 접근이 가능하기도 합니다.
//중요 : uniform변수는 사용되지 않았을경우 오류를 throw한다.
"uniform vec4 ourColor;\n"
"in vec3 vertexColor;"
"void main()\n"
"{\n"
"   FragColor = vec4(vertexColor,1.0);\n"
"}\n\0";

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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices1[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	//컬러 데이터 Attribute를 추가한 정점데이터 새로 추가
	float vertices[] = {
		// 위치              // 컬러
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 우측 하단
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 좌측 하단
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 위 
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	Shader OurShader("03.Shader.vs","03.Shader.fs");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
						//location = 0에 버텍스의 위치데이터를 집어넣습니다.
						   //위치데이터는 float형 데이터 세개로 이루어져있으므로 포인터의 앞쪽 float형 세개를 읽습니다..
												//또한 데이터를 읽는 포인터의 간격을 float형 6칸으로 설정합니다. 이는 컬러 데이터까지 포함되어 버텍스데이터 하나의 크기가 float 6개로 늘었기 때문입니다.
																	//버텍스의 위치데이터는 앞쪽 세개이므로 offset을 맨 처음으로 설정합니다.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//Attribute location = 0을 사용하겠다고 알려줍니다.
	glEnableVertexAttribArray(0);
						//location = 1에 버텍스의 컬러데이터를 집어넣습니다.
						   //컬러데이터 역시 float형 데이터 3개로 이루어져있습니다.
												//데이터를 읽는 포인터의 간격은 위치데이터와 같습니다.
																	//컬러데이터는 뒤쪽 세개이므로 offset을 float형 세번째 부터 읽습니다.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//Attribute location = 1을 사용하겠다고 알려줍니다.
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glfwGetTime() : OpenGL이 실행된 시간을 초단위로 반환합니다.
		//float timeValue = glfwGetTime();
		//시간값을 컬러값으로 변환 => sin함수를 통해 0에서 1사이의 값으로 매핑
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//glGetUniformLocation(Shader Program, Uniform Name)은 Uniform Name의 uniform 변수를 Shader Program 내에서 찾아서 그 "위치"를 반환합니다.
		//값이 -1일경우 uniform변수를 찾지 못한것입니다.
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		
		OurShader.use();
		//glUniform4f(Uniform Location, v0, v2, v3, v4)는 Uniform Location 위치의 uniform변수에 float4(v0, v1, v2, v3)값을 할당합니다.
		//glUniform함수는 Shader Program의 사용을 "요구"하므로 반드시 glUseProgram함수 이후에 사용해야한다.
		//이 glUniform함수는 밑의 4f와 같은 접미사를 여럿 가진다. 전문확인 : https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml
		//따라서 다음 코드는 시간에 따라 삼각형의 초록색의 강도가 변하는 코드이다.
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


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