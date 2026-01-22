#pragma once
#include "GameObject/02_StaticMeshObject/02_ShotBase/BossShot/BossShot.h"

class Portal;

/*****************************************************************
*	敵の通常攻撃マネージャークラス.
*	ToDo : 基本的にはPShotManagerと同じ作りで作成している.
**/
class BossShotManager final
{
public:
	~BossShotManager();

	static BossShotManager* GetInstance();

	void Update();	//弾の更新処理.
	void Draw(const D3DXMATRIX& view, const D3DXMATRIX& proj);	//弾の描画処理.

	void Init();

	void AddEnemyNomalShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirecton);	//敵の弾を作成して管理リストに追加.

	void Clear();

	void ReMoveEnemyNomalShot(size_t index);	//敵の弾を削除.

	const std::vector<std::unique_ptr<BossShot>>& GetEnemyNomalShot() const;	//敵の弾リスト取得.

	BossShot* GetEnemyNomalShot(size_t No);	//敵の弾取得.

	size_t GetEnemyNomalShotCount() const;	//敵の弾数取得.

	std::vector<BossShot*> GetShots();

	void ChackPortalKill(const Portal& portal);
private:
	BossShotManager();

	BossShotManager(const BossShotManager& rhs) = delete;	//コピー禁止.
	BossShotManager& operator = (const BossShotManager& rhs) = delete;	//代入禁止.
private:
	static BossShotManager* m_pInstance;	//シングルトンインスタンス.

	std::vector<std::unique_ptr<BossShot>> m_pBossShot;	//敵弾の管理配列.
};