#pragma once
#include "..//PlayerAttckStateBase.h"

class Player;
class PlayerContext;
class Timer;

/*******************************************************************
*	Player‚ÌUŒ‚‚Ì‰Šú‰»‚ğ‚·‚é‚½‚ß‚Ì‹ó‚ÌƒNƒ‰ƒX.
**/

class NoAttack
	: public PlayerAttckStateBase
{
public:
	NoAttack();
	~NoAttack() override;

	void Enter(Player* player) override;
	void ExecuteAttack(Player* player) override;
	void Exit(Player* player) override;

private:

};