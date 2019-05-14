#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <stdbool.h>
#include <stdio.h>

#define BLOCK_TILE_NUMBER 4
#define BLOCK_SHAPE_ROW 2
#define BLOCK_SHAPE_COL 3
#define MAP_ROW 10
#define MAP_COL 7

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
    BlockTile tile;
    Point point;
} Block;

void InitializeTetris();

void SpawnNextBlock();
void GenerateBlockBag();
BlockTile GetNextBlockTile();
void GetBlockShape(BlockTile blockTile, unsigned char shape[][BLOCK_SHAPE_COL]);

void MoveBlock(Direction direction);
bool CheckCollision(Block* block, Point point);
bool CheckOutOfMap(Block* block, Point point);

void DrawMap();
void GetMap(BlockTile map[][MAP_COL]);

#endif
