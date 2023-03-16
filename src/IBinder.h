#pragma once

class IBinder
{
public:
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual ~IBinder();
};