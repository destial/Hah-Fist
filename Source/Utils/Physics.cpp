#include "Physics.hpp"

namespace Physics
{
	bool AABB::CanFit(AABB& a)
	{
		if (this->min.x <= a.min.x && this->max.x >= a.max.x && this->min.y <= a.min.y && this->max.y >= a.max.y)
		{
			return true;
		}
		return false;
	}

	bool AABB::Intersects(AABB& a)
	{
		if (this->min.x > a.max.x || this->max.x < a.min.x || this->min.y > a.max.y || this->max.y < a.min.y)
		{
			return false;
		}
		return true;
	}
}