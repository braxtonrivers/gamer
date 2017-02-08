#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <random>
#include <QVector3D>


using namespace std;

class Util {
  
 public:
  static void Tokenize(const string& str,
		       vector<string>& tokens,
		       const string& delimiters = " ");
  
  
  static void string2char(string s, char* to);
  static string toString(double d, string param);
  static string toString(double d);
  static string toString(int d);
  static const char* read_textfile(string filename);
  static void verify_file(string filename);
  static bool verify_file_bool(string filename);
  static string trim(string s);

  static float floatRandom(const float & min, const float & max) {
      static std::mt19937 generator;
      std::uniform_real_distribution<float> distribution(min, max);
      return distribution(generator);
  }

  static bool IntersectSphere(QVector3D o, QVector3D d, QVector3D r,QVector3D& isp1,QVector3D& isp2, float& t0, float& t1) {

      r.setX(1.0/(r.x()*r.x()));
      r.setY(1.0/(r.y()*r.y()));
      r.setZ(1.0/(r.z()*r.z()));


      QVector3D rD = QVector3D(d.x()*r.x(), d.y()*r.y(), d.z()*r.z());
      QVector3D rO = QVector3D(o.x()*r.x(), o.y()*r.y(), o.z()*r.z());


      float A = QVector3D::dotProduct(d,rD);
      float B = 2.0*(QVector3D::dotProduct(d, rO));
      float C = QVector3D::dotProduct(o, rO) - 1.0;

      float S = (B*B - 4.0f*A*C);

      if (S<=0) {
          isp1 = QVector3D(0,0,0);
          isp2 = QVector3D(0,0,0);
          t0 = 0;
          t1 = 0;
          return false;
      }

      t0 =  (-B - sqrt(S))/(2*A);
      t1 =  (-B + sqrt(S))/(2*A);

      isp1 = o+d*t0;
      isp2 = o+d*t1;

      return true;
  }

  static float smoothstep(float edge0, float edge1, float x);
  static float clamp(float val, float min, float max);

};
