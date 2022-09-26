#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	//GLFW를 초기화
	glfwInit();
	//glfwWindowHint는 glfw를 설정한다.
	//OpenGL 3.3을 사용하므로 GLFW_CONTEXT_VERISION_MAJOR을 3으로(3.X를 의미),
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//GLFW_CONTEXT_VERSION_MINOR를 3으로 설정( X.3을 의미 )
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//GLFW_OPENGL_PROFILE을 CORE_PROFILE로 설정
	//Core Profile에 대한 설명 : https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=cjdeka3123&logNo=220947660369
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Mac 환경에선 아래의 코드를 추가
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//GLFWwindow 객체 생성
	//glfwCreateWindow는 창 크기를 지정하면 설정한 glfw대로 창을 생성하고 그 객체를 반환한다.
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		//std::cout : std클래스의 cout정적함수를 사용
		//<< : 출력 연산자
		//std::endl : std의 endl정적함수를 사용
		//cout는 c의 printf와 같은 역할
		//endl은 콘솔창에서 줄바꿈을 해준다.
		std::cout << "Failed to create GLFW window" << std::endl;
		//glfwTerminate함수는 OpenGL을 사용하며 할당된 메모리를 해제해준다.
		//따라서 프로그래 종료전에 반드시 호출해 주어야 한다.
		glfwTerminate();
		return -1;
	}
	//glfwMakeContextCurrent는 window의 컨텍스트를 현재 스레드의 주 컨텍스트로 지정하겠다고 GLFW에게 알려준다.
	glfwMakeContextCurrent(window);

	//GLAD는 OpenGL을 이용하면서 번거로울수도 있는 작업을 처리해준다.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//랜더링 윈도우의 사이즈를 설정합니다.
	glViewport(0, 0, 800, 600);
	//창의 크기가 변경될때마다 호출될 콜백함수를 OpenGl에 등록
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glfwWIndowShouldClose (window* window)는 window가 종료되었는지 확인합니다.
	//glfwPollEvents는 키보드 입력이나 마우스 이동등의 이벤트를 감지하여 윈도우의 상태를 업데이트하고 콜백함수를 통해 등록된 정해진 함수를 호출해줍니다.
	//glfwSwapBuffer는 윈도우에 그려진 컬러버퍼를 업데이트 합니다. 즉, 윈도우에 오브젝트들의 업데이트 사항을 표기합니다.
	while (!glfwWindowShouldClose(window))
	{
		//입력 감지
		processInput(window);

		//OpenGL의 랜더링루프에서는 매 루프마다 윈도우의 컬러버퍼를 지워줘야한다.
		//그렇지 않으면 이전 루프의 버퍼가 화면에 남기 때문이다.
		//따라서 우리는 glClear함수를 통해 버퍼를 지워줘야한다.(현재는 컬러버퍼만 생각하므로 컬러버퍼의 비트만 지웠다.)
		//glClearColor는 버퍼를 지울때, 즉 초기화할때 어떤 색깔로 초기화 할지 정할 수 있다. 아래는 청록색으로 초기화하는 샘플 코드다.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

//사용자가 키보드 입력을 줬을때 호출될 콜백함수를 정의
void processInput(GLFWwindow* window)
{
	//glfwGetKey(window* window, keyCode)는 window에서 keyCode의 Key가 눌러져있는지 여부를 리턴합니다.
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		//glfwSetWindowShouldClose(window* window, bool b)은 window의 glfwWindowShouldClose의 반환값을 b로 설정합니다.
		glfwSetWindowShouldClose(window, true);
}

//사용자가 창의 크기를 조정할 경우 그에 맞춰 랜더링 윈도우의 크기 역시 조절되어야하므로
//이를 수행할, 사용자가 창의 크기를 조절할 때마다 호출될 콜백함수를 정의
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}