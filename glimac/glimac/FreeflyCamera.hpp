#pragma once

#include <vector>
#include "glm.hpp"

namespace glimac {

class FreeflyCamera {
public:
    FreeflyCamera()
        : m_Position(glm::vec3(0, 0, 5)), m_fPhi(glm::pi<float>()), m_fTheta(0.f)
    {
        computeDirectionVectors();
    };
    void moveLeft(float t)
    {
        this->m_Position.x += -t;
        this->computeDirectionVectors();
    };
    void moveFront(float t)
    {
        this->m_Position += -t * this->m_UpVector;
        this->computeDirectionVectors();
    };
    void rotateLeft(float degrees)
    {
        this->m_fPhi += -glm::radians(degrees);
        this->computeDirectionVectors();
    };
    void rotateUp(float degrees)
    {
        this->m_fTheta += -glm::radians(degrees);
        this->computeDirectionVectors();
    };
    glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(this->m_Position, glm::vec3(this->m_Position + this->m_FrontVector), this->m_UpVector);
    };

private:
    glm::vec3 m_Position;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;
    float     m_fPhi;
    float     m_fTheta;

    void computeDirectionVectors()
    {
        this->m_FrontVector = glm::vec3(glm::cos(this->m_fTheta) * glm::sin(this->m_fPhi), glm::sin(this->m_fTheta), glm::cos(this->m_fTheta) * glm::cos(this->m_fPhi));

        this->m_LeftVector = glm::vec3(glm::sin(this->m_fPhi + glm::half_pi<float>()), 0, glm::cos(this->m_fPhi) + glm::half_pi<float>());

        this->m_UpVector = glm::cross(this->m_FrontVector, this->m_LeftVector);
    };
};

} // namespace glimac