#pragma once
#include "System//02_Singleton//00_Timer//Timer.h"
#include "Collision//BoundingBox//BoundingBox.h"
#include "Collision//BoundingSphere//BoundingSphere.h"

//当たり判定に使用するクラスをインクルード.
#include "System//00_Manager//ManagerBase.h"
#include "System//00_Manager//04_BossShotManager//BossShotManager.h"
#include "GameObject//02_StaticMeshObject//02_ShotBase//PShot//PShot.h"
#include "GameObject//00_SkinMeshObject//Character//01_Player//Player.h"
#include "GameObject/00_SkinMeshObject/Character//00_Boss/Boss.h"

/******************************************************************************
*	当たり判定マネージャークラス.
**/

class CollisionManager
	: public ManagerBase
{
public:
	CollisionManager();
	~CollisionManager() override;

	void Update() override;
	void Draw() override;
	void Create() override;
	void Release() override;

	//コライダーの登録関数.
	void AddSphere(const std::shared_ptr<BoundingSphere>& sphere);
	void AddBox(const std::shared_ptr<BoundingBox>& box);
	//登録されているすべてのコライダーを削除する.
	void Clear();

	//当たり判定を常に起動しておく関数.
	void AllCollider();

public:
	void SetPlayer(const std::shared_ptr<Player>& player) { m_pPlayer = player; }
	void SetEnemies(const std::vector<Boss*>& enemies) { m_vEnemies = enemies; }
	void SetPlayerShots(const std::vector<PShot*>& shots) { m_PlayerShots = shots; }
	void SetEnemyShots(const std::vector<BossShot*>& shots) { m_EnemyShots = shots; }

	void SetShortAttackSphere(const std::shared_ptr<BoundingSphere>& sphere) { m_pShortAttackSphere = sphere; }

private:
	//当たり判定の関数をここに書きます
	bool CheckSphereSphere(const BoundingSphere& a, const BoundingSphere& b);	//スフィア同士の判定. 
	bool CheckBoxBox(const BoundingBox& a, const BoundingBox& b);				//AABB同士の判定. 
	bool CheckSphereBox(const BoundingSphere& sphere, const BoundingBox& box);	//スフィアとAABBの判定. 

private:
	std::vector<std::shared_ptr<BoundingSphere>>	m_pBSphere;
	std::vector<std::shared_ptr<BoundingBox>>		m_pBBox;

	std::shared_ptr<Player> m_pPlayer;
	std::vector<Boss*> m_vEnemies;	
	std::vector<PShot*> m_PlayerShots;
	std::vector<BossShot*> m_EnemyShots;
	
	std::shared_ptr<BoundingSphere> m_pShortAttackSphere;
};