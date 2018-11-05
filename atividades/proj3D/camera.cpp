#include "camera.h"

Camera::Camera()
{
    projectionMatrix.setToIdentity();
    computeViewMatrix();
}

void Camera::computeViewMatrix()
{
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(eye, center, up);
}

void Camera::resizeViewport(int width, int height)
{
    projectionMatrix.setToIdentity();
    float aspectRatio = static_cast<float>(width) /
            static_cast<float>(height);

    // Orthographic projection
    // projectionMatrix.ortho(-1, 1, -1, 1, 0, 2);

    // Perspective projection
    projectionMatrix.perspective(60, aspectRatio, 0.1f, 20.0f);
}

void Camera::moveForward(float amount)
{
    // Compute forward vector
    QVector3D forward = (center - eye).normalized();

    // Move eye and center by forward vector * amount
    eye += forward * amount;
    center += forward * amount;

    // Call computeViewMatrix
    computeViewMatrix();
}

void Camera::turn(float amount)
{
    QMatrix4x4 transform;
    transform.setToIdentity();
    transform.translate(eye); // T(d)
    transform.rotate(-amount, up); // Ry(theta)
    transform.translate(-eye); // T(-d)

    center = transform * center;

    computeViewMatrix();
}

void Camera::strafe(float amount)
{
    QVector3D forward = (center - eye).normalized();
    QVector3D left = QVector3D::crossProduct(up, forward).normalized();

    eye -= left * amount;
    center -= left * amount;
}
