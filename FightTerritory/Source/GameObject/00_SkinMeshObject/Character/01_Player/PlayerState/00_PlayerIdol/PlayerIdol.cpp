#include "PlayerIdol.h"

#include "..//..//Player.h"
#include "..//..//PlayerContext/PlayerContext.h"
#include "..//01_PlayerMove//PlayerMove.h"

PlayerIdol::PlayerIdol(Player* pOwner)
    : PlayerState(pOwner)
    //Playerの情報を取得するので引数をpOwnerにしておく.
    , m_pPlayerMove     (std::make_shared<PlayerMove>(pOwner))
{
}

PlayerIdol::~PlayerIdol()
{
}

void PlayerIdol::Enter()
{
}

void PlayerIdol::Update()
{
    PlayerContext ctx(m_pOwner);

    bool isMoving = false; //移動しているかフラグ.
    const int IDOL_ANIM_NO = 0;

    //最初はIdolでその中で毎フレームUpdata()が回っている.
    m_pPlayerMove->Update();
}

void PlayerIdol::Exit()
{
}

void PlayerIdol::Draw()
{
    PlayerState::Draw();
}

void PlayerIdol::Init()
{
    m_pPlayerMove->Init();
    PlayerState::Init();
}
