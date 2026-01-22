#pragma once

#include "SkinMesh//SkinMesh.h"

/*********************************************************************
*	StateBaseClass
**/

class StateBase
{
public:
	StateBase() {};
	virtual ~StateBase() = default;

	//‰Šú‰»ˆ—
	virtual void Enter() = 0;
	//–ˆƒtƒŒ[ƒ€“ü‚éˆ—
	virtual void Update() = 0;
	//State‚©‚ço‚é‚Æ‚«‚É“ü‚éˆ—.
	virtual void Exit() = 0;

	virtual void Draw() = 0;
	virtual void Init() = 0;
protected:
};