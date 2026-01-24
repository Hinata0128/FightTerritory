#pragma once
#include "..//BossStateBase.h"

class Boss;
class BossContext;

//class Com;

/*****************************************************************
*	NomalIdolClass(敵のアイドル状態).
**/

class BossIdol final
	: public BossStateBase
{
public:
	BossIdol(Boss* pOwner);
	~BossIdol() override;

	//初期化処理.
	void Enter() override;
	//毎フレーム入る処理.
	void Update() override;
	//Stateから出るときに入る処理.
	void Exit() override;

	//継承されているから使用しない.
	void Draw() override;
	void Init() override;
private:
};