#pragma once

#include "GLObject.h"
#include "IBinder.h"

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>


class ElementBuffer : public IBinder, GLObject
{
private:
	unsigned int count;
public:
	ElementBuffer(unsigned int* data, unsigned int count);
	~ElementBuffer();

	void Bind() const override;
	void Unbind() const override;

	unsigned int GetCount() const;
};