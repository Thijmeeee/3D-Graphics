#include "FpsCam.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

FpsCam::FpsCam(GLFWwindow* window)
{
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// if (glfwRawMouseMotionSupported())
	// 	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	move_camera = true;
}


glm::mat4 FpsCam::getMatrix()
{
	glm::mat4 ret(1.0f);
	ret = glm::rotate(ret, rotation.x, glm::vec3(1, 0, 0));
	ret = glm::rotate(ret, rotation.y, glm::vec3(0, 1, 0));
	ret = glm::translate(ret, position);
	return ret;
}

void FpsCam::move(float angle, float fac)
{
	position.x += (float)cos(rotation.y + glm::radians(angle)) * fac;
	position.z += (float)sin(rotation.y + glm::radians(angle)) * fac;
}

void FpsCam::moveY(float fac)
{
	position.y += fac;
}




void FpsCam::update(GLFWwindow* window)
{
	double x, y;

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		move_camera = !move_camera;

	if (move_camera) {
		glfwGetCursorPos(window, &x, &y);

		static double lastX = x;
		static double lastY = y;

		rotation.x -= (float)(lastY - y) / 100.0f;
		rotation.y -= (float)(lastX - x) / 100.0f;

		lastX = x;
		lastY = y;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			move(0, 0.01f);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			move(180, 0.01f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			move(90, 0.01f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			move(-90, 0.01f);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			moveY(-0.01f);
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			moveY(0.01f);
	}
	else
	{
		static double lastX = x;
		static double lastY = y;

		rotation.x -= (float)(lastY - y) / 100.0f;
		rotation.y -= (float)(lastX - x) / 100.0f;

		lastX = x;
		lastY = y;
	}

}