#pragma once

#include "IBinder.h"

class VertexBuffer : public IBinder
{
private:
	unsigned int renderer_id;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const override;
	void Unbind() const override;

};