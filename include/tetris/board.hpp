#pragma once
#include <ncurses.h>
#include <vector>
#include <string>
#include <cstring>

#include "tetris.hpp"

class Board{
    /** Game board, contains foour windows including
     * game window, next window, score window, and 
     * line window. 
     */
    public:
        Board();
        ~Board();
        void init();
        void updateGameWindow();
        void updateScoreWindow();
        void updateNextWindow();
        void updatelineWindow();
         
        bool gameOver();
        

    private:
        // Game realted
        Tetris current_tetris;
        Tetris next_tetris; 

        int score, line; 

        // Window and UI related
        WINDOW* game_win;
        WINDOW* next_win;
        WINDOW* score_win;
        WINDOW* line_win;
        
        int gap_between_win = 2;
        // settings for game window
        int gw_height=20, gw_width=30;
        int gw_start_y=5, gw_start_x=5;

        // Setting for next window
        int nw_height=6, nw_width=12;
        int nw_start_y=gw_start_y;
        int nw_start_x=gw_start_x + gw_width + gap_between_win;

        // settings for score window 
        int sw_height=6, sw_width=12;
        int sw_start_y=nw_start_y + nw_height + gap_between_win;
        int sw_start_x = nw_start_x; 

        // setting for line window
        int lw_height = 6, lw_width = 12;
        int lw_start_y = sw_start_y + sw_height + gap_between_win; 
        int lw_start_x = nw_start_x;

        WINDOW* createNewwinWithBox(int height, int width, int start_y, int start_x);
        void destory_win(WINDOW* win);
        void printInMiddle(WINDOW* win, int height, int width, std::string msg);
        void drawTetris(WINDOW* win, const Tetris& block, int start_y, int start_x);
        void deleteTetris(WINDOW* win, const Tetris& block, int start_y, int start_x);


};
