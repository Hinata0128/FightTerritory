#pragma once
#include <d3dx10math.h>

class Camera {
public:
    static Camera& GetInstance() 
    {
        static Camera instance; 
        return instance;
    }

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    void Update(float mouseDx, float mouseDy, const D3DXVECTOR3& targetPos);

    const D3DXMATRIX& GetViewMatrix() const { return m_viewMatrix; }
    D3DXVECTOR3 GetForward() const;
    D3DXVECTOR3 GetRight() const;

    void SetOffset(const D3DXVECTOR3& offset) { m_offset = offset; }

    void SetActive(bool active) { m_isActive = active; }
    bool IsActive() const { return m_isActive; }
private:
    Camera();
    ~Camera() {}

    D3DXMATRIX  m_viewMatrix;
    D3DXVECTOR3 m_offset;     
    float       m_angleX;     
    float       m_angleY;      
    float       m_sensitivity;

    bool m_isActive;
};