#include "PlayerPortalAnim.h"

#include "..//..//Player.h"
#include "..//..//PlayerContext/PlayerContext.h"

#include "System/00_Manager/02_PShotManager/PShotManager.h"

PlayerPortalAnim::PlayerPortalAnim(Player* pOwner)
	: PlayerState	( pOwner )
{
}

PlayerPortalAnim::~PlayerPortalAnim()
{
}

void PlayerPortalAnim::Enter()
{
    PlayerContext ctx(m_pOwner);
    //仮のアニメーション番号.
    ctx.AnimNo = 7; // ポータル取得アニメーションの番号
    ctx.AnimTime = 0.0f;

    // アニメーションを最初から再生し、速度を通常に設定
    ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed, ctx.AnimCtrl);
    ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);

    PlayerState::Enter();

    m_pOwner->InitializePlayerMove();

}

void PlayerPortalAnim::Update()
{
}

void PlayerPortalAnim::Exit()
{
    PlayerContext ctx(m_pOwner);
    ctx.Mesh->SetAnimSpeed(0.0f, ctx.AnimCtrl);
}

void PlayerPortalAnim::Draw()
{
}

void PlayerPortalAnim::Init()
{
}
