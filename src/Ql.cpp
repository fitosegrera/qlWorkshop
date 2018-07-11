#include "ofApp.h"

//--------------------------------------------------------------
Ql::Ql(){
}

//--------------------------------------------------------------
void Ql::setup(int NS, vector <string> A, float EPS, float ALP, 
	float G, int ME, float FT, float ET, int initpx, int initpy, 
	int cs, bool db){
	
	//ofSeedRandom(ofGetElapsedTimef());

	//define all variables
	N_STATES = NS;
	ACTIONS.push_back(A[0]);
	ACTIONS.push_back(A[1]);
	EPSILON = EPS;
	ALPHA = ALP;
	GAMMA = G;
	MAX_EPISODES = ME;
	REFRESH_TIME = FT;
	EPISODE_TIME = ET;
	
	s_ = 0;
	r = 0;

	isTerminal = false;
	is_terminated = false;
	doingEpisodes = true;
	done = false;

	debugging = db;

	step_counter = 0;
	episodeCounter = 0;
	updateState = 0;

	cellSize = cs;
	initPosX = initpx;
	initPosY = initpy;

    startTime = ofGetElapsedTimeMillis();  // get the start time
    endTime = REFRESH_TIME*1000; // in milliseconds

	build_q_table(N_STATES, ACTIONS);
	rl();

}

//--------------------------------------------------------------
void Ql::build_q_table(int n_states, vector <string> actions){

	qTable.clear();
	qTable.resize(N_STATES);
	
	for (int i = 0; i < n_states; i++){
		qTable[i].resize(actions.size());
		for (int j = 0; j < actions.size(); j++){
			qTable[i][j] = 0;
		}
	}
	
}

//--------------------------------------------------------------
string Ql::choose_action(int state, vector <vector <float>> q_table){

	vector <float> qlayer;
	string action_name = "";

	for(int i=0; i<q_table.size(); i++){
		if(i == state){
			qlayer = q_table[i];
			//cout<< "qt: " + ofToString(q_table[i])<< endl;
		}
	}

	float rnd = ofRandomuf();

	//act non-greedy or state-action have no value
	if(rnd > EPSILON || qlayer[0] == 0 && qlayer[1] == 0){
		int randomChoice = ofRandom(0, qlayer.size());
		action_name = ACTIONS[randomChoice];
	 }
	else{ //act greedy
        if (qlayer[0] >= qlayer[1]){
            action_name = "left";
        }else{
        	action_name = "right";
        }
	}

	return action_name;
}

//--------------------------------------------------------------
void Ql::get_env_feedback(int s, string a){

	s_ = 0;
	r = 0;

	if(a=="right"){
		if(s >= N_STATES - 2){
			if(debugging){
				cout<< "TERMIAL!"<< endl;
			}
			s_ = s + 1; //terminal
			r = 1;
			isTerminal = true;
		}else{
			s_ = s + 1;
            r = 0;
		}
	}
	else{
		r = 0;
		if(s == 0){
			s_ = s;
		}else{
			s_ = s - 1;
		}
	}
}

//--------------------------------------------------------------
void Ql::rl(){

	string A = choose_action(updateState, qTable);

	get_env_feedback(updateState, A);

	float maxVal = 0;
	float q_target = 0;

	float actionIndex = 0;

	if(A == "left"){
		actionIndex = 0;
	}else{
		actionIndex = 1;
	}
	
	float q_predict = qTable[updateState][actionIndex];

	if(qTable[s_][0] >= qTable[s_][1]){
		maxVal = qTable[s_][0];
	}else{
		maxVal = qTable[s_][1];
	}

	if(!isTerminal){ // if state is not terminal
		q_target = r + GAMMA * maxVal; // next state is not terminal
	}else{
		q_target = r; // next state is terminal
		isTerminal = false;
		is_terminated = true;
		doingEpisodes = true;
	}

	qTable[updateState][actionIndex] += ALPHA * (q_target - q_predict);
	updateState = s_;

	step_counter += 1;

	string t = "";
	for (int i = 0; i < N_STATES; i++){
		for (int j = 0; j < ACTIONS.size(); j++){
			t += ofToString(qTable[i][j]) + ",";
		}
		t+="\n";
	}

	if(debugging){
		cout<< "\n"<< endl;
		cout<< t<< endl;
		cout<< "A: " + A<< endl;
		cout<< "s_: " + ofToString(s_)<< endl;
		cout<< "r: " + ofToString(r)<< endl;
		cout<< "Q-TARGET: " + ofToString(q_target)<< endl;
		cout<< "Q-PREDICT: " + ofToString(q_predict)<< endl;
		cout<< "ACTION VAL: " + ofToString(qTable[updateState][actionIndex])<< endl;
		cout<< "EPISODE: " + ofToString(episodeCounter)<< endl;
		cout<< "---------------------"<< endl;
	}
}

//--------------------------------------------------------------
void Ql::run(){
	
	if(!done){
		if(doingEpisodes){
			doingEpisodes = false;
			is_terminated = false;
			step_counter = 0;
			updateState = 0;

			if(episodeCounter>=MAX_EPISODES){
				done = true;
			}

			episodeCounter++;
		}		

		if(!is_terminated){

			float timer = ofGetElapsedTimeMillis() - startTime;
		    if(timer >= endTime) {      
		        rl();
				startTime = ofGetElapsedTimeMillis();
		    }
		}
	}
}

//--------------------------------------------------------------