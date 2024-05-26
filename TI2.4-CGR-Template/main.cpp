#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpsCam.h"
#include "Texture.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "ObjectModel.h"
#include "stb_image.h"
//#include "imgui.h"
//#include "backends/imgui_impl_glfw.h"
//#include "backends/imgui_impl_opengl3.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

using tigl::Vertex;
GLFWwindow* window;

void init();
void update();
void draw();
void draw_kube(int index, int x, int z);

const int SIZE_X = 10;
const int SIZE_Y = 20;
const int SIZE_Z = 4;

int array_blocks[SIZE_X][SIZE_Y][SIZE_Z];

float rotation = 0.0f;
float yposition_steve = 0.0f;
float yposition_increment = 0.005f;

FpsCam* camera;

int main(void)
{
	if (!glfwInit())
		throw "Could not initialize glwf";
	window = glfwCreateWindow(1900, 1050, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	//// Setup Dear ImGui style
	//ImGui::StyleColorsDark();

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();

		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		//ImGui::SetNextWindowSize(ImVec2(750.0f, 450.0f));

		//if (ImGui::Begin("Steve customization"))
		//{
		//	ImGui::SliderFloat("Rotation", &rotation, 0.0f, 360.0f);

		//	ImGui::SetWindowFontScale(1.5f);
		//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		//	ImGui::End();
		//}


		//ImGui::Render();
		//ImGui::EndFrame();

		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	}

	/*ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();*/
	

	glfwTerminate();

	return 0;
}

void init()
{
	int value[10];

	//ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback = true will install GLFW callbacks and chain to existing ones.
	//ImGui_ImplOpenGL3_Init();

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});
	camera = new FpsCam(window);
}

void update()
{
	camera->update(window);
	if (yposition_steve >= 5.0f && yposition_increment > 0.0f)
	{
		yposition_increment = -yposition_increment;
	}
	else if (yposition_steve <= 0.0f && yposition_increment < 0.0f)
	{
		yposition_increment = -yposition_increment;
	}

	yposition_steve += yposition_increment;
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 100.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(camera->getMatrix());
	tigl::shader->setModelMatrix(glm::mat4(1.0f));

	tigl::shader->enableColor(true);
	tigl::shader->enableTexture(true);

	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i <= SIZE_X; i++) {
		for (int j = 0; j <= SIZE_Z; j++) {
			draw_kube(2, i, j);
		}
	}

	tigl::shader->enableTexture(false);
}

void draw_kube(int index, int x, int z) {
	if (index == -1) {
		return;
	}

	int columnsInAtlas = 256 / 16;
	int column = index % columnsInAtlas;
	int row = index / columnsInAtlas;

	float u2 = static_cast<float>(column * 16 + 1) / 256;
	float v2 = static_cast<float>(row * 16 + 1) / 256;
	float u1 = static_cast<float>((column + 1) * 16 - 1) / 256;
	float v1 = static_cast<float>((row + 1) * 16 - 1) / 256;

	tigl::begin(GL_QUADS);

	// Achterkant
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 0, z), glm::vec2(u2, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 1, z), glm::vec2(u2, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 1, z), glm::vec2(u1, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 0, z), glm::vec2(u1, v1)));

	// Voorkant
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 0, z + 1), glm::vec2(u1, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 1, z + 1), glm::vec2(u1, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 1, z + 1), glm::vec2(u2, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 0, z + 1), glm::vec2(u2, v1)));

	// Onderkant
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 0, z), glm::vec2(u1, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 0, z + 1), glm::vec2(u1, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 0, z + 1), glm::vec2(u2, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 0, z), glm::vec2(u2, v2)));

	// Bovenkant
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 1, z), glm::vec2(u1, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 1, z + 1), glm::vec2(u1, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 1, z + 1), glm::vec2(u2, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 1, z), glm::vec2(u2, v1)));

	// Linkerzijde
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 0, z), glm::vec2(u1, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 0, z + 1), glm::vec2(u2, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 1, z + 1), glm::vec2(u2, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x, 1, z), glm::vec2(u1, v2)));

	// Rechterzijde
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 0, z), glm::vec2(u2, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 0, z + 1), glm::vec2(u1, v1)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 1, z + 1), glm::vec2(u1, v2)));
	tigl::addVertex(tigl::Vertex::PT(glm::vec3(x + 1, 1, z), glm::vec2(u2, v2)));

	tigl::end();
}


