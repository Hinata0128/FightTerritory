#include "BossPortalAnim.h"

#include "..//..//Boss.h"
#include "..//..//00_BossContext//BossContext.h"
#include "System//00_Manager//04_BossShotManager//BossShotManager.h"

BossPortalAnim::BossPortalAnim(Boss* pOwner)
	: BossStateBase	( pOwner )
{
}

BossPortalAnim::~BossPortalAnim()
{
}

void BossPortalAnim::Enter()
{
	//最初にアニメーションを再生させる.
	
	BossContext ctx(m_pOwner);
	//ポータル取得アニメーション番号.
	ctx.AnimNo = 5;
	//アニメーションを最初から再生して、速度を通常に設定.
	ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed, ctx.AnimCtrl);
	//アニメーションを変更させる.
	ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);

	BossStateBase::Enter();
}

void BossPortalAnim::Update()
{
}

void BossPortalAnim::Exit()
{
	BossContext ctx(m_pOwner);
	ctx.Mesh->SetAnimSpeed(0.0f, ctx.AnimCtrl);
}

void BossPortalAnim::Draw()
{
}

void BossPortalAnim::Init()
{
}
