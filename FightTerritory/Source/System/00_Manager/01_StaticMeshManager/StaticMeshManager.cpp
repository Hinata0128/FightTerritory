#include "StaticMeshManager.h"
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning(disable:4005)

StaticMeshManager::StaticMeshManager()
{
}

StaticMeshManager::~StaticMeshManager()
{
	Release();
}

void StaticMeshManager::Create()
{
	//スタティックメッシュ配列を初期化.
	for (int i = 0; i < static_cast<int>(CMeshList::Max); ++i)
	{
		m_pMesh[i] = std::make_unique<StaticMesh>();
	}

	//読み込みリスト構造体の定義.
	struct MeshList
	{
		int listNo;//CMeshList列挙型を設定.
		const TCHAR path[256];//ファイルパス.
	};

	//読み込み対象リストの設定.
	MeshList MList[] =
	{
		{ static_cast<int>(CMeshList::Bullet),			_T("Data\\Mesh\\Static\\Bullet\\BulletTest.x") },
		{ static_cast<int>(CMeshList::Ground),			_T("Data\\Mesh\\Static\\Ground\\ground.x") },
		{ static_cast<int>(CMeshList::SphereCollision),	_T("Data\\Mesh\\Static\\Collision\\Sphere.x") },
		{ static_cast<int>(CMeshList::EnemyBullet),		_T("Data\\Mesh\\Static\\Bullet\\EnemyBullet.x") },
		{ static_cast<int>(CMeshList::BoxCollision),	_T("Data\\Mesh\\Static\\Collision\\Box.x")},
		{ static_cast<int>(CMeshList::Portal),			_T("Data\\Mesh\\Static\\Portal\\Portal.x")},
		{ static_cast<int>(CMeshList::SkyBox),			_T("Data\\Mesh\\Static\\SkyBox\\SkyBox.x")},
		{ static_cast<int>(CMeshList::PortalFrame),		_T("Data\\Mesh\\Static\\PortalFrame\\PortalFrame.x")},
	};

	//配列の要素数を取得.
	const int listMax = static_cast<int>(std::size(MList));

	//読み込み処理.
	for (int i = 0; i < listMax; ++i)
	{
		int idx = MList[i].listNo;
		if (FAILED(m_pMesh[idx]->Init(MList[i].path)))
		{
			//読み込み失敗時は安全に終了.
			return;
		}
	}
}

void StaticMeshManager::Release()
{
	//unique_ptrなので自動で解放されるが、明示的にリセットしておく.
	for (int i = 0; i < static_cast<int>(CMeshList::Max); ++i)
	{
		m_pMesh[i].reset();
	}
}

void StaticMeshManager::Init()
{
}
