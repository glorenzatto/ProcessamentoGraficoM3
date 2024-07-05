#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"
#include "Game.h"

int main()
{
    // Inicialização da GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Criação da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Jogo das cores", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwMakeContextCurrent(window);

    // Inicialização do GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Registro de callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Informações da versão
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;

    // Definindo dimensões da viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Compilação e build do shader
    Shader shader("Shaders/retangulo.vs", "Shaders/retangulo.fs");

    // Configuração do buffer
    GLuint VAO = setup();

    initRandomColors(colorMatrix);

    // Inicializa o título da janela
    std::string initialTitle = "Gustavo Lorenzatto Cauduro - M3 P.G. | Jogo das cores | Cliques Restantes: " + std::to_string(remainingClicks) + " | Pontuacao: " + std::to_string(score);
    glfwSetWindowTitle(window, initialTitle.c_str());

    // Loop principal da aplicação
    while (!glfwWindowShouldClose(window))
    {
        // Verifica se o jogo acabou
        if (remainingClicks <= 0) {
            std::cout << "Pontos realizados: " << score << std::endl;
            break;
        }

        if (colorsInScreen <= 0) {
            initRandomColors(colorMatrix);
            colorsInScreen = COLUMNS * LINES;
        }

        // Processamento de eventos de input
        glfwPollEvents();

        // Limpeza do buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO); // Conectando ao buffer de geometria

        shader.Use();
        GLfloat xc = -0.77f, xl = 0.90f;
        for (int c = 0; c < COLUMNS; c++) {
            for (int l = 0; l < LINES; l++) {
                shader.setVec3("cor", colorMatrix[c][l].r, colorMatrix[c][l].g, colorMatrix[c][l].b);

                glm::mat4 model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(xc + c * 0.385, xl - l * 0.125, 0));
                model = glm::scale(model, glm::vec3(0.38, 0.38, 1));
                shader.setMat4("model", glm::value_ptr(model));

                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }

        glBindVertexArray(0); // Desconectando o buffer de geometria

        // Troca dos buffers da tela
        glfwSwapBuffers(window);
    }

    // Desalocação dos buffers e finalização da GLFW
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}
