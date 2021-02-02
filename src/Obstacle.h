#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include "DisplayObject.h"
class Obstacle final: public DisplayObject
{
public:
	Obstacle();
	~Obstacle();
	// life cycle function
	void draw() override;
	void update() override;
	void clean() override;



};

#endif // !__OBSTACLE__
