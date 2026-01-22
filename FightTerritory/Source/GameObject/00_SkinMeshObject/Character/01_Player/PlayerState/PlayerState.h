#pragma once
#include "System/03_Base/StateBase/StateBase.h"

class Player;

/*********************************************************************
*	PlayerStateBaseClass
**/

class PlayerState
	: public StateBase
{
public:
	PlayerState(Player* pOwner);
	virtual ~PlayerState() override;

	//初期化処理
	virtual void Enter() override;
	//毎フレーム入る処理
	virtual void Update() override;
	//Stateから出るときに入る処理.
	virtual void Exit() override;

	//エフェクトを入れるときに使用する.
	virtual void Draw() override;
	virtual void Init() override;

protected:
	//派生クラスからOwnerを取得.
	Player* GetOwner() const;

	Player* m_pOwner;
};