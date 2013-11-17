#ifndef DATATYPES_H
#define DATATYPES_H

#include <cmath>
#include <iostream>


using namespace std;


struct Vec2f {
   float x,y;
   Vec2f() {
		x = y = 0;
   }
   Vec2f(float x_, float y_) {
          x = x_;
          y = y_;
   }
   Vec2f add(Vec2f v_) {
        return Vec2f(x+v_.x, y+v_.y);
   }
   Vec2f sub(Vec2f v_) {
        return Vec2f(x-v_.x, y-v_.y);
   }
   Vec2f mult(float s_) {
        return Vec2f(x*s_, y*s_);
   }
   float dot(Vec2f v_) {
        return x*v_.x + y*v_.y;
   }
   float norm() {
         return sqrt(x*x + y*y);
   }
   Vec2f normalize() {
        float length = norm();
        return Vec2f(x / length, y / length);
   }
};



struct Triangle {

	Vec2f p1, p2, p3;

	Triangle() {

	}

	Triangle(Vec2f p1_, Vec2f p2_, Vec2f p3_) {
		p1 = p1_;
		p2 = p2_;
		p3 = p3_;
	}

	bool isInside(Vec2f p_) {

		Vec2f p1p2 = p2.sub(p1);
		Vec2f p1p3 = p3.sub(p1);
		Vec2f p = p_.sub(p1);

		float a,b,c,d,e,f,det,lambda,mu;

		a = p1p2.x;
		b = p1p3.x;
		c = p1p2.y;
		d = p1p3.y;
		e = p.x;
		f = p.y;
		det = a*d-b*c;

		if(det != 0) {

			lambda = (d * e - b * f) / det;
			mu =     (a * f - c * e ) / det;

			if(lambda>=0 && mu>= 0 && lambda+mu <= 1) {
				return true;
			}
		}


		return false;
	}

	bool isInBetweenNr(float p_, float p1_, float p2_) {
	  if((p_ >= p1_ && p_ <= p2_) || (p_ <= p1_ && p_ >= p2_)) return true;
	  return false;
	}

	float lineLineIntersection(Vec2f line0Origin_, Vec2f line0Direction_, Vec2f line1Origin_, Vec2f line1Direction_) {
		float a,b,c,d,e,f,ia,ib,det;
		a = line0Direction_.x;
		c = line0Direction_.y;
		b = -line1Direction_.x;
		d = -line1Direction_.y;
		e = line1Origin_.x - line0Origin_.x;
		f = line1Origin_.y - line0Origin_.y;
		det = a*d-b*c;
		if(det!=0) {
			ia = d / det;
			ib = -b / det;
			return ia * e + ib * f;
		} else {
			return -1.0;
		}
	}

	bool hasIntersectionWithLine(Vec2f origin_, Vec2f end_) {

		bool ibx0, iby0, ibx1, iby1;
		Vec2f dir_ = end_.sub(origin_).normalize();

		float iParam1 = lineLineIntersection( origin_, dir_, p1, p2.sub(p1).normalize());
		Vec2f iPoint1 = origin_.add(dir_.mult(iParam1));
	 	ibx0 = isInBetweenNr(iPoint1.x, p1.x, p2.x);
	 	iby0 = isInBetweenNr(iPoint1.y, p1.y, p2.y);
	 	ibx1 = isInBetweenNr(iPoint1.x, origin_.x, end_.x);
	 	iby1 = isInBetweenNr(iPoint1.y, origin_.y, end_.y);
		if(ibx0 && iby0 && ibx1 && iby1) return true;

		float iParam2 = lineLineIntersection( origin_, dir_, p2, p3.sub(p2).normalize());
		Vec2f iPoint2 = origin_.add(dir_.mult(iParam2));
	 	ibx0 = isInBetweenNr(iPoint2.x, p2.x, p3.x);
	 	iby0 = isInBetweenNr(iPoint2.y, p2.y, p3.y);
	 	ibx1 = isInBetweenNr(iPoint2.x, origin_.x, end_.x);
	 	iby1 = isInBetweenNr(iPoint2.y, origin_.y, end_.y);
		if(ibx0 && iby0 && ibx1 && iby1) return true;

		float iParam3 = lineLineIntersection( origin_, dir_, p3, p1.sub(p3).normalize());
		Vec2f iPoint3 = origin_.add(dir_.mult(iParam3));
	 	ibx0 = isInBetweenNr(iPoint3.x, p3.x, p1.x);
	 	iby0 = isInBetweenNr(iPoint3.y, p3.y, p1.y);
	 	ibx1 = isInBetweenNr(iPoint3.x, origin_.x, end_.x);
	 	iby1 = isInBetweenNr(iPoint3.y, origin_.y, end_.y);
		if(ibx0 && iby0 && ibx1 && iby1) return true;

		return false;
	}

};



struct TreeElement {
	Vec2f position;
	int predecessor;

	TreeElement() {
		predecessor = 0;
	}

	TreeElement(Vec2f pos_) {
		position = pos_;
		predecessor = 0;
	}

	TreeElement(Vec2f pos_, int predecessor_) {
		position = pos_;
		predecessor = predecessor_;
	}

};


#endif