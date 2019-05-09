#include "tetris.h"

unsigned int Score;
unsigned int Stage;
float Speed;

BlockTile BlockBag[4];
int BlockBagCount;

BlockTile Map[10][7];
Point SpawnPoint = { 3, 0 };
Block 

void Initialize()
{
    GenerateBlockBag();
}


void SpawnNextBlock()
{
    BlockTile nextBlockTile;
    Block nextBlock;

    nextBlock = GetNextBlockTile();

    nextBlock.direction = Direction.Up;
    switch (nextBlockTile)
    {
        case I:
            nextBlock.shape = I_Block_Shape;
            break;

        case J:
            nextBlock.shape = J_Block_Shape;
            break;

        case S:
            nextBlock.shape = S_Block_Shape;
            break;

        case T:
            nextBlock.shape = T_Block_Shape;
            break;

        default:
            printf("Unknown block tile, value: '%d'\n", nextBlockTile);
            nextBlock.shape = S_Block_Shape;
            break;
    }

    
    if (CheckCollision(&nextBlock, SpawnPoint) == true)
    {
        // TODO: GameOver
        printf("GameOver\n");
        return;
    }

    nextBlock.point = SpawnPoint;    
