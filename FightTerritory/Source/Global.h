#pragma once

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning(disable:4005)
#include <Windows.h>
#include <crtdbg.h>

#include <D3DX9.h>
//DiewctX11
#include <D3DX11.h>
#include <D3D11.h>
//DiewctX10
#include <D3DX10.h>	//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include <memory>
#include <vector>

#include "MyMacro.h"

//ライブラリ読み込み.
#pragma comment( lib, "winmm.lib" )
//DiewctX9
#pragma comment( lib , "d3dx9.lib" )
#pragma comment( lib , "d3d9.lib" )
//DiewctX11
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
//DiewctX10
#pragma comment( lib, "d3dx10.lib" )	//「D3DX～」の定義使用時に必要.

//=================================================
//	定数.
//=================================================
//#define WND_W 1280	//←定数宣言で#defineは使わない.
const int WND_W = 1280;	//ウィンドウの幅.
const int WND_H = 720;	//ウィンドウの高さ.
const int FPS = 60;		//フレームレート.

//弾の最大数.
static const int ShotMax = 100;

//=================================================
//	構造体.
//=================================================
//カメラ情報構造体.
struct CAMERA
{
	D3DXVECTOR3	vPosition;	//視点.
	D3DXVECTOR3 vLook;		//注視点.
};
//ライト情報.
struct LIGHT
{
	D3DXVECTOR3	Position;	//位置.
	D3DXVECTOR3	vDirection;	//方向.
	D3DXMATRIX	mRotation;	//回転行列.
	float		fIntensity;	//強度(明るさ).
};

