#pragma once
#include <ncurses.h>
#include <vector>
#include <string>

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
        void closeWindow();
        void updateGameWindow();
        void updateScoreWindow();
        void updateNextWindow();
        void updateLineWindow();
        bool gameOver();
        

    private:
        // Game realted
        Tetris* current_tetris;
        Tetris* next_tetris; 

        int score, line; 
        bool gameboard_status[20][30] = {{false}};

        // Window and UI related
        WINDOW* game_win;
        WINDOW* next_win;
        WINDOW* score_win;
        WINDOW* line_win;
        
        const int gap_between_win = 2;
        // settings for game window
        const int gw_height=20, gw_width=30;
        const int gw_start_y=5, gw_start_x=5;

        // Setting for next window
        const int nw_height=6, nw_width=12;
        const int nw_start_y=gw_start_y;
        const int nw_start_x=gw_start_x + gw_width + gap_between_win;

        // settings for score window 
        const int sw_height=6, sw_width=12;
        const int sw_start_y = nw_start_y + nw_height;
        const int sw_start_x = nw_start_x; 

        // setting for line window
        const int lw_height = 6, lw_width = 12;
        const int lw_start_y = sw_start_y + sw_height;
        const int lw_start_x = nw_start_x;

        WINDOW* createNewwinWithBox(int height, int width, int start_y, int start_x);
        void destoryWin(WINDOW* win);
        void printInMiddle(WINDOW* win, int height, int width, std::string msg);
        void drawTetris(WINDOW* win, const Tetris* block);
        void deleteTetris(WINDOW* win, const Tetris* block);


};
