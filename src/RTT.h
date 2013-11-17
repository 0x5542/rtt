#ifndef RTT_H
#define RTT_H

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include "Map.h"
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <algorithm>

#include "datatypes.h"


class RTT {




	public:

		RTT(Map *map_, float stepSize_);
		~RTT();

		void setStartPoint(Vec2f start_);
		void setEndPoint(Vec2f end_);
		bool isReady();
		bool isSearching();
		bool hasFoundPath();
		void startSearch();
		void nextSearchStep();
		void reset();

		vector<TreeElement>* getStartTree();
		vector<TreeElement>* getEndTree();
		vector<Vec2f>* getPath();

	private:

		void generatePath(TreeElement partOfStartTree_, TreeElement partOfEndTree_);

		float stepSize;
		int counter;

		Map *map;
		Vec2f start;
		Vec2f end;
		bool searching;
		bool pathFound;

		vector<TreeElement> startTree;
		vector<TreeElement> endTree;

		vector<Vec2f> path;
};


#endif