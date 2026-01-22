#include "SoundManager.h"
#include "MyMacro.h"

SoundManager::SoundManager()
	: m_pSound	()
{
	//インスタンス生成.
	for( int i = 0; i < enList::max; i++ )
	{
		m_pSound[i] = new Sound();
	}
}

SoundManager::~SoundManager()
{
	Release();

	//インスタンス破棄.
	for( int i = enList::max - 1; i >= 0; i-- )
	{
		SAFE_DELETE( m_pSound[i] );
	}
}

//サウンドデータ読込関数.
bool SoundManager::Load( HWND hWnd )
{
	struct SoundList
	{
		int listNo;				//enList列挙型を設定.
		const TCHAR path[256];	//ファイルの名前(パス付き).
		const TCHAR alias[32];	//エイリアス名.
	};
	SoundList SList[] =
	{
		{ enList::SE_Round,		_T( "Data\\Sound\\SE\\Round.mp3" ),				_T("SE_Round") },
		{ enList::SE_Select,		_T( "Data\\Sound\\SE\\Select.mp3" ),				_T("SE_Select") },
		{ enList::SE_Slash,		_T( "Data\\Sound\\SE\\Slash.mp3" ),				_T("SE_Slash") },
		{ enList::SE_PortalGet,		_T( "Data\\Sound\\SE\\PortalGet.mp3" ),				_T("SE_PortalGet") },
		{ enList::SE_Enter,		_T( "Data\\Sound\\SE\\Enter.mp3" ),				_T("SE_Enter") },
		{ enList::SE_Exp,		_T( "Data\\Sound\\SE\\Daed.mp3" ),			_T( "SE_Daed" ) },
		{ enList::SE_Shot,		_T( "Data\\Sound\\SE\\Shot.mp3" ),				_T( "SE_Shot" )},
		{ enList::BGM_Ending,	_T( "Data\\Sound\\BGM\\Win.mp3" ),			_T( "BGM_End"	)},
		{ enList::BGM_Over,		_T( "Data\\Sound\\BGM\\GameOver.mp3" ),			_T( "BGM_Over"	)},
		{ enList::BGM_Main,		_T( "Data\\Sound\\BGM\\Main.mp3" ),			_T( "BGM_Main"	)},
		{ enList::BGM_Title,	_T( "Data\\Sound\\BGM\\Title.mp3" ),			_T( "BGM_Title"	)},
	};
	//配列の最大要素数を算出 (配列全体のサイズ/配列1つ分のサイズ).
	int list_max = sizeof( SList ) / sizeof( SList[0] );
	for( int i = 0; i < list_max; i++ )
	{
		if( m_pSound[SList[i].listNo]->Open(
			SList[i].path,
			SList[i].alias,
			hWnd ) == false )
		{
			return false;
		}
		//音量の調節をしている.
		m_pSound[SList[i].listNo]->SetVolume(1000);
	}

	return true;
}

//サウンドデータ解放関数.
void SoundManager::Release()
{
	//開いた時と逆順で閉じる.
	for( int i = enList::max - 1; i >= 0; i-- )
	{
		if( m_pSound[i] != nullptr )
		{
			m_pSound[i]->Close();
		}
	}
}
