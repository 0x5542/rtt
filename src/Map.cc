#include "Map.h"


Map::Map(int width_, int height_) {

	srand((unsigned)time(NULL));

	width = width_;
	height = height_;

	/*
	// create random map
	int k;
	Vec2f p1, p2, p3;
	for(k=0; k<80; ++k) {
		p1 = Vec2f(width * (rand() / double(RAND_MAX)), height * (rand() / double(RAND_MAX)));
		p2 = p1.add(Vec2f( 100*(2*(rand() / double(RAND_MAX))-1), 100*(2*(rand() / double(RAND_MAX))-1) ));
		p3 = p1.add(Vec2f( 100*(2*(rand() / double(RAND_MAX))-1), 100*(2*(rand() / double(RAND_MAX))-1) ));
		triangles.push_back(Triangle(p1, p2, p3));
	}
	*/
}


Map::~Map() {
	triangles.clear();
}


vector<Triangle>* Map::getTriangles() {
	return &triangles;
}


bool Map::isInside(Vec2f p_) {
	for(int k=0; k<triangles.size(); ++k) {
		if(triangles[k].isInside(p_)) return true;
	}
	return false;
}


bool Map::hasIntersectionWithLine(Vec2f origin_, Vec2f end_) {
	for(int k=0; k<triangles.size(); ++k) {
		if(triangles[k].hasIntersectionWithLine(origin_, end_)) return true;
	}
	return false;
}

void Map::clear() {
	triangles.clear();
}

void Map::addRectangle(Vec2f a_, Vec2f b_) {
	Vec2f dx = Vec2f(b_.x - a_.x, 0);
	Vec2f dy = Vec2f(0, b_.y - a_.y);

	Vec2f p1 = a_;
	Vec2f p2 = p1.add(dx);
	Vec2f p3 = b_;
	Vec2f p4 = p1.add(dy);

	triangles.push_back(Triangle(p1, p2, p3));
	triangles.push_back(Triangle(p3, p4, p1));
}


int Map::getWidth() {
	return width;
}

int Map::getHeight() {
	return height;
}