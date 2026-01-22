#pragma once
#include "GameObject/02_StaticMeshObject/02_ShotBase/PShot/PShot.h"
#include <vector>

class Player;
class Portal;

/**********************************************
*プレイヤー弾マネージャークラス.
*弾の生成・更新・描画・破棄を管理する.
**********************************************/
class PShotManager
{
public:
	~PShotManager();

	static PShotManager* GetInstance();	//インスタンス取得.

	void Update();	//弾の更新処理.
	void Draw(const D3DXMATRIX& view, const D3DXMATRIX& proj);	//弾の描画処理.
	void Init();

	void AddPlayerShot(const D3DXVECTOR3& Pos, const D3DXVECTOR3& InitDirection);	//プレイヤーの弾を追加.
	void ReMovePlayerShot(size_t index);	//指定インデックスの弾を削除.

	//Playerが死んだときにGameMain内にPlayerの弾が残るのを一斉に削除する.
	void ClearPlayerShot();

	const std::vector<std::unique_ptr<PShot>>& GetPlayerShot()const;	//弾リストを取得.
	PShot* GetPlayerShot(size_t No);	//指定番号の弾を取得.
	size_t GetPlayerShotCount()const;	//弾の総数を取得.

	std::vector<PShot*> GetShots();

	void ChackPortalKill(const Portal& portal);
private:
	PShotManager();
	PShotManager(const PShotManager& rhs) = delete;
	PShotManager& operator=(const PShotManager& rhs) = delete;

private:
	std::vector<std::unique_ptr<PShot>> m_PlayerShot;	//弾リスト.
};