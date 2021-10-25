#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

enum ShaderType {
    vertex,
    fragment
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key , int scancode, int action, int mods);
unsigned int create_compile_shader(ShaderType type, const char* source);

/*float vertices[] = {
     0.5f,  0.5f, 0.0f, //0
     0.5f, -0.5f, 0.0f, //1
    -0.5f, -0.5f, 0.0f, //2
    -0.5f,  0.5f, 0.0f  //3
};*/
// I am currently using the not optimized way around
// to check my knowledge !
float vertices[] = {
    // first triangle
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f,  0.5f, 0.0f,  // top left 
    // second triangle
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
}; 

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main() {

    // initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // added for researches
    //glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // added end
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw "Failed to create GLFW Window\n";
    }

    glfwMakeContextCurrent(window);

    if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
        throw "Failed to initialize GLAD\n";
    }  

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // our vertex shader
    unsigned int vertexShader = create_compile_shader(vertex, vertexShaderSource);

    // our fragment shader
    unsigned int fragmentShader = create_compile_shader(fragment, fragmentShaderSource);

    // the shader program
    unsigned int shaderProgram;

    // if we get here;
    // shaders compiled, no worries
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    // linking
    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if(success == false) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        char errorLog[] { "ERROR::PROGRAM::LINKING_FAILED\n" };
        strncat(errorLog, infoLog, 512);
        throw errorLog;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // our VBO, VAO
    unsigned int VAO, VBO, EBO;
    // generation
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // binding
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // mapping
    // the current buffer data is set to the latest bound VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 0 refers to the `layout (location = 0)` attribute in both
    // glVertexAttribPointer and glEnableVertexAttribArray
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // release
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // not necessary, in general, don't do this

    while(!glfwWindowShouldClose(window)) {
        //processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // the process of drawing
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // draw two triangles overlapping for the experiment !!!
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, 800, 600);
}

void key_callback(GLFWwindow* window, int key , int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

unsigned int create_compile_shader(ShaderType type, const char* source) {
    unsigned int shader;
    shader = (type == vertex) ? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, /* how many strings ? */1, &source, nullptr);
    
    // compiling   
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(success == false) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        char errorLog[] { "ERROR::SHADER::COMPILATION_FAILED\n" };
        strncat(errorLog, infoLog, 512);
        throw errorLog;
    }

    return shader;
}