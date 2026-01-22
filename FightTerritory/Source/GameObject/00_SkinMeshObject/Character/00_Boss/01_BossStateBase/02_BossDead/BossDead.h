#pragma once
#include "..//BossStateBase.h"

/******************************************************************
*	敵の死亡クラス.
**/

class Boss;
class BossContext;

class BossDead final
	: public BossStateBase
{
public:
	BossDead(Boss* pOwner);
	~BossDead();

	//初期化処理
	void Enter() override;
	//毎フレーム入る処理
	void Update() override;
	//Stateから出るときに入る処理.
	void Exit() override;

	void Draw() override;
	void Init() override;


private:

};