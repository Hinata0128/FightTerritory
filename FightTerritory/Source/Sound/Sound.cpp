#include "Sound.h"
#include <stdio.h>  //sprintf_s関数を使用するのに必要.

//コンストラクタ.
Sound::Sound()
	//初期化子リスト.
	: m_hWnd( nullptr )
	, m_sAlias()
	, m_iVolume( 1000 )
{
}

//デストラクタ.
Sound::~Sound()
{
}

//================================================================================
//  音声ファイルを開く関数.
//================================================================================
bool Sound::Open( LPCTSTR sFileName, LPCTSTR sAlias, HWND hWnd )
{
	//初期設定.
	SetInitParam( sAlias, hWnd );

	//コマンド.
	TCHAR cmd[STR_BUFF_MAX] = _T("");

	wsprintf( cmd, _T("open %s alias %s"), sFileName, m_sAlias );

	if( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//================================================================================
//  音声ファイルを閉じる関数.
//================================================================================
bool Sound::Close()
{
	//コマンド.
	TCHAR cmd[STR_BUFF_MAX] = _T("");

	wsprintf( cmd, _T("close %s"), m_sAlias );

	if( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//==============================================================================
//  音声を再生する関数.
//==============================================================================
bool Sound::Play( bool bNotify )
{
	//コマンド.
	TCHAR cmd[STR_BUFF_MAX] = _T("");

	if( bNotify == true ){
		wsprintf( cmd, _T("play %s notify"), m_sAlias ); //notify:音声の状態取得で必要.
	}
	else{
		wsprintf( cmd, _T("play %s"), m_sAlias );
	}

	if( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//==============================================================================
//  音声を再生する(SEで使う).
//==============================================================================
bool Sound::PlaySE()
{
	SeekToStart();
	if( Play() == true ){
		return true;
	}
	return false;
}


//==============================================================================
//  音声を再生する(ループ).
//==============================================================================
bool Sound::PlayLoop()
{
	if( IsStopped() == true ){
		SeekToStart();
		if( Play( true ) == true ){
			return true;
		}
	}
	return false;
}


//==============================================================================
//  音声を停止する関数.
//==============================================================================
bool Sound::Stop()
{
	//コマンド.
	TCHAR cmd[STR_BUFF_MAX] = _T("");

	wsprintf( cmd, _T("stop %s"), m_sAlias );

	if( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//==============================================================================
//  音声の状態を取得する関数.
//      sStatus の 配列数は 256 以下にすること.
//  ※ただし、状態を取得する場合は、再生時に「notify」を設定し、
//    ウィンドウハンドルにメッセージを送っておく必要がある.
//==============================================================================
bool Sound::GetStatus(LPTSTR sStatus )
{
	//コマンド.
	TCHAR cmd[STR_BUFF_MAX] = _T("");

	wsprintf( cmd, _T("status %s mode"), m_sAlias );

	if( mciSendString( cmd, sStatus, STR_BUFF_MAX, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//==============================================================================
//  音声の停止を確認する関数.
//==============================================================================
bool Sound::IsStopped()
{
	TCHAR sStatus[STR_BUFF_MAX] = _T("");

	// 状態の取得.
	if( GetStatus( sStatus ) ){
		// 文字列比較.
		if( lstrcmp( sStatus, _T("stopped") ) == 0 ){
			return true;
		}
	}
	return false;
}


//==============================================================================
//  音声の再生位置を最初にする関数.
//==============================================================================
bool Sound::SeekToStart()
{
	//コマンド.
	TCHAR cmd[STR_BUFF_MAX] = _T("");

	wsprintf( cmd, _T("seek %s to start"), m_sAlias );

	if( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}


//==============================================================================
//  音声の音量を設定する関数.
//==============================================================================
bool Sound::SetVolume( int iVolume )
{
	//範囲内に丸める.
	if( iVolume < 0 )       { iVolume = 0; }
	if( iVolume > 1000 )    { iVolume = 1000; }

	//メンバ変数に設定.
	m_iVolume = iVolume;

	//コマンド.
	TCHAR cmd[STR_BUFF_MAX] = _T("");

	wsprintf( cmd, _T("setaudio %s volume to %d"), m_sAlias, m_iVolume );

	if( mciSendString( cmd, nullptr, 0, m_hWnd ) == 0 ){
		return true;
	}
	return false;
}

//==============================================================================
//	初期値の設定する関数.
//==============================================================================
void Sound::SetInitParam( LPCTSTR sAlias, HWND hWnd )
{
	//ウィンドウハンドルを登録.
	m_hWnd = hWnd;

	//エイリアスを登録(文字列のコピー).
	lstrcpy( m_sAlias, sAlias );
}