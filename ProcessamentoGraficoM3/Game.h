#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

const GLuint WIDTH = 1024, HEIGHT = 768;
const int COLUMNS = 5, LINES = 11;
const float SIMILARITY_COLORS = 0.05f;

extern int colorsInScreen;
extern int points;
extern int remainingClicks;
extern int score;

extern glm::vec3 colorMatrix[COLUMNS][LINES];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
int setup();
void pickColor(GLdouble xpos, GLdouble ypos);
void initRandomColors(glm::vec3 colorMatrix[COLUMNS][LINES]);

#endif
