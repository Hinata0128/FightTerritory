#pragma once

#include <Windows.h>

//ライブラリ読み込み.
#pragma comment( lib, "winmm.lib" ) //音声再生で必要.

/**************************************************
*	サウンドクラス.
*		midi,mp3,wav形式ファイルの再生・停止等を行う.
**/
class Sound
{
public://public:外部からアクセス可能.
	//定数宣言.
	static constexpr int STR_BUFF_MAX = 256;//文字バッファ.

	//コンストラクタ.
	//オブジェクトの生成時に自動的に呼び出される特殊なメンバ関数.
	Sound();

	//デストラクタ.
	//オブジェクトが破棄されたときに自動的に呼び出される特殊なメンバ関数.
	//関数名はクラス名に「~(チルダ)」を付ける.
	~Sound();

	//音声ファイルを開く.
	bool Open( LPCTSTR sFName, LPCTSTR sAlias, HWND hWnd );

	//音声ファイルを閉じる.
	bool Close();

	//音声を再生する関数(状態通知の有無フラグ).
	bool Play( bool bNotify = false );

	//音声を再生する(SEで使う).
	bool PlaySE();

	//音声を再生する(ループ).
	bool PlayLoop();

	//音声を停止する関数.
	bool Stop();

	//音声の状態を取得する.
	bool GetStatus( LPTSTR sStatus );

	//音声の停止を確認する関数.
	bool IsStopped();

	//音声の再生位置を最初にする関数.
	bool SeekToStart();

	//音声の音量を設定する関数.
	bool SetVolume( int iVolume );
	//音声の音量を取得する関数.
	int GetVolume() const { return m_iVolume; }

private://private:外部からアクセス不可能.
		//このクラス内でしか使用しない関数などもここに宣言する.
	//初期値の設定する関数.
	void SetInitParam( LPCTSTR sAlias, HWND hWnd );

private://private:外部からアクセス不可能.
	//メンバ変数は基本的にはprivateで宣言する.
	HWND	m_hWnd;					//ウィンドウハンドル.
	TCHAR	m_sAlias[STR_BUFF_MAX];	//エイリアス.
	int		m_iVolume;				//ボリューム.
};

//命名規則とハンガリアン記法.
//m_:member（メンバー）.
//g_:global（グローバル）.
//s_:static（スタティック）.
//iNo:int（int型の番号）.
//str:string（ストリング：文字列）.