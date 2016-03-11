#include "stdafx.h"
#include "GameManager.h"
#include <math.h>
#include <stdio.h>

GameManager::GameManager()
{
	mIsExitRequested = false;
	mGlobals.GetSecondsPerTick();
	mBulletManager = new BulletManager();
}


GameManager::~GameManager()
{
	if (mBulletManager != NULL)
		delete mBulletManager;
}

void GameManager::Tick(MSG& msg, HACCEL& hAccelTable)
{
	static double LastTime = mGlobals.GetCurrentTimeInSeconds();
	static double timeToCatchup = 0.0;
	static double fps = 1 / 120.0f; //FPS.
	while (!mIsExitRequested)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			mGlobals.UpdatePrevTime();

			mGlobals.SetCurrentTime(mGlobals.GetCurrentTimeInSeconds());

			double deltaTime = mGlobals.GetCurrTime() - LastTime;

			timeToCatchup += deltaTime;

			mGlobals.SetDeltaTime(deltaTime);

			while (timeToCatchup >= fps)
			{
				mBulletManager->Update((float)timeToCatchup);
				timeToCatchup -= fps;
			}

			LastTime = mGlobals.GetCurrTime();
		}
	}
}
