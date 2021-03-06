﻿#include "source/galaxy/galaxycomponents.h"
#include "math.h"
#include "source/util/util.h"

void GalaxyComponentBulge::componentIntensity(RasterPixel* rp, QVector3D& p, float ival) {
    double rho_0 = m_componentParams.strength()*ival;
    QVector3D pos = m_currentGI->rotmat()*(p);

    double rad = (pos.length()+0.01)*m_componentParams.r0();
    rad+=0.01;
    float i =    rho_0 * (pow(rad,-0.855)*exp(-pow(rad,1/4.0f)) -0.05f) *m_currentGI->intensityScale();
//    double i = rho_0*1/rad*0.5;

    if (i<0) i=0;
    rp->setI(rp->I() + i*m_spectrum->spectrum()*rp->scale);
    rp->tmp = i;
}

float GalaxyComponentBulge::calculateIntensity(RasterPixel* rp, QVector3D& p, GalaxyInstance* gi, float weight) {
    m_currentGI = gi;
//    m_componentParams.setZ0(0.25);
    componentIntensity(rp, p, weight);
    return rp->tmp;
}

float GalaxyComponentBulge::getHeightModulation(float height)
{
    return 1;
    float rho_0 = m_componentParams.strength();
    float rad = (height+0.01f)*m_componentParams.r0();

    return rho_0 * (pow(rad,-0.855)*exp(-pow(rad,1/4.0f)) -0.05f)*0.1;
}

float GalaxyComponentBulge::getRadius(const QVector3D &p, QVector3D &P, float &dott, GalaxyInstance *gi)
{
    return 0;
}

void GalaxyComponentDisk::componentIntensity(RasterPixel* rp, QVector3D& p, float ival) {
    float p2 = 0.5;
    if (ival<0.0005)
        return;
    p2 =abs(getPerlinCloudNoise(p, rp->winding, 10, m_componentParams.scale(), m_componentParams.ks()));

    p2 = fmax (p2, 0.01f);
    //if (p2!=0)
//        p2=0.1/p2;

    p2 = pow(p2,m_componentParams.noiseTilt());

    p2+=m_componentParams.noiseOffset();

    if (p2<0)
        return;

    rp->setI( rp->I() + ival*p2*m_spectrum->spectrum()*rp->getScale());
}




void GalaxyComponentDust::componentIntensity(RasterPixel* rp, QVector3D& p, float ival) {
    if (ival<0.0005)
        return;

    float p2 = getPerlinCloudNoise(p, rp->winding, 9, m_componentParams.scale() , m_componentParams.ks());
    //if (p2!=0)
//    p2=0.1f/(abs(p2) + 0.1f);

    p2 = max(p2 - m_componentParams.noiseOffset(),0.0f);
/*    if (p2<0)
        return;*/
    p2 = Util::clamp(pow(5*p2,m_componentParams.noiseTilt()),-10,10);

    float s = 0.01;

    rp->I().setX(rp->I().x()*exp(-p2*ival*m_spectrum->spectrum().x()*s));
    rp->I().setY(rp->I().y()*exp(-p2*ival*m_spectrum->spectrum().y()*s));
    rp->I().setZ(rp->I().z()*exp(-p2*ival*m_spectrum->spectrum().z()*s));

  //  rp->dust += ival*p2*m_spectrum->spectrum()*rp->getScale();
}
void GalaxyComponentDust2::componentIntensity(RasterPixel* rp, QVector3D& p, float ival) {
    if (ival<0.0005)
        return;

    float scale = m_componentParams.scale();
    QVector3D r = twirl(p, rp->winding);

    float p2 = m_galaxyParams->noise()->getRidgedMf(r*scale,
      m_componentParams.ks(),9,2.5,
      m_componentParams.noiseOffset(),m_componentParams.noiseTilt());  //getPerlinCloudNoise(p, rp->winding, 9, m_componentParams.scale() , m_componentParams.ks());
//    p2 = abs(p2);
    p2 = max(p2,0.0f);//abs(p2);

    //if (p2!=0)
//    p2=0.1f/(abs(p2) + 0.1f);

//    p2 = max(p2 - m_componentParams.noiseOffset(),0.0f);
/*    if (p2<0)
        return;*/
//    p2 = Util::clamp(pow(5*p2,m_componentParams.noiseTilt()),-10,10);

    float s = 0.01;

    rp->I().setX(rp->I().x()*exp(-p2*ival*m_spectrum->spectrum().x()*s));
    rp->I().setY(rp->I().y()*exp(-p2*ival*m_spectrum->spectrum().y()*s));
    rp->I().setZ(rp->I().z()*exp(-p2*ival*m_spectrum->spectrum().z()*s));

//    rp->dust += ival*p2*m_spectrum->spectrum()*rp->getScale();

}

void GalaxyComponentDustPositive::componentIntensity(RasterPixel* rp, QVector3D& p, float ival) {
    if (ival<0.0005)
        return;

    float scale = m_componentParams.scale();
    QVector3D r = twirl(p, rp->winding);

    float p2 = m_galaxyParams->noise()->getRidgedMf(r*scale,
      m_componentParams.ks(),9,2.5,
      m_componentParams.noiseOffset(),m_componentParams.noiseTilt());  //getPerlinCloudNoise(p, rp->winding, 9, m_componentParams.scale() , m_componentParams.ks());
    p2 = max(p2,0.0f);//abs(p2);

    rp->setI( rp->I() + ival*p2*m_spectrum->spectrum()*rp->getScale());

    float s = 0.001;

/*    rp->I().setX(rp->I().x()*exp(-p2*ival*m_spectrum->spectrum().x()*s));
    rp->I().setY(rp->I().y()*exp(-p2*ival*m_spectrum->spectrum().y()*s));
    rp->I().setZ(rp->I().z()*exp(-p2*ival*m_spectrum->spectrum().z()*s));
*/
}


void GalaxyComponentStars::componentIntensity(RasterPixel* rp, QVector3D& r, float ival) {

    float perlinnoise = abs(m_galaxyParams->noise()->octave_noise_3d(10,m_componentParams.ks(),0.01f*m_componentParams.scale()*100,r.x(),r.y(),r.z()));

    float addNoise = 0;
    if (m_componentParams.noiseOffset() != 0)
    {
        addNoise = (m_componentParams.noiseOffset() * getPerlinCloudNoise(r,  rp->winding, 4, 2, -2));
        addNoise += 0.5f * (m_componentParams.noiseOffset() * getPerlinCloudNoise(r,  rp->winding * 0.5f, 4, 4, -2));
    }
    float val = abs(pow(perlinnoise+1 + addNoise,m_componentParams.noiseTilt()));
    //            val = perlinnoise;

    rp->setI(rp->I() +  ival * val * m_spectrum->spectrum() *rp->scale);


}



void GalaxyComponentStarsSmall::componentIntensity(RasterPixel* rp, QVector3D& r, float ival) {


    if (rand()%(int)m_componentParams.scale()==0) {
        float val = pow((rand()%10),m_componentParams.noiseTilt());
        rp->setI(rp->I() +  ival * val * m_spectrum->spectrum() *rp->scale);
        return;
    }
    return;


}






