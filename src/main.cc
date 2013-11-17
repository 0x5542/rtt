#include <windows.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include "Map.h"
#include "RTT.h"

#include "datatypes.h"


#define PI 3.1415926535

using namespace std;

int windowWidth = 800;
int windowHeight = 600;

Map map(windowWidth, windowHeight);
RTT rtt = RTT(&map, 10.0f);

int main(int argc, char ** argv);
void display();
void reshape(int width, int height);

void mouse(int button, int state, int mx, int my);
void motion (int x, int y);
void keyboardDown(unsigned char key, int x, int y);

Vec2f startPoint;
Vec2f endPoint;

bool drawing = false;
Vec2f startDrawing;
Vec2f endDrawing;



int mode = 1; // 1= setting points;

// *******************************************************



int main(int argc, char ** argv) {

	cout << "Press...\n";
	cout << "1 for setting points (left click: start, right click: end) \n";
	cout << "2 for drawing map elements \n";
	cout << "3 for clearing map";

	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow ("RTT");
    glEnable(GL_DEPTH_TEST);

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
    glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboardDown);

	glutMainLoop();

	return 0;
}







void display(void) {

	TreeElement e;
	Vec2f p1, p2;

	if(rtt.isSearching() && !rtt.hasFoundPath()) {
		rtt.nextSearchStep();
	}


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	int k;
	vector<Triangle>* triangles = map.getTriangles();
	glColor3f(0.4f, 0.4f, 0.4f);
	glBegin(GL_TRIANGLES);
	for(k=0; k<triangles->size(); ++k) {
		glVertex2f(triangles->at(k).p1.x, triangles->at(k).p1.y);
		glVertex2f(triangles->at(k).p2.x, triangles->at(k).p2.y);
		glVertex2f(triangles->at(k).p3.x, triangles->at(k).p3.y);
	}
	glEnd();


	if(drawing) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_QUADS);
			glVertex2f(startDrawing.x, startDrawing.y);
			glVertex2f(endDrawing.x, startDrawing.y);
			glVertex2f(endDrawing.x, endDrawing.y);
			glVertex2f(startDrawing.x, endDrawing.y);
		glEnd();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	if(rtt.hasFoundPath()) {
		glLineWidth(2.0f);
		vector<Vec2f> *path = rtt.getPath();
		glColor3f(0.2f, 0.2f, 0.2f);
		glBegin(GL_LINES);
		for(k=0; k<path->size()-1; ++k) {
			p1 = path->at(k);
			p2 = path->at(k+1);
			glVertex2f(p1.x, p1.y);
			glVertex2f(p2.x, p2.y);
		}
		glEnd();
	}
	glLineWidth(1.0f);


	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(startPoint.x, startPoint.y);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(endPoint.x, endPoint.y);
	glEnd();
	glPointSize(3.0f);



	// draw startTree lines
	glColor3f(1.0f, 0.0f, 0.0f);
	vector<TreeElement> *startTree = rtt.getStartTree();
	glBegin(GL_LINES);
	for(k=1; k<startTree->size(); ++k) {
		e = startTree->at(k);
		p1 = e.position;
		p2 = startTree->at(e.predecessor).position;
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();

	// draw startTree points
	glBegin(GL_POINTS);
	for(k=1; k<startTree->size(); ++k) {
		e = startTree->at(k);
		p1 = e.position;
		glVertex2f(p1.x, p1.y);
	}
	glEnd();

	// draw endTree lines
	glColor3f(0.0f, 0.0f, 1.0f);
	vector<TreeElement> *endTree = rtt.getEndTree();
	glBegin(GL_LINES);
	for(k=1; k<endTree->size(); ++k) {
		e = endTree->at(k);
		p1 = e.position;
		p2 = endTree->at(e.predecessor).position;
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
	}
	glEnd();

	// draw endTree points
	glBegin(GL_POINTS);
	for(k=1; k<endTree->size(); ++k) {
		e = endTree->at(k);
		p1 = e.position;
		glVertex2f(p1.x, p1.y);
	}
	glEnd();







	glutSwapBuffers();
	glutPostRedisplay();
}





void reshape(int width, int height) {
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void motion (int x, int y) {
	if(drawing) {
		endDrawing = Vec2f(x,y);
	}
}


void restart() {
	if(!map.isInside(startPoint) && !map.isInside(endPoint) && rtt.isReady()) {
		rtt.startSearch();
	}
}


void mouse(int button, int state, int x, int y) {
   switch( button ) {
       case GLUT_LEFT_BUTTON:
        	if( state == GLUT_DOWN ) {

				if(mode == 1) {
					Vec2f newStartPoint = Vec2f(x,y);
					if(!map.isInside(newStartPoint)) {
						startPoint = newStartPoint;
						rtt.setStartPoint(startPoint);
						restart();
					}
				}

				if(mode == 2) {
					startDrawing = Vec2f(x,y);
					endDrawing = Vec2f(x,y);
					drawing = true;
				}
			} else if(state == GLUT_UP ) {
				if(mode == 2) {
					endDrawing = Vec2f(x,y);
					drawing = false;
					map.addRectangle(startDrawing, endDrawing);
				}
			}



			break;
       case GLUT_RIGHT_BUTTON:
        	if( state == GLUT_DOWN ) {

				if(mode == 1) {
					Vec2f newEndPoint = Vec2f(x,y);
					if(!map.isInside(newEndPoint)) {
						endPoint = newEndPoint;
						rtt.setEndPoint(endPoint);
						restart();
					}
				}
			}
			break;

	}
}



void keyboardDown(unsigned char key, int x, int y) {
    switch (key) {
      case '1':
      {
           mode = 1;
           break;
      }
      case '2':
      {
			mode = 2;
            break;
      }
      case '3':
      {
			map.clear();
			rtt.reset();
            break;
      }
    }
}