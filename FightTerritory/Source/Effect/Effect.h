#pragma once

//警告についてのコード分析を無効にする。4005:再定義
#pragma warning(disable:4005)

//---------------------------------------
// Effekseer関係ヘッダ、ライブラリ
//---------------------------------------
#include "Effekseer//include//Effekseer.h"
#include "Effekseer//include//EffekseerRendererDX11.h"

#include "System/01_Renderer/Renderer.h"
#ifdef _DEBUG
#pragma comment(lib,"Effekseerd.lib")
#pragma comment(lib,"EffekseerRendererDX11d.lib")
#else//#ifdef _DEBUG
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"EffekseerRendererDX11.lib")

#endif//#ifdef _DEBUG

//エイリアスを用意
//※コードが横に長くなって読みづらいため
namespace 
{
	namespace Es = ::Effekseer;
	using EsManagerRef = ::Es::ManagerRef;
	using EsManager = ::Es::Manager;
	using EsEffectRef = ::Es::EffectRef;
	using EsEffect = ::Es::Effect;
	using EsVec3 = ::Es::Vector3D;
	using EsMatrix = ::Es::Matrix44;
	using EsHandle = ::Es::Handle;
	using EsRendererRef = ::EffekseerRendererDX11::RendererRef;
	using EsRenderer = ::EffekseerRendererDX11::Renderer;
};

/**************************************************
*	フリーソフト Effekseerのデータを使うためのクラス
*	singleton(シングルトン：デザインパターンの１つ)で作成
**/
class Effect
{
public:
	//エフェクト種類列挙型.
	enum enList
	{
		Laser01 = 0,	//当たり判定確認用で設定.
		//ToDo : エフェクトの追加はこの下から.
		LookPortal,		//ポータルをわかりやすく表示.

		Max				//最大数.
	};

	//インスタンス取得(唯一のアクセス経路)
	static Effect* GetInstance()
	{
		//唯一のインスタンスを作成する.
		//※staticで作成されたので２回目以降は下の１行は無視される.
		static Effect s_Instance;	//s_:staticの意味
		return &s_Instance;
	}
	~Effect();

	//構築.
	HRESULT Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	//データ読み込み.
	HRESULT LoadData();
	//描画.
	void Draw();

	//-----------------------------------
	// 変換系
	//-----------------------------------
	//ベクター.
	::EsVec3 ToEfkVector3(const D3DXVECTOR3* pSrcVec3Dx);
	D3DXVECTOR3 ToDxVector3(const ::EsVec3* pSrcVec3Efk);
	//行列.
	::EsMatrix ToEfkMatrix(const D3DXMATRIX* pSrcMatDx);
	D3DXMATRIX ToDxMatrix(const ::EsMatrix* pSrcMatEfk);

	//-----------------------------------
	// 制御系
	//-----------------------------------
	//再生.
	static ::EsHandle Play(enList listNo, const D3DXVECTOR3& pos) 
	{
		Effect* pE = Effect::GetInstance();
		return pE->m_pManager->Play(pE->m_pEffect[listNo], pos.x, pos.y, pos.z);
	}
	//停止.
	static void Stop(::EsHandle handle) 
	{
		Effect::GetInstance()->m_pManager->StopEffect(handle);
	}
	//全て停止.
	static void StopAll() 
	{
		Effect::GetInstance()->m_pManager->StopAllEffects();
	}
	//一時停止.
	static void Paused(::EsHandle handle, bool paused)
	{
		Effect::GetInstance()->m_pManager->SetPaused(handle, paused);
	}
	//再生速度の設定.
	static void SetSpeed(::EsHandle handle, float speed) 
	{
		Effect::GetInstance()->m_pManager->SetSpeed(handle, speed);
	}
	//位置を指定する.
	static void SetLocation(::EsHandle handle, D3DXVECTOR3 pos) 
	{
		Effect::GetInstance()->m_pManager->
			SetLocation(handle, ::EsVec3(pos.x, pos.y, pos.z));
	}
	//回転を指定する.
	static void SetRotation(::EsHandle handle, D3DXVECTOR3 rot) 
	{
		Effect::GetInstance()->m_pManager->
			SetRotation(handle, rot.x, rot.y, rot.z);
	}
	//回転を指定する（軸回転）.
	static void SetRotation(::EsHandle handle, D3DXVECTOR3 vAxis, float angle)
	{
		Effect::GetInstance()->m_pManager->
			SetRotation(handle, ::EsVec3(vAxis.x, vAxis.y, vAxis.z), angle);
	}
	//サイズを指定する.
	static void SetScale(::EsHandle handle, D3DXVECTOR3 scale) 
	{
		Effect::GetInstance()->m_pManager->
			SetScale(handle, scale.x, scale.y, scale.z);
	}

private:
	//生成やコピーを禁止する.
	Effect();
	Effect(const Effect& rhs) = delete;
	Effect& operator = (const Effect& rhs) = delete;

	//データ解放.
	HRESULT ReleaseData();

	void SetViewMatrixFromRenderer();
	void SetProjectionMatrixFromRenderer();


private:
	//エフェクトを動作させるために必要.
	::EsManagerRef		m_pManager;
	::EsRendererRef		m_pRenderer;

	//エフェクトの種類ごとに必要.
	::EsEffectRef		m_pEffect[enList::Max];
};

