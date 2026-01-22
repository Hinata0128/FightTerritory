#pragma once

class Player;
class SkinMesh;

/**************************************************************
*	プレイヤーコンテキストクラス.
*   ToDo : プレイヤーのアニメーション再生とうに使用する
**/

class PlayerContext
{
public:
    D3DXVECTOR3&                Position;
    D3DXVECTOR3&                Rotation;
    D3DXVECTOR3&                Scale;
    int&                        AnimNo;
    double&                     AnimTime;
    double&                     AnimSpeed;
    SkinMesh*                   Mesh;
    LPD3DXANIMATIONCONTROLLER   AnimCtrl;
    D3DXVECTOR3&                BonePos;
    
    PlayerContext(Player* player);
private:
};