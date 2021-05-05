#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <chrono>

#include "../include/board.hpp"

Board::Board(int frame_rate){
    score = 0;
    line = 0;
    tetris_freeze = false;
    this->frame_rate=frame_rate;
    frame_delay = 1000000/frame_rate;
    for(int i = 0; i < gw_height-2; ++i){
        for(int j = 0; j < gw_width-2; ++j){
            gameboard_status[i][j] = -1;
        }
    }
    // set gameboard_status to false 
    game_win = createNewwinWithBox(gw_height, gw_width, gw_start_y, gw_start_x);
    next_win = createNewwinWithBox(nw_height, nw_width, nw_start_y, nw_start_x);
    score_win = createNewwinWithBox(sw_height, sw_width, sw_start_y, sw_start_x);
    line_win = createNewwinWithBox(lw_height, lw_width, lw_start_y, lw_start_x);
    
    // create random tetris
    srand(time(NULL));
    int random_current = rand() % Tetris::num_of_type; 
    int random_next = rand() % Tetris::num_of_type; 
    current_tetris = new Tetris(1, gw_width/2-4, random_current, random_current);
    next_tetris = new Tetris(nw_height/2-2, nw_width/2-4, random_next, random_next);
}

Board::~Board(){
    delete current_tetris;
    delete next_tetris; 
}

void Board::init(){
    // Enable keypad userinput
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    updateLineWindow();
    updateScoreWindow();
    updateNextWindow();
}

void Board::closeWindow(){
    destoryWin(game_win);
    destoryWin(next_win);
    destoryWin(score_win);
    destoryWin(line_win);
}


void Board::handleUserInput(){
    //std::chrono::time_point p = std::chrono::high_resolution_clock::now();
    //while(true){
    //    std::chrono::time_point n = std::chrono::high_resolution_clock::now();
    //    auto nanosecond = std::chrono::duration_cast<std::chrono::nanoseconds>(n-p);
    //    
    //}
    int c = getch();
    usleep(frame_delay);
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
        case KEY_DOWN:
            dropCurrentTetris();
            break;
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
    printInMiddle(score_win, 0, sw_width, "score");
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
    printInMiddle(line_win, 0, lw_width, "Line");
    printInMiddle(line_win, lw_height/2, lw_width, std::to_string(line));
    wrefresh(line_win);
}

void Board::gameStart(){
    init();
    int frame_count = 0;
    while(true){
        if(gameOver()){
            drawTetris(game_win, current_tetris);
            break;
        }
        drawTetris(game_win, current_tetris);
        handleUserInput();
        if(frame_count % frame_rate == 0){
            if(!tetris_freeze){
                deleteTetris(game_win, current_tetris);
                current_tetris->fall();
                if(!tetrisCanMove()){
                    freezeTetris(); 
                }else{
                    drawTetris(game_win, current_tetris);
                }
            }
            frame_count = 0;
        }
        if(tetris_freeze){
            getFullLineIndex();
            removeFullLineWithAnimation();
            updateLineWindow();
            updateNextWindow();
            updateScoreWindow();
            tetris_freeze = false;
        }
        frame_count += 1;
    }
}

bool Board::gameOver(){
    auto shape = current_tetris->getShape();
    int row_num = shape.size();
    std::vector<int> element_count;
    for(int i = 0; i < row_num; ++i){
        int tmp =  0;
        for(int j = 0; j < row_num; ++j){
            if(shape[j][i]){
                tmp += 1;
            }
        }
        element_count.push_back(tmp);
    }
    int current_x = current_tetris->getTopLeftCor().second;
    for(int i = 0; i < row_num; ++i){
        for(int j : element_count){
            if(j==0){
                continue;
            }
            if(gameboard_status[j][current_x+i-1]!=-1){
                return true;
            }
        }
    }
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
    wrefresh(win);
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
    wrefresh(win);
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

void Board::removeFullLineWithAnimation(){
    if(full_line_index.empty()){
        return;
    }
    
    for(auto& i : full_line_index){
        for(int j = i; j > 0; --j){
            for(int k = 0; k < gw_width-2; ++k){
                gameboard_status[j][k] = gameboard_status[j-1][k]; 
            }
        }
    }

    int size = full_line_index.size();
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < gw_width-2; ++j){
            gameboard_status[i][j] = -1;
        }
    }
    wattron(game_win, COLOR_PAIR(7));
    for(auto& i: full_line_index){
        for(int j = 0; j < gw_width-2; ++j){
            mvwaddch(game_win, i+1, j+1, ACS_CKBOARD);
        }
    }
    wattroff(game_win, COLOR_PAIR(7));
    wrefresh(game_win);
    usleep(frame_delay*10);
    redrawGameBoard(); 
    wrefresh(game_win);

    // add line
    line += full_line_index.size();
    // add score
    int previous = -2;
    int current_score = basic_score;
    for(auto i: full_line_index){
        if(i == previous+1){
            current_score *= 2;
        }else{
            current_score = basic_score;
        }
        previous = i;
        score += current_score;
    }
    full_line_index.clear();
}

void Board::redrawGameBoard(){
    for(int i = 0; i < gw_height-2; ++i){
        for(int j = 0; j < gw_width-2; ++j){
            // remove current board char
            mvwaddch(game_win, i+1, j+1, ' ');
            // redraw new one
            if(gameboard_status[i][j] != -1){
                wattron(game_win, COLOR_PAIR(gameboard_status[i][j]+1));
                mvwaddch(game_win, i+1, j+1, ACS_CKBOARD);
                wattroff(game_win, COLOR_PAIR(gameboard_status[i][j]+1));
            }
        }
    }
}

void Board::dropCurrentTetris(){
    deleteTetris(game_win, current_tetris);
    while(tetrisCanMove()){
        current_tetris->fall();
    }         
    freezeTetris();
}

void Board::freezeTetris(){
    auto top_left = current_tetris->getTopLeftCor();
    top_left.first -= 1;
    current_tetris->setTopLeftCor(top_left.first, top_left.second);
    drawTetris(game_win, current_tetris);
    tetris_freeze = true;
    auto shape = current_tetris->getShape();
    int row_num = shape.size();
    //Update gameboard status
    for(int i = 0; i < row_num; ++i){
        for(int j = 0; j < row_num; ++j){
            if(shape[i][j]){
                //printw("%d %d\n", top_left.first+i-1, top_left.second+2*j-1);
                //refresh();
                gameboard_status[top_left.first+i-1][top_left.second+2*j-1] = current_tetris->getColor();
                gameboard_status[top_left.first+i-1][top_left.second+2*j] = current_tetris->getColor();
            }
        }
    }
    // generate new next and replace current with next 
    delete current_tetris;
    current_tetris = next_tetris;
    int random_next = rand() % Tetris::num_of_type;
    next_tetris = new Tetris(nw_height/2-2, nw_width/2-4, random_next, random_next);
    current_tetris->setTopLeftCor(1, gw_width/2-4);
}
