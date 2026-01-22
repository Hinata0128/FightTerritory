#include "SkinMeshManager.h"

SkinMeshManager::SkinMeshManager()
{
}

SkinMeshManager::~SkinMeshManager()
{
	Release();
}

void SkinMeshManager::Create()
{
	//スキン配列の初期化.
	for (int i = 0; i < static_cast<int>(SkinList::Max); ++i)
	{
		m_pSkin[i] = std::make_unique<SkinMesh>();
	}

	//読み込みリスト構造体の定義.
	struct MeshList
	{
		int listNo;
		const TCHAR path[256];
	};

	//読み込み対象リストの設定.
	MeshList SList[] =
	{
		{ static_cast<int>(SkinList::Player),	_T("Data\\Mesh\\Skin\\EXTINGER\\extinger.X") },
		{ static_cast<int>(SkinList::Enemy),	_T("Data\\Mesh\\Skin\\boss\\boss.X") },
		{ static_cast<int>(SkinList::zako),	_T("Data\\Mesh\\Skin\\zako\\zako.X") },
	};

	//配列の要素数を取得.
	const int listMax = static_cast<int>(std::size(SList));

	//読み込み処理.
	for (int i = 0; i < listMax; ++i)
	{
		int idx = SList[i].listNo;
		if (FAILED(m_pSkin[idx]->Init(SList[i].path)))
		{
			//読み込み失敗時は安全に終了.
			return;
		}
	}
}

void SkinMeshManager::Release()
{
	//unique_ptrなので自動で解放されるが、明示的にリセットしておく.
	for (int i = 0; i < static_cast<int>(SkinList::Max); ++i)
	{
		m_pSkin[i].reset();
	}
}

SkinMesh* SkinMeshManager::GetSkinMeshInstance(SkinList skinNo)
{
	//指定されたスキン番号のインスタンスを取得.
	return m_pSkin[static_cast<int>(skinNo)].get();
}

//シングルトンインスタンスの取得.
SkinMeshManager* SkinMeshManager::GetInstance()
{
	static SkinMeshManager s_Instance;
	return &s_Instance;
}
