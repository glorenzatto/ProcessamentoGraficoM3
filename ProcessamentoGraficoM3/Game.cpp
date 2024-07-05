#include "Game.h"
#include <random>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int colorsInScreen = 0;
int remainingClicks = 20;
int score = 0;

glm::vec3 colorMatrix[COLUMNS][LINES];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    GLdouble xpos, ypos;
    int w, h;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && remainingClicks > 0) {
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &w, &h);
        int previousColorsInScreen = colorsInScreen;
        pickColor(xpos, h - ypos); // cor do pixel

        int removedRectangles = previousColorsInScreen - colorsInScreen;

        if (removedRectangles > 0) {
            score += removedRectangles;
            remainingClicks--;
        }

        // Atualizar o título da janela
        std::string title = "Gustavo Lorenzatto Cauduro - M3 P.G. | Jogo das cores | Cliques Restantes: " + std::to_string(remainingClicks) + " | Pontuacao: " + std::to_string(score);
        glfwSetWindowTitle(window, title.c_str());
    }
}

int setup()
{
    GLfloat vertices[] = {
        -0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  // Vértice retângulo
        -0.5f, 0.15f, 0.0f, 0.0f, 0.0f, 1.0f,   // Vértice retângulo
        0.5f, -0.15f, 0.0f, 0.0f, 0.0f, 1.0f,   // Vértice retângulo
        0.5f, 0.15f, 0.0f, 0.0f, 0.0f, 1.0f,    // Vértice retângulo 
    };

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Atributo 0 - posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1 - cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Desvincula o VAO
    glBindVertexArray(0);

    return VAO;
}

void pickColor(GLdouble xpos, GLdouble ypos) {
    float pixel[4];

    glReadPixels(xpos, ypos, 1, 1, GL_RGBA, GL_FLOAT, &pixel);

    if (pixel[0] == 0.0f && pixel[1] == 0.0f && pixel[2] == 0.0f)
        return;

    for (int c = 0; c < COLUMNS; ++c) {
        for (int l = 0; l < LINES; ++l) {
            if (colorMatrix[c][l].r == 0.0f && colorMatrix[c][l].g == 0.0f && colorMatrix[c][l].b == 0.0f)
                continue;

            if ((0.3 * ((colorMatrix[c][l].r - pixel[0]) * (colorMatrix[c][l].r - pixel[0]))) +
                (0.59 * ((colorMatrix[c][l].g - pixel[1]) * (colorMatrix[c][l].g - pixel[1]))) +
                (0.11 * ((colorMatrix[c][l].b - pixel[2]) * (colorMatrix[c][l].b - pixel[1]))) < SIMILARITY_COLORS) {
                colorMatrix[c][l] = glm::vec3(0.0f, 0.0f, 0.0f);
                colorsInScreen--;
            }
        }
    }
}

void initRandomColors(glm::vec3 colorMatrix[COLUMNS][LINES]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int c = 0; c < COLUMNS; ++c) {
        for (int l = 0; l < LINES; ++l) {
            float r = dis(gen);
            float g = dis(gen);
            float b = dis(gen);
            colorMatrix[c][l] = glm::vec3(r, g, b);
        }
    }
}
