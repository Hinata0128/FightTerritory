#pragma once

class Player;

/*************************************************************************
*	攻撃をManagerClassにアタッチするために必要な基底クラス.
**/

class PlayerAttckStateBase
{
public:
	PlayerAttckStateBase();
	virtual ~PlayerAttckStateBase() = default;

	virtual void Enter(Player* player) {}
	virtual void ExecuteAttack(Player* player) {}
	virtual void Exit(Player* player) {}

	virtual bool IsFinished() const { return false; }

protected:
};