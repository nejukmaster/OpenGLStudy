//디스크에서 shader를 읽고 컴파일하고 그들을 연결한 후 오류를 확인하는 shader class 생성
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // 필요한 모든 OpenGL의 헤더파일을 가져오기 위해 glad를 포함합니다.
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // program ID
    unsigned int ID;

    // 생성자는 shader를 읽고 생성합니다.
    //생성자는 인자로 버텍스 쉐이더 파일의 경로와 프래그넌트 쉐이더 파일의 경로를 받습니다.
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. 파일 경로를 통해 vertex/fragment shader 소스 코드를 검색합니다.
        //std는 c++에서 여러 함수를 모아둔 네임스페이스이고 ::는 객체의 정적함수에 접근하는 람다식입니다.
        //std::string은 빈 문자열 객체를 생성한다.
        std::string vertexCode;
        std::string fragmentCode;
        //std::ifstream은 외부파일을 읽을 바이트 버퍼 스트림 객체를 생성한다.
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        //파일이 존재하지 않을경우 ifstream객체들이 예외를 throw할수 있게 한다.
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // 파일 열기
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            // 바이트 버퍼를 문자열로 읽어올 stringstream 선언
            std::stringstream vShaderStream, fShaderStream;
            // stream에 파일의 버퍼 내용을 읽기
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // 바이트 버퍼를 닫아 파일을 읽는것을 끝냅니다.
            vShaderFile.close();
            fShaderFile.close();
            // stringstream을 문자열로 바꿔줍니다.
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        //파일을 읽지 못했을때 에러 메세지 출력
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }
        //c_str은 문자열을 char형 포인터로 나타내줍니다.(read only)
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. 읽어들인 쉐이더를 컴파일합니다.
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // program 내부에서 shader들이 링크 완료되었다면 이제 필요 없으므로 shader들을 삭제
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // shader를 활성화하고 사용합니다.
    void use()
    {
        glUseProgram(ID);
    }
    // Uniform 유틸리티 함수들
    // 해당 Uniform 변수의 값을 바꾸는 함수들입니다.
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setMat4(const std::string& name, glm::mat4 value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

private:
    // 쉐이더의 오류를 찾아 출력하는 유틸리티 함수입니다.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

#endif