#ifndef BULLET_H_PROJECTILE
#define BULLET_H_PROJECTILE

#include "Projectile.h"

struct Bullet : public IProjectile
{
	Bullet()
	{
		positionVec.Zero();
		dirVec.Zero();
		force = startTime, lifeTime = 0.0f;
		isAlive = true;
	}

	bool IsAlive(){ return isAlive; }
	void SetAlive(bool _val){ isAlive = _val; }
	void ResetParams()
	{
		positionVec.Zero();
		dirVec.Zero();
		force = startTime, lifeTime = 0.0f;
		isAlive = false;
	}
};

#endif