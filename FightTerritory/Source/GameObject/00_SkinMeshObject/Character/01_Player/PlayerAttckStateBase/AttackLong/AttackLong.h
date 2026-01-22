#pragma once
#include "..//PlayerAttckStateBase.h"

class Player;
class PlayerContext;
class Timer;

/***************************************************************
*	AttackLongClass(Playerの遠距離攻撃Class).
*	PlayerAttackStateBaseを継承させる
*	PlayerAttackManagerでポリモーフィズムを使用してPlayerClassへ操作をまかしている.
**/

class AttackLong
	: public PlayerAttckStateBase
{
public:
	AttackLong();
	~AttackLong() override;
	
	void Enter(Player* player) override;
	void ExecuteAttack(Player* player) override;
	void Exit(Player* player) override;

private:
	float m_ShotCoolDown;
	float m_CoolTime;
	D3DXVECTOR3 m_ShotOffset;
};