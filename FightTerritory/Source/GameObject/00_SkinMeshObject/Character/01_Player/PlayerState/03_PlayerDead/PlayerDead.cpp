#include "PlayerDead.h"

#include "..//..//Player.h"
#include "..//..//PlayerContext/PlayerContext.h"

#include "System/00_Manager/02_PShotManager/PShotManager.h"

PlayerDead::PlayerDead(Player* pOwner)
	: PlayerState(pOwner)
{
}

PlayerDead::~PlayerDead()
{
}

//Player‚ªŽ€–S‚µ‚½‚Æ‚«‚É“ü‚éˆ—.
void PlayerDead::Enter()
{
	PlayerContext ctx(m_pOwner);

	ctx.Mesh->SetAnimSpeed(ctx.AnimSpeed);

	ctx.AnimNo = 7;
	ctx.AnimTime = 0.0f;
	ctx.Mesh->ChangeAnimSet(ctx.AnimNo, ctx.AnimCtrl);

	PShotManager::GetInstance()->ClearPlayerShot();

	PlayerState::Enter();
}

void PlayerDead::Update()
{
	//Player‚Ì’e‚ðˆêÄ‚Éclear‚É‚·‚é.
	//í‚É‚·‚é‚Ì‚Å‚Í‚È‚­PlayerDead‚É“ü‚Á‚½Žž‚É‚±‚ÌŠÖ”‚Ì’†‚É“ü‚é.
	PShotManager::GetInstance()->ClearPlayerShot();

	PlayerState::Update();
}

void PlayerDead::Exit()
{
	PlayerState::Exit();
}

void PlayerDead::Draw()
{
	PlayerState::Draw();
}

void PlayerDead::Init()
{
	PlayerState::Init();
}
