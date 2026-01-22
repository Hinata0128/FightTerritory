#pragma once
#include "System//03_Base//StateBase//StateBase.h"

//----------------------------------
// EnemyNomalClassの前方宣言.
//----------------------------------
class Boss;

/**********************************************************
*	BossStateBase
*	ノーマル敵のステートクラスの作成.
*	このクラスは基底クラスになっています.
**/

class BossStateBase
	: public StateBase
{
public:
	//引数アリのコンストラクタを使用する.
	//引数にEnemyの情報を取得する.
	BossStateBase(Boss* pOwner);
	virtual ~BossStateBase() override;

	//初期化
	virtual void Enter() override;
	//毎フレーム入る処理.
	virtual void Update() override;
	//Stateから出るときに入る処理.
	virtual void Exit() override;

	//エフェクトを入れるときに使用する.
	virtual void Draw();
	virtual void Init();

protected:
	//派生クラスからOwnerを取得.
	Boss* GetOwner() const;

	Boss* m_pOwner;

};