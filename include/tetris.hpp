#pragma once
#include <vector>
#include <utility>

class Tetris{
    public:
        Tetris(int start_y, int start_x, int shape_index, int color_pair);
        ~Tetris();
        // rotate the tetris close wise
        void rotate();
        // auto fall the tetris by 1 grid
        void fall();
        int getColor();
        const std::pair<int, int>& getTopLeftCor();
        void setTopLeftCor(std::pair<int, int> cor);
        
    private:    
        static inline const std::vector<std::vector<std::vector<bool>>> shape_list{
            {   // the 'o' block
                {0,0,0,0},
                {0,1,1,0},
                {0,1,1,0},
                {0,0,0,0}
            },
            {   // the long block
                {0,0,0,0},
                {1,1,1,1},
                {0,0,0,0},
                {0,0,0,0}
            },
            {   // the l block
                {0,1,1,0},
                {0,1,0,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {   // the j block
                {0,1,1,0},
                {0,0,1,0},
                {0,0,1,0},
                {0,0,0,0}
            },
            {   // the zag block
                {0,0,1,0},
                {0,1,1,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {   // the zig block
                {0,1,0,0},
                {0,1,1,0},
                {0,0,1,0},
                {0,0,0,0}
            },
            {   // the t block
                {0,0,0,0,0},
                {0,0,1,0,0},
                {0,1,1,1,0},
                {0,0,0,0,0},
                {0,0,0,0,0},
            } 
        };
        std::vector<std::vector<bool>> shape;
        std::pair<int, int> top_left;
        int color_pair;
};
