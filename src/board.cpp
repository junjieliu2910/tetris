#include <cstring>
#include <stdlib.h>
#include <time.h>

#include "../include/board.hpp"

Board::Board(): gameboard_status{{0}}{
    this->score = 0;
    this->line = 0;
    // set gameboard_status to false 
    this->game_win = createNewwinWithBox(this->gw_height, this->gw_width, this->gw_start_y, this->gw_start_x);
    this->next_win = createNewwinWithBox(this->nw_height, this->nw_width, this->nw_start_y, this->nw_start_x);
    this->score_win = createNewwinWithBox(this->sw_height, this->sw_width, this->sw_start_y, this->sw_start_x);
    this->line_win = createNewwinWithBox(this->lw_height, this->lw_width, this->lw_start_y, this->lw_start_x);
    
    // create random tetris
    srand(time(NULL));
    int random_current = rand() % Tetris::num_of_type; 
    int random_next = rand() % Tetris::num_of_type; 
    this->current_tetris = new Tetris(0, 0, random_current, random_current);
    this->next_tetris = new Tetris(0, 0, random_next, random_next);
}

Board::~Board(){
    delete this->current_tetris;
    delete this->next_tetris; 
}

void Board::init(){
    this->printInMiddle(this->next_win, 0, this->nw_width, "Next");
    this->printInMiddle(this->score_win, 0, this->sw_width, "Score");
    this->printInMiddle(this->line_win, 0, this->lw_width, "Line");
}

void Board::closeWindow(){
    this->destoryWin(this->game_win);
    this->destoryWin(this->next_win);
    this->destoryWin(this->score_win);
    this->destoryWin(this->line_win);
}

void Board::updateGameWindow(){
    
}

void Board::updateScoreWindow(){

}

void Board::updateNextWindow(){
    wclear(this->next_win);
    box(this->next_win, 0, 0);
    this->drawTetris(this->next_win, this->next_tetris);
    this->printInMiddle(this->next_win, 0, this->nw_width, "Next");
    wrefresh(this->next_win);
}

void Board::updateLineWindow(){

}

bool Board::gameOver(){
    return false;
}


WINDOW* Board::createNewwinWithBox(int height, int width, int start_y, int start_x){
    WINDOW* win = newwin(height, width, start_y, start_x);
    refresh();
    box(win, 0, 0);
    wrefresh(win);
    return win;
}

void Board::destoryWin(WINDOW* win){
    wborder(win , ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

void Board::printInMiddle(WINDOW* win, int height, int width, std::string msg){
    int msg_length = msg.length();
    mvwprintw(win, height, (width-msg_length)/2, msg.c_str());
    wrefresh(win);
}

void Board::drawTetris(WINDOW* win, const Tetris* block){
    auto shape = block->getShape();
    auto top_left_cor = block->getTopLeftCor();
    int color_pair = block->getColor();
    int row_num = shape.size();
    wattron(win, COLOR_PAIR(color_pair+1));
    for(int i = 0; i < row_num; ++i){
        for(int j = 0; j < row_num; ++j){
            if(shape[i][j]){
                mvwaddch(win, top_left_cor.first+i, top_left_cor.second+2*j,  ACS_CKBOARD);
                mvwaddch(win, top_left_cor.first+i, top_left_cor.second+2*j+1,  ACS_CKBOARD);
            }
        }
    }
    wattroff(win, COLOR_PAIR(color_pair+1));
}

void Board::deleteTetris(WINDOW* win, const Tetris* block){
    auto shape = block->getShape();
    auto top_left_cor = block->getTopLeftCor();
    int row_num = shape.size();
    for(int i = 0; i < row_num; ++i){
        for(int j = 0; j < row_num; ++j){
            if(shape[i][j]){
                mvwaddch(win, top_left_cor.first+i, top_left_cor.second+2*j,  ACS_CKBOARD);
                mvwaddch(win, top_left_cor.first+i, top_left_cor.second+2*j+1,  ACS_CKBOARD);
            }
        }
    }
}
