#include "stdafx.h"
#include "BulletManager.h"
#include "Bullet.h"


BulletManager::BulletManager()
{
	float bulletSpeed = 2.0f;
	//Create a pool of bullets;
	for (int i = 0; i < 100; ++i)
	{
		mBulletsPoolList.emplace_back(new Bullet());
	}

	//Create a list of walls;
	for (int i = 0; i < 20; ++i)
	{
		Plane mTempPlane(Vector3(i*10.0f, 1*20.0f, 1*30.0f), Vector3(i*1.0f, i+1.0f, i+2.0f));
		
		mWallsList.push_back(mTempPlane);
	}

	//Fire some bullets
	INIT_CRITICAL_SEC(CRITICAL_SEC_OBJ)
	Vector3 pos, dir;
	for (int i = 0; i < 90; ++i)
	{
		pos.Set(i*1.0f, i*1.0f, i*1.0f);
		dir.Set(-i*1.0f, i*1.0f, -i*1.0f);
		Fire(pos, dir, bulletSpeed+i*1, 0, 10.0f);
		//Sleep(500);
	}
	DEL_CRITICAL_SEC(CRITICAL_SEC_OBJ)
}

BulletManager::~BulletManager()
{
	//Clear the containers.

	std::deque<IProjectile*>::const_iterator iter = mBulletsList.begin();
	while (iter != mBulletsList.end())
	{
		delete (*iter);
		iter++;
	}
	mBulletsList.clear();

	iter = mBulletsPoolList.begin();
	while (iter != mBulletsPoolList.end())
	{
		delete (*iter);
		iter++;
	}
	mBulletsPoolList.clear();

	mWallsList.clear();
}

static int count = 0;
void BulletManager::Update(float _dt)
{
#ifdef _DEBUG
	char charBuff[500];
	count = 0;
#endif
	//Iterate all bullets and integrate.
	std::deque<IProjectile*>::iterator iterCur = mBulletsList.begin();
	std::deque<IProjectile*>::iterator iterEnd = mBulletsList.end();
	for (; iterCur != iterEnd; ++iterCur)
	{
#ifdef _DEBUG
		count++;
#endif
		//Update the bullet simulation	
		Bullet* bullet = ((Bullet*)*iterCur);
		Integrate(bullet->positionVec, bullet->dirVec, bullet->force, bullet->startTime, _dt);
		bullet->startTime += _dt;

		//Check life before collision test.
		if (CheckLifeTime(bullet->startTime, bullet->lifeTime))
		{
#ifdef _DEBUG
			sprintf_s(charBuff, "\nBullet-> %d died.\n", count);
			DEBUGOUT(charBuff);
#endif
			bullet->ResetParams();
			continue;
		}
		/*
		//TODO:
		Instead of checking every bullet against everywall, We could implement some
		3D partitioning technique to reduce the iteration of bullet/wall collision with each other.
		*/
		std::vector<Plane>::iterator iterPlaneCur = mWallsList.begin();
		std::vector<Plane>::iterator iterPlaneEnd = mWallsList.end();
		for (; iterPlaneCur != iterPlaneEnd; ++iterPlaneCur)
		{
			Plane tempWall = (*iterPlaneCur);
			float planeBulletDistance = Plane::PointAndPlaneDistance(tempWall, bullet->positionVec);

#ifdef _DEBUG
			//sprintf_s(charBuff, "\nBullet-> %d\nbullet X-> %f\nbullet Y-> %f\nbullet time-> %f\ndistance-> %f\ndt-> %f \n============================", count, bullet->positionVec.x, bullet->positionVec.y, bullet->startTime, planeBulletDistance, _dt);
			//sprintf_s(charBuff, "\n\n ( %f , %f , %f ) and bulletTime-> %f and bulletDistance-> %f\n==================================================", bullet->positionVec.x, bullet->positionVec.y, bullet->positionVec.z, bullet->startTime, planeBulletDistance);
			//DEBUGOUT(charBuff);
#endif
			//If distance has reached threshold reflect the bullet
			if (planeBulletDistance < 0.01f)
			{
#ifdef _DEBUG
				sprintf_s(charBuff, "\nBullet-> %d collided with a wall\n", count);
				DEBUGOUT(charBuff);
#endif
				Vector3 reflectionVec = bullet->dirVec.ReflectionVector(tempWall.GetNormal());
				bullet->dirVec = reflectionVec.Normalize();
			}
		}
		
	}

	//Clean up the array for any dead bullets
	//by swapping Live-->Dead keep container sorted.
	iterCur = mBulletsList.begin();
	iterEnd = mBulletsList.end();
	for (; iterCur != iterEnd; ++iterCur)
	{
		Bullet* bullet = ((Bullet*)*iterCur);
		if (bullet->IsAlive())
			continue;

		std::deque<IProjectile*>::iterator iterLasPtr = mBulletsList.end();
		--iterLasPtr;
		for (;; --iterLasPtr)
		{
			Bullet* temp = ((Bullet*)*iterLasPtr);
			if (temp->IsAlive() && iterLasPtr != iterCur)
			{
				std::iter_swap(iterCur, iterLasPtr);
			}
			if (iterLasPtr == iterCur)
				break;
		}
	}

	while (!mBulletsList.empty())
	{
		Bullet* temp = (Bullet*)mBulletsList.back();
		if (temp->IsAlive())
			break;//Sorted list done.
		mBulletsList.pop_back();
		mBulletsPoolList.push_back(temp);
	}
	
}

//Method to fire bullets.
void BulletManager::Fire(Vector3 pos, Vector3 dir, float speed, float time, float life_time)
{
	ENTER_CRITICAL_SEC(CRITICAL_SEC_OBJ)
	Bullet* bullet;

	if (mBulletsPoolList.empty())
	{
		bullet = new Bullet();
	}
	else
	{
		bullet = (Bullet*)mBulletsPoolList.front();
		mBulletsPoolList.pop_front();
	}

	bullet->positionVec = pos;
	bullet->dirVec = dir;
	bullet->force = speed;
	bullet->startTime = time;
	bullet->lifeTime = time + life_time;
	mBulletsList.push_back(bullet);
	LEAVE_CRITICAL_SEC(CRITICAL_SEC_OBJ)
}


void BulletManager::Integrate(Vector3& _initPos, Vector3& _dir, float _speed, float _time, float _dt)
{
	//Using RK4 method advance the simulation per-step.
	/*
		yi = yi-1 + 1/6(k1+2k2+3k3+k4)h
		k1 = f(xi,yi);
		k2 = f(xi+1/2h, yi+1/2k1h);
		k3 = f(xi+1/2h, yi+1/2k2h);
		k4 = f(xi+h, yi+k3h);

		initPos + dir * speed * dt;
	*/

	CurrentTransform initialState;
	initialState.position = _initPos;
	initialState.velocity = _dir.Normalize() * _speed;
	DeltaTransform k1 = RK4Stepper(initialState, _time, _dt, DeltaTransform());
	DeltaTransform k2 = RK4Stepper(initialState, _time, _dt*0.5f, k1);
	DeltaTransform k3 = RK4Stepper(initialState, _time, _dt*0.5f, k2);
	DeltaTransform k4 = RK4Stepper(initialState, _time, _dt, k3);

	Vector3 posDx = (k1.positionDx + k2.positionDx * 2 + k3.positionDx * 2 + k4.positionDx) * ONEOVERSIX ;
	Vector3 velDx = (k1.velocityDy + k2.velocityDy * 2 + k3.velocityDy * 2 + k4.velocityDy) * ONEOVERSIX ;

	_initPos = _initPos + posDx * _dt;
}


DeltaTransform BulletManager::RK4Stepper(CurrentTransform& _initial, float _t, float _dt, DeltaTransform& _d)
{
	DeltaTransform output;
	if (_t == 0.0f)
	{
		output.positionDx = _initial.velocity;
		return output;
	}
	else
	{
		CurrentTransform state;
		state.position = _initial.position + _d.positionDx*_dt;
		state.velocity = _initial.velocity + _d.velocityDy*_dt;
		output.positionDx = state.velocity;
	}
	return output;
	
}

bool BulletManager::CheckLifeTime(float _curTime, float _totalTime)
{
	if (_curTime < _totalTime)
		return false;
	return true;
}
