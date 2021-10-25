#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertShFile;
    std::ifstream fragShFile;
    // setting exceptions flags :
    vertShFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragShFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        
        vertShFile.open(vertexPath);
        fragShFile.open(fragmentPath);
        std::stringstream vertShStream, fragShStream;
        
        vertShStream << vertShFile.rdbuf();
        fragShStream << fragShFile.rdbuf();		
        
        vertShFile.close();
        fragShFile.close();
        
        vertexCode   = vertShStream.str();
        fragmentCode = fragShStream.str();		
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vertShCode = vertexCode.c_str();
    const char* fragShCode = fragmentCode.c_str();
    unsigned int vertex, fragment;
    vertex = createCompileShader(vertexType, vertShCode);
    fragment = createCompileShader(fragmentType, fragShCode);
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, programType);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

unsigned int Shader::createCompileShader(ShaderType type, const char* source) {
    unsigned int shader;

    if(type == vertexType) shader = glCreateShader(GL_VERTEX_SHADER);
    if(type == fragmentType) shader = glCreateShader(GL_FRAGMENT_SHADER);
    if(type != fragmentType && type != vertexType) throw -1;

    glShaderSource(shader, /* how many strings ? */1, &source, nullptr);
    
    // compiling   
    glCompileShader(shader);
    checkCompileErrors(shader, type);

    return shader;
}

void Shader::checkCompileErrors(unsigned int shader, ShaderType type) {
    int success;
    char infoLog[1024];
    if (type != programType) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            char errorLog[] { "ERROR::SHADER::COMPILATION_FAILED\n" };
            std::cout << errorLog << std::endl << infoLog << std::endl;
            throw -1;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            char errorLog[] { "ERROR::PROGRAM::LINKING_FAILED\n" };
            std::cout << errorLog << std::endl << infoLog << std::endl;
            throw -1;
        }
    }
}