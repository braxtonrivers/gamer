﻿#include <QMatrix3x3>
#include "source/galaxy/gamercamera.h"
#include "math.h"

QVector3D GamerCamera::camera() const
{
    return m_camera;
}

void GamerCamera::setCamera(const QVector3D &camera)
{
    m_camera = camera;
}

QVector3D GamerCamera::target() const
{
    return m_target;
}

void GamerCamera::setTarget(const QVector3D &target)
{
    m_target = target;
}

QVector3D GamerCamera::up() const
{
    return m_up;
}

void GamerCamera::setUp(const QVector3D &up)
{
    m_up = up;
}

float GamerCamera::perspective() const
{
    return m_perspective;
}

void GamerCamera::setPerspective(float perspective)
{
    m_perspective = perspective;
}

QMatrix4x4 GamerCamera::rotMatrix() const
{
    return m_rotMatrix;
}

void GamerCamera::setRotMatrix(const QMatrix4x4 &rotMatrix)
{
    m_rotMatrix = rotMatrix;
}

void GamerCamera::setRotation(QVector3D& v) {
    QMatrix3x3 rot = QQuaternion::fromEulerAngles(v).toRotationMatrix();
    m_rotMatrix.setToIdentity();
    for (int i=0;i<3;i++)
        for (int j=0;j<3;j++)
            m_rotMatrix(i,j) = rot(i,j);
//    m_rotMatrix = m_rotMatrix*rot;
    //QMatrix4x4::TRS(Vector3.zero, Quaternion.EulerAngles(v), Vector3.one);
    setupViewmatrix();
}


GamerCamera* GamerCamera::copy() {
    GamerCamera* g = new GamerCamera();
    g->setCamera(m_camera);
    g->setTarget(m_target);
    g->setPerspective(m_perspective);
    g->setRotMatrix(m_rotMatrix);
    g->setUp(m_up);
    return g;
}


GamerCamera::GamerCamera() {
    m_rotMatrix.setToIdentity();
}

void GamerCamera::TranslateXY(QVector3D& delta) {
    QVector3D right = QVector3D::crossProduct((m_camera-m_target), m_up).normalized();
    QVector3D d =  -delta.y()*m_up.normalized() + right*delta.x();
    m_camera = m_camera + d;
    m_target = m_target + d;
}

void GamerCamera::ZoomXY(QVector3D& delta) {
    m_camera = m_camera - (m_camera-m_target).normalized()*delta.y();
}

void GamerCamera::RotateUp(float r) {
    QVector3D d = (m_camera-m_target).normalized();
    QVector3D right = QVector3D::crossProduct(d, m_up).normalized();
    m_up = QVector3D::crossProduct(right, d).normalized();
    QQuaternion q = QQuaternion::fromAxisAndAngle(d, r);
    m_up = q*m_up;
}


/*void GamerCamera::RotateXY(QVector3D& delta) {
    //gamer.rast.RP.camera.RotateVertical(delta.x*10);
    // gamer.rast.RP.camera.RotateHorisontal(delta.y*10);

}
*/
QMatrix4x4 GamerCamera::GetRotationMatrix() {
    QVector3D zaxis = (m_camera-m_target).normalized();
    QVector3D xaxis = (QVector3D::crossProduct(m_up, zaxis)).normalized()*-1;
    QVector3D yaxis = (QVector3D::crossProduct(zaxis, xaxis)).normalized();

    QMatrix4x4 M;
    M.setToIdentity();
    M(0,0) = xaxis.x();
    M(0, 1) = yaxis.x();
    M(0, 2) = zaxis.x();

    M(1, 0) = xaxis.y();
    M(1, 1) = yaxis.y();
    M(1, 2) = zaxis.y();

    M(2, 0) = xaxis.z();
    M(2, 1) = yaxis.z();
    M(2, 2) = zaxis.z();

    M(3, 0) = 0;
    M(3, 1) = 0;
    M(3, 2) = 0;

    return M;

}


/*
void UpdateMaterial(Material m)
{
}

*/
void GamerCamera::setupViewmatrix() {
    QVector3D zaxis = (m_target-m_camera).normalized();
    QVector3D xaxis = (QVector3D::crossProduct(m_up,zaxis)).normalized();
    QVector3D yaxis = (QVector3D::crossProduct(zaxis,xaxis)).normalized();

    QMatrix4x4 M;
    M.setToIdentity();
    M(0,0) = xaxis.x();
    M(0,1) = yaxis.x();
    M(0,2) = zaxis.x();

    M(1,0) = xaxis.y();
    M(1,1) = yaxis.y();
    M(1,2) = zaxis.y();

    M(2,0) = xaxis.z();
    M(2,1) = yaxis.z();
    M(2,2) = zaxis.z();

    M(3,0) = -m_camera.x();
    M(3,1) = -m_camera.y();
    M(3,2) = -m_camera.z();

    //            Debug.Log(rotMatrix);

    m_viewMatrix = m_rotMatrix*M;


}

void GamerCamera::RotateVertical(float angle) {
    QVector3D d = m_camera - m_target;
    QVector3D side = QVector3D::crossProduct( m_up, d);
    QQuaternion q = QQuaternion::fromAxisAndAngle(side,angle);
    m_camera = q*(m_camera-m_target) + m_target;

    d = m_camera - m_target;
    m_up = QVector3D::crossProduct(d, side).normalized();
}
void GamerCamera::RotateHorisontal(float angle) {
    QVector3D d = m_camera - m_target;
    QVector3D side = QVector3D::crossProduct(m_up, d).normalized();
    QQuaternion q = QQuaternion::fromAxisAndAngle(m_up, angle);
    m_camera = q * (m_camera - m_target) + m_target;
    d = m_camera - m_target;
    m_up = QVector3D::crossProduct(d, side).normalized();
}

QVector3D GamerCamera::coord2ray(float x, float y, float width, float height) {

    float aspect_ratio = 1;
    float FOV = m_perspective / 360.0f * 2 * M_PI; // convert to radians
    float dx=tan(FOV*0.5f)*(x/(width/2)-1.0f)/aspect_ratio;
    float dy=tan(FOV*0.5f)*(1- y/(width/2));


    float far = 10;

    QVector3D Pfar = QVector3D(dx*far, dy*far, far);
    QVector3D res = m_viewMatrix*Pfar;
    res = res.normalized();

    return res;
}
