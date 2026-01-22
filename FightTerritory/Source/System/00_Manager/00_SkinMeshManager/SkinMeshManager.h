#pragma once

#include "SkinMesh//SkinMesh.h"
#include "System//00_Manager//ManagerBase.h"

/*****************************************************************************
*	スキンメッシュマネージャークラス
*	最終的には、アセットクラスを作成してそこにスタティックとスキンメッシュを統合させる.
*	今はシングルトンにしておきます.
**/

class SkinMeshManager
	: public ManagerBase
{
public:
	// スキンモデルの種類の列挙型.
	enum class SkinList
	{
		Player,		// プレイヤー.
		Enemy,		// 仮の敵です.
		zako,

		Max
	};

public:
	// シングルトン関連
	static SkinMeshManager* GetInstance();
	~SkinMeshManager() override;

	// ManagerBaseの関数をオーバーライド
	void Create() override;
	void Release() override;

	SkinMesh* GetSkinMeshInstance(SkinList skinNo);

private:
	SkinMeshManager();
	SkinMeshManager(const SkinMeshManager&) = delete;
	SkinMeshManager& operator=(const SkinMeshManager&) = delete;

private:
	std::unique_ptr<SkinMesh> m_pSkin[static_cast<int>(SkinList::Max)];
};
