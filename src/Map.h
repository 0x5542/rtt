#ifndef MAP_H
#define MAP_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include "datatypes.h"


class Map {



	public:

		Map(int width_, int height_);
		~Map();

		vector<Triangle>* getTriangles();
		bool isInside(Vec2f p_);
		bool hasIntersectionWithLine(Vec2f origin_, Vec2f end_);
		void clear();
		void addRectangle(Vec2f a_, Vec2f b_);
		int getWidth();
		int getHeight();

	private:

		int width, height;

		vector<Triangle> triangles;


};


#endif