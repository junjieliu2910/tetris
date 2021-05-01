#include "../include/tetris.hpp" 

Tetris::Tetris(int start_y, int start_x, int shape_index, int color_pair){
    this->top_left = std::make_pair(start_y, start_x);
    this->color_pair = color_pair;
    this->shape = Tetris::shape_list[shape_index];
}

Tetris::~Tetris(){

}


void Tetris::rotate(){
    // Assume square shape 
    int row_num = this->shape.size();
    for(int i = 0; i < row_num / 2; ++i){
        for(int j = i; j < row_num - i - 1; ++j){
            bool temp = this->shape[i][j];
            this->shape[i][j] = this->shape[j][row_num-i-1];
            this->shape[j][row_num-i-1] = this->shape[row_num-i-1][row_num-j-1];
            this->shape[row_num-i-1][row_num-j-1] = this->shape[row_num-j-1][i];
            this->shape[row_num-j-1][i] = temp; 
        }
    }
}

void Tetris::fall(){
    // Fall the tetris by 1 grid 
    this->top_left.first ++;
}

int Tetris::getColor() const{
    return this->color_pair;
}

const std::vector<std::vector<bool>>& Tetris::getShape() const{
    return this->shape;
}

const std::pair<int, int>& Tetris::getTopLeftCor() const{
    return this->top_left;
}

void Tetris::setTopLeftCor(std::pair<int, int> cor){
     
}
