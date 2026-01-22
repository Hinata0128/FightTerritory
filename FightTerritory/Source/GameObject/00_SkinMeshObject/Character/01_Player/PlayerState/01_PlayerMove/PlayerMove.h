#pragma once
#include "..//PlayerState.h"

class Player;
class PlayerContext;

#include "System/00_Manager/07_PlayerAttackManager/PlayerAttackManager.h"
#include "System/02_Singleton/00_Timer/Timer.h"
#include "..//..//System/00_Manager/08_InputKeyManager/InputKeyManager.h"

/******************************************************************
*	PlayerMoveClass.
**/

class PlayerMove final
	: public PlayerState
{
public:
	//Playerの右クリックの制御列挙型.
	enum class enStep : byte
	{
		none,
		first,
		run,
		end,
		release_anim,
	};
	//Playerの左クリックの制御列挙型.
	enum class enLeftStep : byte
	{
		none,
		first,
		Attack,
		end,
		release_anim,
	};

	//Playerの移動(WASD)の制御列挙型
	enum class enMove : byte
	{
		//待機.
		Idol,		
		//WASDの順番でか書いている.
		ForWard,			//前進(W).
		Left,				//左(A).
		Back,				//後退(S).
		Right,				//右(D).
		//斜め前.
		ForWardAndLeft,		//左斜め前(W+A).
		ForWardAAndRight,	//右斜め前(W+D).
		//斜め後ろ.
		BackAndLeft,		//左斜め後ろ(S+A).
		BackAndRight,		//右斜め後ろ(S+D).
	};
public:
	PlayerMove(Player* pOwner);
	~PlayerMove() override;

	//初期化処理
	void Enter() override;
	//毎フレーム入る処理
	void Update() override;
	//Stateから出るときに入る処理.
	void Exit() override;

	void Draw() override;
	void Init() override;

	//Playerの右クリックの攻撃関数.
	void RbuttonAttackStep(PlayerContext& ctx);
	//Playerの左クリックの攻撃関数.
	//引数を書かないといけないけれども一旦関数を作成するために書いていない.
	void LButtonAttackStep(PlayerContext& ctx);

	//Playerの動作[GetAsyncKeyState()]を書く関数.
	void HandleMove(
		PlayerContext& ctxconst,
		const D3DXVECTOR3& WS,
		const D3DXVECTOR3& AD);
public:
	//操作の関数,
	enMove GetMoveInput();
private:
	//遠距離攻撃の列挙.
	enStep step;
	//近距離攻撃の列挙.
	enLeftStep LStep;
	//Playerの移動の列挙.
	enMove Move;

	bool AttackType = false;

	bool m_IsShot = false;
	//InputKeyのメンバ変数.
	std::unique_ptr<InputKeyManager> m_Key;

	//攻撃状態をboolで管理させる.
	bool IsRAttacking = false;
	//右クリックの攻撃状態をboolで管理する.
	bool IsLAttacking = false;
};
