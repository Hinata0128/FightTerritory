#include "CollisionManager.h"

#include "Effect/Effect.h"

#include <algorithm>

#include "System//02_Singleton//00_Timer//Timer.h"

#include "Sound//SoundManager.h"



CollisionManager::CollisionManager()
    : m_pPlayer(nullptr)
{
}

CollisionManager::~CollisionManager()
{
    Release();
}

void CollisionManager::Update()
{
    AllCollider(); 
}

void CollisionManager::Draw()
{
#ifdef _DEBUG
    //スフィア描画
    for (const auto& sphere : m_pBSphere)
    {
        if (sphere) sphere->Draw();	
    }

    for (auto eShot : m_EnemyShots)
    {
        if (eShot && eShot->IsDisplay() && eShot->IsActive())
        {
            eShot->GetBoundingSphere().Draw();
        }
    }

    //ボックス描画
    for (const auto& box : m_pBBox)
    {
        if (box) box->Draw();		
    }
#endif
}

void CollisionManager::Create()
{
    m_pBSphere.clear();	
    m_pBBox.clear();	
}

void CollisionManager::Release()
{
    m_pBSphere.clear();
    m_pBBox.clear();
    m_pPlayer.reset();
    m_vEnemies.clear();
    m_PlayerShots.clear();
    m_EnemyShots.clear();
}

void CollisionManager::AddSphere(const std::shared_ptr<BoundingSphere>& sphere)
{
    if (sphere) m_pBSphere.push_back(sphere); //スフィア登録.
}

void CollisionManager::AddBox(const std::shared_ptr<BoundingBox>& box)
{
    if (box) m_pBBox.push_back(box); //ボックス登録.
}

void CollisionManager::Clear()
{
}

void CollisionManager::AllCollider()
{
    if (!m_pPlayer || m_pPlayer->IsDead()) return;

    //プレイヤーのスフィア座標を取得
    const BoundingSphere& pSphere = m_pPlayer->GetBoundingSphere();
    D3DXVECTOR3 pPos = pSphere.GetPostion();
    float pRadius = pSphere.GetRadius();

    //プレイヤー vs ボス (押し戻し)
    for (auto enemy : m_vEnemies)
    {
        if (!enemy) continue;

        enemy->GetBoundingSphere().SetPosition(enemy->GetHitCenter());
        D3DXVECTOR3 ePos = enemy->GetHitCenter();
        float eRadius = enemy->GetBoundingSphere().GetRadius();

        D3DXVECTOR3 diff = pPos - ePos;
        float distance = D3DXVec3Length(&diff);
        float sumRadius = pRadius + eRadius;

        if (distance < sumRadius)
        {
            float overlap = sumRadius - distance;
            if (distance < 0.0001f) { diff = D3DXVECTOR3(0, 0, 1); }
            else { D3DXVec3Normalize(&diff, &diff); }

            D3DXVECTOR3 moveVec = diff * overlap;
            moveVec.y = 0.0f; // Y軸移動を制限
            m_pPlayer->AddPosition(moveVec);

            // 移動させたのでスフィア位置を即時再同期
            m_pPlayer->GetBoundingSphere().SetPosition(m_pPlayer->GetHitCenter());
        }
    }

    //プレイヤー弾 vs ボス
    for (auto enemy : m_vEnemies)
    {
        if (!enemy) continue;
        for (auto pShot : m_PlayerShots)
        {
            if (!pShot || !pShot->IsDisplay() || !pShot->IsActive()) continue;

            pShot->GetBoundingSphere().SetPosition(pShot->GetPosition());

            if (pShot->GetBoundingSphere().IsHit(enemy->GetBoundingSphere()))
            {
                enemy->Hit();
                pShot->SetDisplay(false);
                pShot->SetActive(false);
                pShot->SetPosition(D3DXVECTOR3(0.f, -100.f, 0.f));

                if (enemy->GetEnemyHitPoint() <= 0.0f)
                {
                    SoundManager::GetInstance()->PlaySE(SoundManager::SE_Exp);
                    Effect::Play(Effect::Laser01, enemy->GetHitCenter());
                    enemy->SetPosition(0.0f, -10.0f, 0.0f);
                }
                break;
            }
        }
    }

    //敵の弾 vs プレイヤー
    for (auto eShot : m_EnemyShots)
    {
        if (!eShot || !eShot->IsDisplay() || !eShot->IsActive()) continue;

        eShot->GetBoundingSphere().SetPosition(eShot->GetPosition());

        // プレイヤーのスフィアと判定
        if (eShot->GetBoundingSphere().IsHit(m_pPlayer->GetBoundingSphere()))
        {
            m_pPlayer->Hit(); // ここでプレイヤーのHPが減る

            if (m_pPlayer->GetHitPoint() <= 0.0f)
            {
                SoundManager::GetInstance()->PlaySE(SoundManager::SE_Exp);
                Effect::Play(Effect::Laser01, eShot->GetPosition());
            }

            eShot->SetDisplay(false);
            eShot->SetActive(false);
            break;
        }
    }

    //近距離攻撃 vs 敵
    AttackShort* pShortAttack = m_pPlayer->GetShortAttackState();
    if (pShortAttack && pShortAttack->IsHitActive() && !pShortAttack->HasHit())
    {
        const BoundingSphere& swordSphere = pShortAttack->GetHitBox();
        for (auto enemy : m_vEnemies)
        {
            if (!enemy) continue;
            if (swordSphere.IsHit(enemy->GetBoundingSphere()))
            {
                enemy->Hit();
                pShortAttack->SetHit();
                if (enemy->GetEnemyHitPoint() <= 0.0f)
                {
                    SoundManager::GetInstance()->PlaySE(SoundManager::SE_Exp);
                    Effect::Play(Effect::Laser01, enemy->GetHitCenter());
                    enemy->SetPosition(0.0f, -10.0f, 0.0f);
                }
                break;
            }
        }
    }
}

bool CollisionManager::CheckSphereSphere(const BoundingSphere& a, const BoundingSphere& b)
{
    return a.IsHit(b);
}

bool CollisionManager::CheckBoxBox(const BoundingBox& a, const BoundingBox& b)
{
    return a.IsHit(b);
}

bool CollisionManager::CheckSphereBox(const BoundingSphere& sphere, const BoundingBox& box)
{
    // 球体とAABBの判定（後で拡張可能）.
    // 今は簡易版として中心距離で判定.
    D3DXVECTOR3 closest;
    closest.x = std::max<float>(box.GetMin().x, std::min<float>(sphere.GetPostion().x, box.GetMax().x));
    closest.y = std::max<float>(box.GetMin().y, std::min<float>(sphere.GetPostion().y, box.GetMax().y));
    closest.z = std::max<float>(box.GetMin().z, std::min<float>(sphere.GetPostion().z, box.GetMax().z));

    D3DXVECTOR3 diff = sphere.GetPostion() - closest;
    return D3DXVec3Length(&diff) <= sphere.GetRadius();
}
