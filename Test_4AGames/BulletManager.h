#pragma once
#include <deque>
#include <vector>
#include "Globals.h"
#include "CriticalSection.h"
#include "Plane.h"
#include "Projectile.h"

class BulletManager
{
public:
	BulletManager();
	virtual ~BulletManager();

	void Update(float _dt);
	void Fire(Vector3 _pos, Vector3 _dir, float _speed, float _time, float _lifeTime);


private:
	BulletManager(const BulletManager& rhs);
	BulletManager& operator=(const BulletManager rhs);
	DEF_CRIT_OBJ
	
	//Motion and Physics
	void Integrate(Vector3& _initPos, Vector3& _dir, float _speed, float _time, float _dt);
	bool CheckLifeTime(float _curTime, float _totalTime);
	DeltaTransform  RK4Stepper(CurrentTransform &initial, float t, float dt, DeltaTransform &d);

	std::deque<IProjectile*> mBulletsList;
	std::deque<IProjectile*> mBulletsPoolList;

	std::vector<Plane> mWallsList;

};

