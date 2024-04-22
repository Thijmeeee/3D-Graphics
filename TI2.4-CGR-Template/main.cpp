#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

void init();
void update();
void draw();
void draw_kube();
void move_kube_left();
void move_kube_right();
void enable_wireframe();


float angle = 0.0f;
float width = 1080;
float height = 750;
float kube_x = 0.0f;
float increment_value = 0.25f;

bool ortho_perspective = false;
bool wireframe = false;


int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();

    init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


    return 0;
}


void init()
{
    glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
        else if (key == GLFW_KEY_LEFT) {
            move_kube_left();
        }
        else if (key == GLFW_KEY_RIGHT) {
            move_kube_right();
        }
        else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
            ortho_perspective = !ortho_perspective;
        }
        else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            wireframe = !wireframe;
            enable_wireframe();
        }
    });

}

void update()
{
    angle += 0.02f;
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tigl::shader->enableColor(true);
    if (ortho_perspective) {
        tigl::shader->setProjectionMatrix(glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 50.0f, -50.0f));
      
    }
    else {
        tigl::shader->setProjectionMatrix(glm::perspective(glm::radians(70.0f), width / height, 0.1f, 200.0f));
    }
    tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(0, 3, 8), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
  
    // Draw first cube and set the projection matrix
    glm::mat4 modelMatrix1 = glm::mat4(1.0f);
    modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(kube_x, 0.0f, 0.0f));
    tigl::shader->setModelMatrix(glm::rotate(modelMatrix1, glm::radians(angle), glm::vec3(0, 1.0f, 0)));
    draw_kube();

    // Draw second cube
    glm::mat4 modelMatrix2 = glm::mat4(1.0f);
    modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(-5, 0, 0));
    tigl::shader->setModelMatrix(glm::rotate(modelMatrix2, glm::radians(angle), glm::vec3(1.0f, 0, 0)));
    draw_kube();

    // Draw third cube
    glm::mat4 modelMatrix3 = glm::mat4(1.0f);
    modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(5, 0, 0));
    tigl::shader->setModelMatrix(glm::rotate(modelMatrix3, glm::radians(angle), glm::vec3(0, 0, 1.0f)));
    draw_kube();

    // Draw fourth cube
    glm::mat4 modelMatrix4 = glm::mat4(1.0f);
    modelMatrix4 = glm::translate(modelMatrix4, glm::vec3(0, 0, -20));
    tigl::shader->setModelMatrix(modelMatrix4);
    draw_kube();

  
}

void move_kube_left() {
    kube_x -= increment_value;
}

void move_kube_right() {
    kube_x += increment_value;
}

void enable_wireframe() {
    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
}


void draw_kube() {
    tigl::begin(GL_QUADS);

    // Back side (blue)
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 0, 0), glm::vec4(0, 1, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 1, 0), glm::vec4(0, 1, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 1, 0), glm::vec4(0, 1, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 0, 0), glm::vec4(0, 1, 0, 1)));

    // Front side (red)
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 0, 1), glm::vec4(1, 0, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 1, 1), glm::vec4(1, 0, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 1, 1), glm::vec4(1, 0, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 0, 1), glm::vec4(1, 0, 0, 1)));

    // Bottom side 
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 0, 0), glm::vec4(0, 0, 1, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 0, 1), glm::vec4(0, 0, 1, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 0, 1), glm::vec4(0, 0, 1, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 0, 0), glm::vec4(0, 0, 1, 1)));

    // Top side 
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 1, 0), glm::vec4(1, 0, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 1, 1), glm::vec4(1, 0, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 1, 1), glm::vec4(1, 0, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 1, 0), glm::vec4(1, 0, 0, 1)));

    // Left side 
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 0, 0), glm::vec4(0, 1, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 0, 1), glm::vec4(0, 1, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 1, 1), glm::vec4(0, 1, 0, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(0, 1, 0), glm::vec4(0, 1, 0, 1)));

    // Right side
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 0, 0), glm::vec4(0, 0, 1, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 0, 1), glm::vec4(0, 0, 1, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 1, 1), glm::vec4(0, 0, 1, 1)));
    tigl::addVertex(tigl::Vertex::PC(glm::vec3(1, 1, 0), glm::vec4(0, 0, 1, 1)));

    tigl::end();
}