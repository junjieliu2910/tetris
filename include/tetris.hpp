#pragma once
#include <vector>
#include <utility>

class Tetris{
    public:
        Tetris(int start_y, int start_x, int shape_index, int color_pair);
        ~Tetris();

        static const int num_of_type = 7;

        // rotate the tetris 
        void rotateClockWise();
        void rotateAntiClockWise();
        // Position related function
        void fall();
        void moveLeft();
        void moveRight();

        int getColor() const;
        const std::vector<std::vector<bool>>& getShape() const;
        const std::pair<int, int>& getTopLeftCor() const;
        void setTopLeftCor(int y, int x);
        
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
