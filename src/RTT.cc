#include "RTT.h"


RTT::RTT(Map *map_, float stepSize_) {

	srand((unsigned)time(NULL));

	map = map_;
	searching = false;

	stepSize = stepSize_;
	counter = 0;
}

RTT::~RTT() {

}


void RTT::setStartPoint(Vec2f start_) {
	start = start_;
}

void RTT::setEndPoint(Vec2f end_) {
	end = end_;
}


bool RTT::isReady() {
	if(start.norm() != 0 && end.norm() != 0) return true;
	return false;
}

bool RTT::isSearching() {
	return searching;
}

bool RTT::hasFoundPath() {
	return pathFound;
}

vector<TreeElement>* RTT::getStartTree() {
	return &startTree;
}

vector<TreeElement>* RTT::getEndTree() {
	return &endTree;
}

vector<Vec2f>* RTT::getPath() {
	return &path;
}

void RTT::reset() {
	searching = false;
	pathFound = false;
	startTree.clear();
	endTree.clear();
	path.clear();
	counter = 0;
}

void RTT::startSearch() {
	reset();
	searching = true;
	startTree.push_back(TreeElement(start));
	endTree.push_back(TreeElement(end));

}




void RTT::nextSearchStep() {

	Vec2f qnew = Vec2f(map->getWidth() * (rand() / double(RAND_MAX)), map->getHeight() * (rand() / double(RAND_MAX)));

	int k;
	float dist = 99999;

	vector<TreeElement> *tree;
	vector<TreeElement> *otherTree;
	if(counter % 2 == 0) {
		tree = &startTree;
		otherTree = &endTree;
	} else {
		tree = &endTree;
		otherTree = &startTree;
	}


	// check tree
	float minDist = 99999;
	int minDistId = 0;
	for(k=0; k<tree->size(); ++k) {
		dist = qnew.sub(tree->at(k).position).norm();
		if(dist < minDist) {
			minDistId = k;
			minDist = dist;
		}
	}

	Vec2f dir = qnew.sub(tree->at(minDistId).position).normalize();
	qnew = tree->at(minDistId).position.add(dir.mult(stepSize));
	if(!map->hasIntersectionWithLine(tree->at(minDistId).position, qnew)) {
		tree->push_back(TreeElement(qnew, minDistId));

		minDist = 99999;
		minDistId = 0;
		for(k=0; k<otherTree->size(); ++k) {
			dist = qnew.sub(otherTree->at(k).position).norm();
			if(dist < minDist) {
				minDistId = k;
				minDist = dist;
			}
		}
		if(minDist < stepSize) {

			if(!map->hasIntersectionWithLine(otherTree->at(minDistId).position, qnew)) {

				pathFound = true;
				searching = false;

				if(counter % 2 == 0) {
					generatePath(tree->at(tree->size()-1), otherTree->at(minDistId));
				} else {
					generatePath(otherTree->at(minDistId), tree->at(tree->size()-1));
				}

			}

		}
	}

	counter++;
}


void RTT::generatePath(TreeElement partOfStartTree_, TreeElement partOfEndTree_) {

	int k;
	TreeElement el;
	vector<Vec2f> startPart;
	vector<Vec2f> endPart;

	k = -1;
	el = partOfStartTree_;
	while(k!=0) {
		 startPart.push_back(el.position);
		 k = el.predecessor;
		 el = startTree[k];
	}
	startPart.push_back(startTree[0].position);

	k = -1;
	el = partOfEndTree_;
	while(k!=0) {
		 endPart.push_back(el.position);
		 k = el.predecessor;
		 el = endTree[k];
	}
	endPart.push_back(endTree[0].position);

	reverse(startPart.begin(), startPart.end());
	for(k=0; k<startPart.size(); ++k) {
		path.push_back(startPart[k]);
	}
	for(k=0; k<endPart.size(); ++k) {
		path.push_back(endPart[k]);
	}
}