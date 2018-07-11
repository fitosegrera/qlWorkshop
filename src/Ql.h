#pragma once

#ifndef _QL
#define _QL

#include "ofMain.h"

class Ql{

	public:

		Ql();//constructor

		void build_q_table(int n_states, vector <string> actions);
		string choose_action(int state, vector <vector <float>> q_table);
		void get_env_feedback(int S, string A);
		void rl();
		void setup(int NS, vector <string> A, float EPS, float ALP, 
			float G, int ME, float FT, float ET, int initpx, 
			int initpy, int cs, bool db);
		void run();

		int N_STATES;
		vector <string> ACTIONS;
		float EPSILON;
		float ALPHA;
		float GAMMA;
		int MAX_EPISODES;
		float REFRESH_TIME;
		float EPISODE_TIME;

		// the length of the 1 dimensional world
        // available self.ACTIONS
        // greedy police / RANDOMNESS / original 0.9
        // learning rate
        // discount factor
        // maximum episodes
        // fresh time for one move

		int s_, r;

		bool isTerminal, is_terminated, doingEpisodes, done, debugging;
		int episodeCounter;

		int step_counter;
		int updateState;

		vector <vector <float>> qTable;

		int cellSize, initPosX, initPosY;

		//timer
		float startTime; // store when we start time timer
		float endTime; // when do want to stop the timer		
};
#endif