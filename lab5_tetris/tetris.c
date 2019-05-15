#include "tetris.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned char I_Block_Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
    { 0, 0, 0 },
    { 1, 1, 1 }
};
unsigned char J_Block_Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
    { 1, 0, 0 },
    { 1, 1, 1 }
};
unsigned char S_Block_Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
    { 1, 1, 0 },
    { 0, 1, 1 }
};
unsigned char T_Block_Shape[BLOCK_SHAPE_ROW][BLOCK_SHAPE_COL] =
{
    { 0, 1, 0 },
    { 1, 1, 1 }
};

unsigned int Score;
unsigned int Stage;
float Speed;

BlockTile BlockBag[BLOCK_TILE_NUMBER];
int BlockBagCount;

BlockTile Map[MAP_ROW][MAP_COL];
BlockTile RenderedMap[MAP_ROW][MAP_COL];
Point SpawnPoint = { 3, 0 };
Block CurrentBlock;

void InitializeTetris()
{
    GenerateBlockBag();
}

void SpawnNextBlock()
{
    Block nextBlock;
    BlockTile nextTile;

    nextTile = GetNextBlockTile();
    if (nextTile < I || nextTile > T)
    {
        printf("Unknown tile, nextTile: '%d'\n", nextTile);
        return;
    }

    nextBlock.direction = Up;
    nextBlock.tile = nextTile;
    
    if (CheckCollision(&nextBlock, SpawnPoint) == true)
    {
        // TODO: GameOver
        printf("GameOver\n");
        return;
    }

    nextBlock.point = SpawnPoint;

    CurrentBlock = nextBlock;
}

void GenerateBlockBag()
{
    BlockTile randomTile;
    bool existCheck[BLOCK_TILE_NUMBER];

    memset(existCheck, 0, sizeof(existCheck));
    memset(BlockBag, 0, sizeof(BlockBag));
    BlockBagCount = 0;

    srand((unsigned)time(NULL));
    while (true)
    {
        randomTile = rand() % BLOCK_TILE_NUMBER + 1;

        if (existCheck[randomTile - 1] == true)
        {
            continue;
        }

        BlockBag[BlockBagCount] = randomTile;
        BlockBagCount++;

        if (BlockBagCount == BLOCK_TILE_NUMBER)
        {
            break;
        }
    }
}

BlockTile GetNextBlockTile()
{
    BlockTile nextBlockTile;

    if (BlockBagCount <= 0)
    {
        GenerateBlockBag();
    }

    BlockBagCount--;
    nextBlockTile = BlockBag[BlockBagCount];
    BlockBag[BlockBagCount] = Empty;

    return nextBlockTile;
}

void GetBlockShape(BlockTile blockTile, unsigned char shape[][BLOCK_SHAPE_COL])
{
    int row, col;
    unsigned char (*targetShape)[BLOCK_SHAPE_COL];

    switch (blockTile)
    {
        case I:
            targetShape = I_Block_Shape;
            break;

        case J:
            targetShape = J_Block_Shape;
            break;

        case S:
            targetShape = S_Block_Shape;
            break;

        case T:
            targetShape = T_Block_Shape;
            break;

        default:
            printf("Unknown BlockTile, blockTile: '%d'\n", blockTile);
            targetShape = I_Block_Shape;
            break;
    }

    for (row = 0; row < BLOCK_SHAPE_ROW; row++)
    {
        for (col = 0; col < BLOCK_SHAPE_COL; col++)
        {
            shape[row][col] = targetShape[row][col];
        }
    }
}

void MoveBlock(Direction direction)
{
}

bool CheckCollision(Block* block, Point point)
{
    if (CheckOutOfMap(block, point) == true)
    {
        return true;
    }

    return false;
}

bool CheckOutOfMap(Block* block, Point point)
{
    int row, col;
    unsigned char (*blockShape)[BLOCK_SHAPE_COL] = NULL;

    GetBlockShape(block->tile, blockShape);

    switch (block->direction)
    {
        case Up:
            for (row = 0; row < BLOCK_SHAPE_COL; row++)
            {
                for (col = 0; col < BLOCK_SHAPE_ROW; col++)
                {
                    if (blockShape[row][col] == 0)
                    {
                        continue;
                    }

                    if (row + point.y >= MAP_ROW || col + point.x >= MAP_COL)
                    {
                        return true;
                    }
                }
            }
    }

    return false;
}

void DrawMap()
{
    int row, col;

    for (row = 0; row < MAP_ROW; row++)
    {
        for (col = 0; col < MAP_COL; col++)
        {
            RenderedMap[row][col] = Map[row][col];
        }
    }

    for (row = 0; row < BLOCK_SHAPE_ROW; row++)
    {
        for (col = 0; col < BLOCK_SHAPE_COL; col++)
        {
        }
    }
}

void GetMap(BlockTile map[][MAP_COL])
{

}
