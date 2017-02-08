﻿#pragma once

#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <math.h>

#include "source/galaxy/galaxy.h"
#include "source/galaxy/componentparams.h"
#include "source/galaxy/galaxyparams.h"
#include "source/galaxy/spectrum.h"
#include "source/galaxy/galaxyinstance.h"
#include "source/galaxy/rasterizer.h"


class GalaxyComponent {

protected:
    GalaxyParams* m_galaxyParams = nullptr;

    ComponentParams m_componentParams;
    ComponentSpectrum* m_spectrum = nullptr;
    float m_keep;
		
    float m_strength;
		
    float m_average, m_count;
		
    float m_bulgeDust;
		
    float m_noiseVal = 0;

    float m_workTheta, m_workRad, m_workWinding, m_workLength;
    GalaxyInstance* m_currentGI = nullptr;
    QVector3D m_dAdd = QVector3D(0,0,0);

		
    float m_winding;
    float m_currentRadius;
		

public:

    GalaxyComponent();
    void Initialize(ComponentParams* cp, GalaxyParams* gp);
    virtual void componentIntensity(RasterPixel* rp, QVector3D& p, float ival );
    virtual void calculateIntensity(RasterPixel* rp,  QVector3D& p,
                                    GalaxyInstance* gi, const float weight);


    void setGalaxyParam(GalaxyParams* gp);
    float cosh(const float x);
    float getHeightModulation(float height);
    float getRadialIntensity(const float rad);
    float getRadius(QVector3D p, QVector3D& P, float& dott, GalaxyInstance* gi);
    QVector3D twirl( QVector3D& p,  const float twirl);
    float getPerlinCloudNoise(QVector3D& p, const float rad, const float t, const int NN, const float ks, const float pers);
    float findDifference( float t1,  float t2);
    float calculateArmValue( float rad,  QVector3D P,  float armStrength);
    float getArm( float rad,  QVector3D p,  float disp);
    float getTheta( QVector3D p );
    float getWinding( float rad);

    ComponentParams getComponentParams() const;
};

