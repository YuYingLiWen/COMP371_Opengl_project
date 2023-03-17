#pragma once

class GLObject
{
protected:
	unsigned int id{};
protected:
	GLObject();
	virtual ~GLObject();

public:
	unsigned int GetID() const;


};