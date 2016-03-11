#pragma once
#include "Globals.h"

#include "BulletManager.h"

class GameManager
{
public:
	GameManager();
	virtual ~GameManager();

	void Tick(MSG& msg, HACCEL& hAccelTable);
	void SetExitRequested(bool val){ mIsExitRequested = val; }

private:

	GameManager(const GameManager& rhs);
	GameManager& operator=(const GameManager rhs);

	bool mIsExitRequested;
	Globals mGlobals;
	BulletManager *mBulletManager;
};

