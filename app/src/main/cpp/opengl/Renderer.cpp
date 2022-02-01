#include "Renderer.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Raydelto::MD2Loader;

Renderer::Renderer() = default;

void Renderer::OnSurfaceCreated()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_player = std::make_unique<MD2Model>("female.md2", "female.tga");
    m_player2 = std::make_unique<MD2Model>("grunt.md2", "grunt.tga");
    m_player2->SetPosition(0.0f, 6.5f, -25.0f);



    // Create the projection matrix
    assert(m_height != 0);
    float aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height);
    m_projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    m_camPos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_targetPos = glm::vec3(0.0f, 0.0f, -20.0f);
    m_up = glm::vec3(1.0f, 0.0f, 0.0f);

    // Create the View matrix
    m_view = glm::lookAt(m_camPos, m_camPos + m_targetPos, m_up);
}

void Renderer::OnDrawFrame()
{
    static const int START_FRAME = 0;
    static const int END_FRAME = m_player->GetEndFrame();

    m_xAngle += m_rotationX;
    m_yAngle -= m_rotationY;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_player->Draw(m_renderFrame, m_xAngle, m_yAngle, m_scale, m_interpolation, m_view, m_projection);
    m_player2->Draw(m_renderFrame, m_xAngle, m_yAngle, m_scale, m_interpolation, m_view, m_projection);
    if (m_interpolation >= 1.0f)
    {
        m_interpolation = 0.0f;
        if (m_renderFrame == END_FRAME)
        {
            m_renderFrame = START_FRAME;
        }
        else
        {
            m_renderFrame++;
        }
    }
    m_interpolation += 0.1f;
}

void Renderer::SetScreenSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void Renderer::SetRotationAngles(float x, float y)
{
    m_rotationX = x;
    m_rotationY = y;
}

void Renderer::SetScale(float scale)
{
    m_scale = scale;
}
