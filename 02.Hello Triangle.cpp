#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//아래는 간단한 GLSL 쉐이더 코드이다.
//대부분의 OpenGL에서는 vertex shader와 fregnant shader를 설정하길 요구하므로간단한 쉐이더를 선언해서 랜더링을 진행할 예정이다
//이 GLSL에 대해서는 다음장에 더 다뤄볼 예정이다.
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//fragment shader코드
//fragment shader는 각 픽셀의 컬러값을 지정해주는 shader이다.
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

	//각 버텍스의 좌표를 선언
	//3차원공간이므로 앞에서부터 세개씩 끊어서 하나의 버텍스이다.
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	//정점 데이터가 정의되면 우리는 그 데이터를 그래픽 파이프라인의 첫 번째 단계인 vertex shader에게 전달합니다.
	//이 정점데이터를 그래픽카드의 메모리에 할당하고 OpenGL이 이 메모리를 어떻게 읽어서 그래픽카드에 전달할건지 명시한 후 vertex shader에 전달하게 됩니다.
	//vertex shader는 우리가 말한 만큼의 정점들을 메모리에서 처리합니다.
	//밑에서 정의할 VBO는 이 메모리를 관리하는 OpenGL의 버퍼 객체입니다.
	unsigned int VBO;
	//glGenBuffers는 버퍼의 아이디를 생성한다.
	////**중요: 버퍼를 생성하는게 아니라 버퍼의 아이디를 생성하여 VBO라는 함수에 담는다.
	glGenBuffers(1, &VBO);
	//glBindBuffer은 이전에 생성한 버퍼 아이디에 버퍼를 바인드합니다.
	//이때 GL_ARRAY_BUFFER은 vertex buffer object(vertex shader에 정점 데이터를 전달할 버퍼)의 type이다
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData함수는 미리 정의해놓은 정점 데이터를 버퍼의 메모리에 복사해준다.
	//이때 버퍼의 사용할 메모리 공간을 지정해주어야 하므로 sizeof(vertices)가 들어간 것을 볼 수 있다.
	//마지막 파라미터는 그래픽카드가 데이터를 관리하는 법을 지정한다.
	/*	GL_STATIC_DRAW: 데이터가 거의 변하지 않습니다.
	*	GL_DYNAMIC_DRAW: 데이터가 자주 변경됩니다.
	*	GL_STREAM_DRAW: 데이터가 그려질때마다 변경됩니다.	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//vertex shader 객체를 저장할 vertexShader선언
	unsigned int vertexShader;
	//glCreateShader는 지정한 shader유형의 shader를 생성하여 반환합니다.
	//우리는 vertex shader를 생성할 것이므로 유형을 GL_VERTEX_SHADER로 설정합니다.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//glShaderSource는 shader를 지정하여 소스코드를 첨부합니다.
	//파라미터로 1.쉐이더를 받고, 2.소스의 문자열수, 즉 소스코드의 갯수를 받으며, 3.실제 문자열을 받습니다.
	//마지막 파라미터는 담?에 나올듯?
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//이후 glShaderCompile은 첨부한 소스코드를 GLSL로 컴파일 합니다.
	glCompileShader(vertexShader);

	//컴파일 성공/실패를 저장할 success변수와 컴파일 로그를 저장할 infoLog 문자열 변수를 선언
	int  success;
	char infoLog[512];
	//glGetShaderiv는 컴파일 결과를 반환한다.
	//glGetShaderiv에 GL_COMPILE_STATUS를 넘기면 마지막 파라미터에 vertexShader의 컴파일 결과를 저장합니다.
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//성공이 아닐때
	if (!success)
	{
		//glGetShaderInfoLog은 지정한 쉐이더의 로그를 마지막 파라미터에 저장합니다.
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		//출력
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//fragment shader 객체를 저장할 변수 선언
	unsigned int fragmentShader;
	//GL_FRAGMENT_SHADER타입(fragment shader)으로 glCreateShader를 호출하여 fragmentShader에 반환값을 저장
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//fragment shader에 fragment shader 소스코드 첨부
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//shader 컴파일
	glCompileShader(fragmentShader);

	//이후, 앞서 생성한 쉐이더들을 shader program로 연결해줍니다.
	//이렇게 리스트처럼 연결된 shader들은 한 shader의 출력값이 다음 shader의 입력값이 되는 형태로 랜더링합니다.
	//이 입력값/출력값 형태가 동일하지 않을 시, 오류를 발생시킵니다.
	//shader program 객체를 저장할 변수 선언
	unsigned int shaderProgram;
	//shader program을 생성후 저장
	shaderProgram = glCreateProgram();

	//glAttachShader는 shader program에 shader을 첨부합니다.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//이후 glLinkProgram은 shader program에 첨부된 shader를 첨부한 순서대로 연결합니다.
	glLinkProgram(shaderProgram);
	//shader program도 shader와 동일하게 연결시 에러등을 확인할 수 있습니다.
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//이후 glUseProgram을 통해 shader program을 활성화합니다.
	glUseProgram(shaderProgram);
	//또한, glDeleteShader를 통해 프로그램에 성공적으로 링크된 shader객체는 shader program에서 삭제해줍니다.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//이후 VAO(Vertex Array Object)를 생성합니다.
	//VAO는 vertex 속성 포인터를 구성할 때 오직 한 번 호출하기만 하면 되고 오브젝트를 그려야 할 때마다 해당 VAO를 바인딩 하기만 하면 된다는 장점
	//Core OpenGL은 정점 입력과 관련하여 VAO를 사용하도록 요구합니다. VAO를 바인딩하는데에 실패한다면 OpenGL은 어떤것이든 그리기를 거부할지도 모릅니다.
	//VAO의 아이디를 저장할 변수 선언
	unsigned int VAO;
	//VAO의 아이디를 생성하여 VAO변수에 담는다.
	glGenVertexArrays(1, &VAO);

	//Vertex Array Object 바인딩
	glBindVertexArray(VAO);

	//glVertexAttribPointer함수는 OpenGL에게 vertex데이터를 어떻게 해석해야하는지 알려줍니다.
	//위의 vertexShader코드에서 layout(location = 0)으로 설정했으므로 이 location값을 첫번째 파라미터에 넣어준다.
	//이후 두번째 파라미터에선 vertex속성의 크기를 받는다. 이 vertex는 3차원 vertex이므로 인자로 3을 넘겨준다.
	//세번째로 데이터타입을 넘긴다. 여기선 실수형 float을 넘겼다.
	//이후 네번째 파라미터는 데이터의 정규화 여부를 받는다. GL_TRUE로 설정할경우 vertex데이터가 0-1사이의 값으로 매핑된다.
	//다섯번째 파라미터는 vertex데이터를 읽는 간격을 지정한다. 즉 포인터가 한번에 얼만큼의 데이터를 읽어 들일지를 정한다.
	//마지막 파라미터는 데이터의 시작점을 표기합니다. 여기서 우리의 데이터는 배열 시작부분부터 존재하므로 (void*)0을 넘겨주었습니다. 이 파라미터에 대해선 이후 자세히 다룰예정입니다.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray 함수의 파라미터로 vertex 속성 location를 전달
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
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