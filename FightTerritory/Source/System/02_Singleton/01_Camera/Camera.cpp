#include "Camera.h"
#include <cmath>

Camera::Camera()
    : m_angleX(0.0f)
    , m_angleY(0.0f)
    , m_sensitivity(0.0006f)
    , m_isActive(true)
{
    m_offset = D3DXVECTOR3(1.5f, 5.0f, -15.0f);
    D3DXMatrixIdentity(&m_viewMatrix);
}

void Camera::Update(float mouseDx, float mouseDy, const D3DXVECTOR3& targetPos)
{
    m_angleX += mouseDx * m_sensitivity;

    m_angleY = 0.0f;

    D3DXMATRIX matRot;
    D3DXMatrixRotationYawPitchRoll(&matRot, m_angleX, m_angleY, 0.0f);

    D3DXVECTOR4 transformedOffset;
    D3DXVec3Transform(&transformedOffset, &m_offset, &matRot);

    D3DXVECTOR3 cameraPos = targetPos + D3DXVECTOR3(transformedOffset.x, transformedOffset.y, transformedOffset.z);

    //注視点
    D3DXVECTOR3 lookAt = targetPos;
    lookAt.y += 1.5f; 

    D3DXVECTOR3 up(0, 1, 0);
    D3DXMatrixLookAtLH(&m_viewMatrix, &cameraPos, &lookAt, &up);
}

D3DXVECTOR3 Camera::GetForward() const
{
    return D3DXVECTOR3(sinf(m_angleX), 0.0f, cosf(m_angleX));
}

D3DXVECTOR3 Camera::GetRight() const 
{
    D3DXVECTOR3 forward = GetForward();
    D3DXVECTOR3 right;
    D3DXVECTOR3 up(0, 1, 0);
    D3DXVec3Cross(&right, &up, &forward);
    return right;
}