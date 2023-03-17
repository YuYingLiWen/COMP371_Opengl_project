#pragma once

#include "GLObject.h"
#include "IBinder.h"

class ElementBuffer : public IBinder, GLObject
{
public:
	ElementBuffer(unsigned int* data, unsigned int count);
	~ElementBuffer();

	void Bind() const override;
	void Unbind() const override;
};