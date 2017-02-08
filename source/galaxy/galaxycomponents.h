﻿#pragma once

#include <QVector3D>
#include "source/galaxy/galaxy.h"
#include "source/galaxy/galaxycomponent.h"

class GalaxyComponentBulge : GalaxyComponent {
public:
    void componentIntensity(RasterPixel* rp, QVector3D& p, float ival) override;

    void calculateIntensity(RasterPixel* rp, QVector3D& p, GalaxyInstance* gi, float weight) override;
		
};

class GalaxyComponentDisk : GalaxyComponent {
	
    void componentIntensity(RasterPixel* rp, QVector3D& p, float ival) override;
};

class GalaxyComponentDust : GalaxyComponent {
		
    void componentIntensity(RasterPixel* rp, QVector3D& p, float ival) override;

};

class GalaxyComponentStars : GalaxyComponent {
		
    void componentIntensity(RasterPixel* rp, QVector3D& r, float ival) override;
		
};