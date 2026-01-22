#pragma once
#include "..//PlayerState.h"

class Player;
class PlayerContext;
class PlayerMove;

#include "System/00_Manager/07_PlayerAttackManager/PlayerAttackManager.h"

/********************************************************************
*	PlayerIdolClass.
**/

class PlayerIdol final
	: public PlayerState
{
public:
	PlayerIdol(Player* pOwner);
	~PlayerIdol() override;

	//初期化処理
	void Enter() override;
	//毎フレーム入る処理
	void Update() override;
	//Stateから出るときに入る処理.
	void Exit() override;

	//継承させているから使用しない.
	void Draw() override;
	void Init() override;
private:
	std::shared_ptr<PlayerMove>		m_pPlayerMove;
};