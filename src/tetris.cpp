#include "../include/tetris.hpp" 

Tetris::Tetris(int start_y, int start_x, int shape_index, int color_pair){
    top_left = std::make_pair(start_y, start_x);
    this->color_pair = color_pair;
    shape = Tetris::shape_list[shape_index];
}

Tetris::~Tetris(){

}

void Tetris::rotateClockWise(){
    // Assume square shape 
    int row_num = shape.size(); 
    for(int i = 0; i < row_num / 2; ++i){
        for(int j = i; j < row_num - i - 1; ++j){
            bool temp = shape[i][j];
            shape[i][j] = shape[row_num-j-1][i];
            shape[row_num-j-1][i] = shape[row_num-i-1][row_num-j-1];
            shape[row_num-i-1][row_num-j-1] = shape[j][row_num-i-1];
            shape[j][row_num-i-1] = temp;
        }
    }
}

void Tetris::rotateAntiClockWise(){
    // Assume square shape 
    int row_num = shape.size();
    for(int i = 0; i < row_num / 2; ++i){
        for(int j = i; j < row_num - i - 1; ++j){
            bool temp = shape[i][j];
            shape[i][j] = shape[j][row_num-i-1];
            shape[j][row_num-i-1] = shape[row_num-i-1][row_num-j-1];
            shape[row_num-i-1][row_num-j-1] = shape[row_num-j-1][i];
            shape[row_num-j-1][i] = temp; 
        }
    }
}

void Tetris::fall(){
    // Fall the tetris by 1 grid 
    top_left.first ++;
}

void Tetris::moveLeft(){
    // NOTE, 2 ACS_BLOCK for 1 rect shape
    top_left.second -= 2;
}

void Tetris::moveRight(){
    top_left.second += 2;
}

int Tetris::getColor() const{
    return color_pair;
}

const std::vector<std::vector<bool>>& Tetris::getShape() const{
    return shape;
}

const std::pair<int, int>& Tetris::getTopLeftCor() const{
    return top_left;
}

void Tetris::setTopLeftCor(int y, int x){
    top_left.first = y;
    top_left.second = x;
}
