#pragma once
#include "StaticMash//StaticMesh.h"
#include "System//00_Manager//ManagerBase.h"

/**********************************************
*	スタティックメッシュマネージャークラス.
**********************************************/
class StaticMeshManager
	: public ManagerBase
{
public:
	//モデルの種類の列挙型.
	enum class CMeshList : byte
	{
		Bullet,				//弾.
		Ground,				//地面.
		SphereCollision,	//球の当たり判定.
		EnemyBullet,		//敵の弾.
		BoxCollision,		//ボックスの当たり判定.

		Portal,				//ポータルのメッシュ.

		SkyBox,				//背景画像の導入.

		PortalFrame,		//ポータルの範囲表示.
		
		Wall,				//壁のメッシュ.

		Max,
	};

public:
	~StaticMeshManager() override;

	void Create() override;
	void Release() override;
	void Init();

	//スタティックメッシュのインスタンス取得.
	StaticMesh* GetMeshInstance(CMeshList MeshNo)
	{
		return m_pMesh[static_cast<int>(MeshNo)].get();
	};

	//シングルトンインスタンスの取得.
	static StaticMeshManager* GetInstance()
	{
		static StaticMeshManager s_Instance;//唯一のインスタンス.
		return &s_Instance;//参照を返す.
	}

private:
	StaticMeshManager();
	//コンストラクタを外部アクセス禁止.
	StaticMeshManager(const StaticMeshManager& rhs) = delete;
	StaticMeshManager& operator=(const StaticMeshManager& rhs) = delete;

	std::unique_ptr<StaticMesh> m_pMesh[static_cast<int>(CMeshList::Max)];
};
