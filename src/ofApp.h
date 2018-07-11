#pragma once

#include "ofMain.h"
#include "Ql.h"
#include "ofxXmlSettings.h"
#include "ofEvents.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void setupSystem();

		vector <Ql> ql;

		int totalAgents;
		vector <int> agentsDone;
		vector <float> refreshTime;

		int N_STATES;
		vector <string> ACTIONS;
		float EPSILON;
		float ALPHA;
		float GAMMA;
		int MAX_EPISODES;
		float REFRESH_TIME_MIN;
		float REFRESH_TIME_MAX;
		float EPISODE_TIME;

		int initPosX, initPosY, cellSize;
		int xSize, ySize;
		float lineWidth;
		ofColor lines;

		string direction;

		int w, h;

		bool runSystem;

		ofTrueTypeFont font;
		int fontSize;

		//qtable data
		vector <vector <float>> qTable;

		ofxXmlSettings XML;

		//serial com
		string PORT;
		int BAUD;

		ofArduino ard;
		bool bSetupArduino;
		int totalMotors;
		bool useMotors;
		bool callibrateMotors;

		void setupArduino(const int & version);
		void updateArduino();

};
