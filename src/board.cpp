#include <cstring>
#include <stdlib.h>
#include <time.h>

#include "../include/board.hpp"

Board::Board(int frame_rate){
    score = 0;
    line = 0;
    this->frame_rate=frame_rate;
    std::fill_n(&gameboard_status[0][0], sizeof(gameboard_status)/sizeof(**gameboard_status
                ), -1);
    // set gameboard_status to false 
    game_win = createNewwinWithBox(gw_height, gw_width, gw_start_y, gw_start_x);
    next_win = createNewwinWithBox(nw_height, nw_width, nw_start_y, nw_start_x);
    score_win = createNewwinWithBox(sw_height, sw_width, sw_start_y, sw_start_x);
    line_win = createNewwinWithBox(lw_height, lw_width, lw_start_y, lw_start_x);
    
    // create random tetris
    srand(time(NULL));
    int random_current = rand() % Tetris::num_of_type; 
    int random_next = rand() % Tetris::num_of_type; 
    current_tetris = new Tetris(0, 0, random_current, random_current);
    next_tetris = new Tetris(0, 0, random_next, random_next);
}

Board::~Board(){
    delete current_tetris;
    delete next_tetris; 
}

void Board::init(){
    // Enable keypad userinput
    keypad(game_win, TRUE);
    printInMiddle(next_win, 0, nw_width, "Next");
    printInMiddle(score_win, 0, sw_width, "Score");
    printInMiddle(line_win, 0, lw_width, "Line");
}

void Board::closeWindow(){
    destoryWin(game_win);
    destoryWin(next_win);
    destoryWin(score_win);
    destoryWin(line_win);
}


void Board::handleUserInput(){
    int c = wgetch(game_win);
    switch(c){
        case KEY_LEFT:
            deleteTetris(game_win, current_tetris);
            current_tetris->moveLeft();
            if(!tetrisCanMove()){
                current_tetris->moveRight();
            }
            drawTetris(game_win, current_tetris);
            break;
        case KEY_RIGHT:
            deleteTetris(game_win, current_tetris);
            current_tetris->moveRight();
            if(!tetrisCanMove()){
                current_tetris->moveLeft();
            }
            drawTetris(game_win, current_tetris);
            break;
        case ' ':
            deleteTetris(game_win, current_tetris);
            current_tetris->rotateClockWise();
            if(!tetrisCanMove()){
                current_tetris->rotateAntiClockWise();
            } 
            drawTetris(game_win, current_tetris);
            break;
        case KEY_DOWN:{
            deleteTetris(game_win, current_tetris);
            auto current_cor = current_tetris->getTopLeftCor();
            current_tetris->fall();
            if(!tetrisCanMove()){
                current_tetris->setTopLeftCor(current_cor.first, current_cor.second);
            }
            drawTetris(game_win, current_tetris);
            break;
        }
        case KEY_UP:{
            deleteTetris(game_win, current_tetris);
            auto current_cor = current_tetris->getTopLeftCor();
            current_tetris->setTopLeftCor(current_cor.first-1, current_cor.second);
            if(!tetrisCanMove()){
                current_tetris->setTopLeftCor(current_cor.first, current_cor.second);
            }
            drawTetris(game_win, current_tetris);
            break;
        }
        default:
            break;
    }
}

void Board::updateGameWindow(){
    this->current_tetris->setTopLeftCor(10, 10); 
    drawTetris(game_win, current_tetris);
}

void Board::updateScoreWindow(){
    wclear(score_win);
    box(score_win, 0, 0);
    printInMiddle(score_win, sw_height/2, sw_width, std::to_string(score));
    wrefresh(score_win);
}

void Board::updateNextWindow(){
    wclear(next_win);
    box(next_win, 0, 0);
    drawTetris(next_win, next_tetris);
    printInMiddle(next_win, 0, nw_width, "Next");
    wrefresh(next_win);
}

void Board::updateLineWindow(){
    wclear(line_win);
    box(line_win, 0, 0);
    printInMiddle(line_win, lw_height/2, lw_width, std::to_string(line));
    wrefresh(line_win);
}

bool Board::tetrisCanMove(){
    // check whether the current tetris can move around 
    // No collision with the border
    // This implementation is somehow ugly
    // Refactor late
    int row_num = current_tetris->getShape().size();
    auto top_left = current_tetris->getTopLeftCor();
    auto shape = current_tetris->getShape();
    auto checker = [](int x, int y)->bool {
        return x >= 0 && x <= gw_height-2 && y >= 0 && y <= gw_width-2;
    };
    for(int i = 0; i < row_num; ++i){
        for(int j = 0; j < row_num; ++j){
            if(shape[i][j]){
                // Check top and bottom border
                if(top_left.first+i<=0 || top_left.first+i+1 >= gw_height){
                    return false;
                }
                // Check left and right border
                if(top_left.second+2*j <= 0 || top_left.second+2*j+2 >= gw_width){
                    return false;
                }
                // Check bottom conflict with existing tetris 
                if(checker(top_left.first+i-1, top_left.second+2*j-1)){
                    if(gameboard_status[top_left.first+i-1][top_left.second+2*j-1] != -1){
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void Board::gameStart(){

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
                mvwaddch(win, top_left_cor.first+i, top_left_cor.second+2*j, ' ');
                mvwaddch(win, top_left_cor.first+i, top_left_cor.second+2*j+1, ' ');
            }
        }
    }
}

void Board::getFullLineIndex(){
    for(int i = 0; i < gw_height-2; ++i){
        bool full = true;
        for(int j = 0; j < gw_width-2; ++j){
            if(gameboard_status[i][j] == -1){
                full = false;
            } 
        }
        if(full){
            full_line_index.push_back(i);
        }
    }
}

void Board::dropCurrentTetris(){
    for(int i = 0; i < gw_width-2; ++i){
         
    }
}
