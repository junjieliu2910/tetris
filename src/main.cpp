#include <iostream> 
#include <unistd.h>

#include "../include/tetris.hpp"
#include "../include/board.hpp"


void gameLoop();

int main(int argc, char* argv[]){
    initscr();
    cbreak();
    setlocale(LC_ALL, "");
    curs_set(0);

    if(!has_colors()){
       printf("Your terminal donot support color");
       endwin();
       return 1;
    }
    start_color(); 
    use_default_colors();
    init_pair(1, 1, 0);
    init_pair(2, 2, 0);
    init_pair(3, 3, 0);
    init_pair(4, 4, 0);
    init_pair(5, 5, 0);
    init_pair(6, 6, 0);
    init_pair(7, 7, 0);

    int frame_rate = 50;
    int delay = 1000000 / frame_rate;
    Board game_board = Board();
    game_board.init();
    game_board.updateGameWindow();

    while(1){
        game_board.handleUserInput();
        game_board.updateLineWindow();
        game_board.updateScoreWindow();
        game_board.updateNextWindow();
        usleep(delay);
    }

    game_board.closeWindow();
    endwin();
    return 0;
}

void gameLoop(){

}
