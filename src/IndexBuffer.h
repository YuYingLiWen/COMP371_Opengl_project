#pragma once

#include "IBinder.h"

class IndexBuffer : public IBinder
{
private:
	unsigned int renderer_id;
	unsigned int count;
public:
	IndexBuffer(unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const override;
	void Unbind() const override;

	unsigned int GetCount();
};