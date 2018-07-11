#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setupArduino(const int & version){
	ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
	bSetupArduino = true;
	ofLogNotice() << ard.getFirmwareName();

	for(int i=0; i< totalMotors; i++){
		ard.sendServoAttach(i+2);
	}
}

//--------------------------------------------------------------
void ofApp::updateArduino(){
	ard.update();
}

//--------------------------------------------------------------
void ofApp::setupSystem(){
	//generate a randome refresh speed for each agent
	//in the system
	for (int i = 0; i < totalAgents; i++){
		float rt = ofRandom(REFRESH_TIME_MIN, REFRESH_TIME_MAX);
		refreshTime.push_back(rt);
	}

	//initiate the Ql objects and store theme in a vector
	Ql qlTmp;
	for (int i = 0; i < totalAgents; i++){
		qlTmp.setup(N_STATES, ACTIONS, EPSILON, ALPHA, 
			GAMMA, MAX_EPISODES, refreshTime[i], EPISODE_TIME, 
			initPosX, initPosY, cellSize, false);
		ql.push_back(qlTmp);
	}
}

//--------------------------------------------------------------
void ofApp::setup(){
	unsigned int tseed = ofGetUnixTime();
	ofSeedRandom(tseed);

	ofTrueTypeFont::setGlobalDpi(72);
	ofSetFrameRate(60);

	XML.loadFile("settings.xml");

	totalAgents = XML.getValue("APP:AGENTS:totalAgents", 20);
	N_STATES = XML.getValue("APP:AGENTS:N_STATES", 6);
	ACTIONS.push_back("left");
	ACTIONS.push_back("right");
	EPSILON = XML.getValue("APP:AGENTS:EPSILON", 0.9);
	ALPHA = XML.getValue("APP:AGENTS:ALPHA", 0.1);;
	GAMMA = XML.getValue("APP:AGENTS:GAMMA", 0.9);
	MAX_EPISODES = XML.getValue("APP:AGENTS:MAX_EPISODES", 20);
	REFRESH_TIME_MIN = XML.getValue("APP:AGENTS:REFRESH_TIME_MIN", 0.01);
	REFRESH_TIME_MAX = XML.getValue("APP:AGENTS:REFRESH_TIME_MAX", 0.5);
	EPISODE_TIME = XML.getValue("APP:AGENTS:EPISODE_TIME", 0);

	PORT = XML.getValue("APP:AGENTS:PORT", "ttyACM0");
	BAUD = XML.getValue("APP:AGENTS:BAUD", 57600);

	lineWidth = XML.getValue("APP:AGENTS:lineWidth", 1);
	direction = "right";

	w = XML.getValue("APP:AGENTS:w", 1200);
	h = XML.getValue("APP:AGENTS:h", 768);

	ofSetWindowShape(w, h);

	initPosX = ofGetWidth()/5;
	initPosY = ofGetHeight()/10;

	xSize = ofGetWidth()/3;
	ySize = 50;
	cellSize = xSize/N_STATES;

	lines.r = 255;
	lines.g = 255;
	lines.b = 255;

	runSystem = false;

	ofSetRectMode(OF_RECTMODE_CENTER);

	fontSize = XML.getValue("APP:AGENTS:fontSize", 10);
	font.load("Roboto-Regular.ttf", fontSize, true, true);

	ard.connect(PORT, BAUD);
	ofAddListener(ard.EInitialized, this, &ofApp::setupArduino);
	bSetupArduino = false;

	totalMotors = XML.getValue("APP:AGENTS:totalMotors", 5);
	useMotors = XML.getValue("APP:AGENTS:useMotors", 0);

	callibrateMotors = useMotors;

	// qTable.resize(N_STATES);
	// for(int i=0; i<N_STATES; i++){
	// 	qTable[i].resize(ACTIONS.size());
	// 	for(int j=0; j<ACTIONS.size(); j++){
	// 		qTable[i][j] = 0;
	// 		//{{0,0},{0,0},{0,0},{0,0},{0,0}}
	// 	}
	// }
}

//--------------------------------------------------------------
void ofApp::update(){
	if(useMotors){
		updateArduino();

		if(bSetupArduino && callibrateMotors){
			for (int i = 0; i < totalMotors; i++){
				ard.sendServo(i+2, 0, false);
			}
			runSystem = true;
			callibrateMotors = false;
			setupSystem();
		}
	}

	if(runSystem){
		for (int i = 0; i < totalAgents; i++){
			ql[i].run();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	ofSetLineWidth(lineWidth);
	ofNoFill();

	if(runSystem){
		ofSetColor(lines);

		//for each agent in the system
		for (int agent = 0; agent < totalAgents; agent++){
			
			qTable = ql[agent].qTable;

			ofNoFill();
			//make a grid
			for (int state = 0; state < N_STATES; state++){
				ofRect(initPosX+(cellSize*2*state),
					initPosY+(agent*cellSize), 
					cellSize*2, cellSize);
			}
			
			
			ofFill();
			ofRect(initPosX+(cellSize*2*ql[agent].s_), 
				initPosY+(agent*cellSize), cellSize, 
				cellSize/2);

			if(bSetupArduino && useMotors){
				for(int i=0; i<totalMotors; i++){
					if(agent == i){
						float angle = ql[agent].s_*10;
						ard.sendServo(i+2, angle, false);
					}
				}
			}

			for(int state=0; state< N_STATES; state++){
				for(int action=0; action< ACTIONS.size(); action++){
					string s = ofToString(roundf(qTable[state][action] * 1000) / 1000);
					if(action == 0){
						font.drawString(s, initPosX+(cellSize*2*state)-10, initPosY+(agent*cellSize));
					}else{
						font.drawString(s, initPosX+(cellSize*2*state)+10, initPosY+(agent*cellSize));
					}
					
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
