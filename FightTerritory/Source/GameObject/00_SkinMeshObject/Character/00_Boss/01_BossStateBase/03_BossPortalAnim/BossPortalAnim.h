#pragma once
#include "..//BossStateBase.h"

/**********************************************************************************
*	ポータル取得時にアニメーションを再生させるクラス.
*	ToDo : ボスがポータルを取得した際にSEがなるけれどももう少しわかりやすくするために
		　 アニメーションを再生させることでわかりやすくする.
**/

class Boss;
class BossContext;
class BossShotManager;

class BossPortalAnim final
	: public BossStateBase
{
public:
	BossPortalAnim(Boss* pOwner);
	~BossPortalAnim();

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