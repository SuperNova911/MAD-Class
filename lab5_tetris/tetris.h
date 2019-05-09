#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <stdbool.h>
#include <stdio.h>

typedef enum _Direction
{
    Up, Down, Left, Right
} Direction;

typedef enum _BlockTile
{
    Empty = 0, I, J, S, T
} BlockTile;

typedef struct Point_t
{
    int x;
    int y;
} Point;

typedef struct Block_t
{
    Direction direction;
    BlockTile shape[2][3];
    Point point;
} Block;

unsigned char I_Block_Shape[2][3] =
{
    { 0, 0, 0 },
    { 1, 1, 1 }
};

unsigned char J_Block_Shape[2][3] =
{
    { 1, 0, 0 },
    { 1, 1, 1 }
};

unsigned char S_Block_Shape[2][3] =
{
    { 1, 1, 0 },
    { 0, 1, 1 }
};

unsigned char T_Block_Shape[2][3] =
{
    { 0, 1, 0 },
    { 1, 1, 1 }
};


void Initialize();

void SpawnNextBlock();
void GenerateBlockBag();
BlockTile GetNextBlockTile();

void MoveBlock(Direction direction);
bool CheckCollision(Block* block, Point point);

#endif
