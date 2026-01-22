#pragma once
#include "..//PlayerState.h"	//基底クラス.

/********************************************************************************
*	ポータル取得時にアニメーションを再生させるためのクラスです.
*	このクラスはPlayerに入れないでポータルクラスにいれようと考えてきます.
**/

class Player;
class PlayerContext;

class PlayerPortalAnim
	: public PlayerState
{
public:
	PlayerPortalAnim(Player* pOwner);
	~PlayerPortalAnim() override;

	//初期化処理
	void Enter() override;
	//毎フレーム入る処理
	void Update() override;
	//Stateから出るときに入る処理.
	void Exit() override;

	//エフェクトを入れるときに使用する.
	void Draw() override;
	void Init() override;

private:
};