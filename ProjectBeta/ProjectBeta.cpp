#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


unsigned int width = 1280;
unsigned int height = 720;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Vertex и Fragment шейдеры
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main()
{
    // Инициализация glfw/OpenGl
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Инициализация окна просмотра (window)
    GLFWwindow* window = glfwCreateWindow(width, height, " ProjectBeta!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Создание контекста OpenGl (window)
    glfwMakeContextCurrent(window);
    // Поллучение размера окна после изминения и отправка их в glViewport
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Проверка инициализации GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Компиляция Vertex и Fragment шейдеров
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
    glCompileShader(vertexShader); 

    int  success_VERTEX_SHADER;
    char infoLog_VERTEX_SHADER[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_VERTEX_SHADER);

    unsigned int fragmentShader; 
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); 
    glCompileShader(fragmentShader); 

    int success_FRAGMENT_SHADER;
    char infoLog_FRAGMENT_SHADER[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_FRAGMENT_SHADER); 

    if (!success_VERTEX_SHADER) 
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog_VERTEX_SHADER);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog_VERTEX_SHADER << std::endl; 
    }

    if (!success_FRAGMENT_SHADER)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog_FRAGMENT_SHADER);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog_FRAGMENT_SHADER << std::endl;
    }

    // Создания и линкование шейдеров к шейдерной програме
    unsigned int shaderProgram; 
    shaderProgram = glCreateProgram(); 
    glAttachShader(shaderProgram, vertexShader); 
    glAttachShader(shaderProgram, fragmentShader); 
    glLinkProgram(shaderProgram); 

    // Проверка линкования шейдерной програмы
    int success_shaderProgram;
    char infoLog_shaderProgram[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_shaderProgram);

    if (!success_shaderProgram) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog_shaderProgram);
        std::cout << "ERROR::SHADER::PROGRAM::LINKED_FAILED\n" << infoLog_shaderProgram << std::endl;
    }

    // Удаление шейдерных обектов 
    glDeleteShader(vertexShader); 
    glDeleteShader(fragmentShader); 

    // Вершины триугольника
    /*float vertices[] = {
         0.5f,  0.5f, 0.0f,  
         0.5f, -0.5f, 0.0f, 
        -0.5f,  0.5f, 0.0f,

         0.5f, -0.5f, 0.0f,  
        -0.5f, -0.5f, 0.0f,  
        -0.5f,  0.5f, 0.0f   
    };*/

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // верхняя правая
        0.5f, -0.5f, 0.0f,  // нижняя правая
       -0.5f, -0.5f, 0.0f,  // нижняя левая
       -0.5f,  0.5f, 0.0f   // верхняя левая 
    };

    unsigned int indices[] = {  // помните, что мы начинаем с 0!
        0, 1, 3, // первый треугольник
        1, 2, 3  // второй треугольник
    };

    unsigned int VBO, VAO, EBO; 
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);  
    glGenBuffers(1, &EBO);    

    // Связываем объект вершинного массива
    glBindVertexArray(VAO);  

    // Копируем наш массив вершин в буфер
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // Связывание атрибутов вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); 

    //glBindBuffer(GL_ARRAY_BUFFER, 0);  
    //glBindVertexArray(0);   

    // Отрисовка полигонов в режиме каркаса
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  

    // Окно цыкла рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Поллучение данных ввода от пользователя
        processInput(window);

        // Очистка экранна цветом 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT); 

        // Рисуем объект
        glUseProgram(shaderProgram); 
        glBindVertexArray(VAO); 
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
        glBindVertexArray(0); 

        // Изменения front буффера на beck буффер и получение данных от ввода пользователя
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Удаление данных OpenGl и закрытие программы
    glDeleteVertexArrays(1, &VAO); 
    glDeleteBuffers(1, &VBO); 

    glfwTerminate();
    return 0;
}

// Поллучение данных ввода от пользователя
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Поллучение размера окна после изминения и отправка их в glViewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "New size window: x = " << width << " y = " << height << " Summ pix: " << width * height << std::endl;
}