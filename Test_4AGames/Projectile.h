#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Vector.h"

struct CurrentTransform
{
	Vector3 position;
	Vector3 velocity;
};

struct DeltaTransform
{
	Vector3 positionDx;
	Vector3 velocityDy;
};

class IProjectile
{
public:
	Vector3 positionVec;
	Vector3 dirVec;
	float force, startTime, lifeTime;
	bool isAlive;
};


#endif