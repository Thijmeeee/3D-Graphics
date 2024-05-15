#pragma once

#include <gl/glew.h>
#include <string>
#include <iostream>

class Texture
{
	GLuint id;
public:
	Texture(const std::string& fileName);
	void bind();
	void unbind();
};