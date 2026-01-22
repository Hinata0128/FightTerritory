#pragma once
#include "Sound.h"		//サウンドクラス.

/**************************************************
*	サウンドマネージャークラス.
*	Manager(マネージャー)：管理者.
*		Singleton(シングルトン：デザインパターンの１つ)で作成.
**/
class SoundManager
{
public:
	//サウンドリスト列挙型.
	enum enList
	{
		BGM_Title,	//タイトル.
		BGM_Main,	//メイン.
		BGM_Over,	//オーバー.
		BGM_Ending,	//エンディング.
		SE_Shot,	//ショット.
		SE_Exp,		//爆発.
		SE_Enter,	//決定の時になる音.
		SE_PortalGet,	//ポータルを取得したときに再生されるSE.
		SE_Slash,	//斬る攻撃.
		SE_Select,	//選択肢選び.
		SE_Round,	//ラウンド開始.

		//音が増えたら「ここ」に追加してください.
		max,		//最大数.
	};

public:
	//インスタンス取得(唯一のアクセス経路).
	//※関数の前にstaticを付けることでインスタンス生成しなくても使用できる.
	static SoundManager* GetInstance()
	{
		//唯一のインスタンスを作成する.
		//※staticで作成されたので2回目以降は、下の1行は無視される.
		static SoundManager s_Instance;	//s_:staticの意味.
		return &s_Instance;
	}


	~SoundManager();

	//サウンドデータ読込関数.
	bool Load( HWND hWnd );
	//サウンドデータ解放関数.
	void Release();

	//SEを再生する.
	static void PlaySE( enList list ) {
		SoundManager::GetInstance()->m_pSound[list]->PlaySE();
	}
	//ループ再生する.
	static void PlayLoop( enList list ) {
		SoundManager::GetInstance()->m_pSound[list]->PlayLoop();
	}
	//停止する.
	static void Stop( enList list ) {
		SoundManager::GetInstance()->m_pSound[list]->Stop();
	}

private://外部からアクセス不可能.
	//外部からコンストラクタへのアクセスを禁止する.
	SoundManager();
	//コピーコンストラクタによるコピーを禁止する.
	//「=delete」で関数の定義を削除できる.
	SoundManager( const SoundManager& rhs ) = delete;
	//代入演算子によるコピーを禁止する.
	//operator(オペレータ):演算子のオーバーロードで、演算の中身を拡張できる.
	SoundManager& operator = ( const SoundManager& rhs ) = delete;

private:
	Sound*		m_pSound[enList::max];
};