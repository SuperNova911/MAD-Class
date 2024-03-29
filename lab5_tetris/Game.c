#include <getopt.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "Control.h"
#include "DeviceManager.h"
#include "DotAnimator.h"
#include "Point.h"
#include "Tetris.h"
#include "TickTimer.h"

#define GAME_UPDATE_SPEED 100
#define INPUT_UPDATE_SPEED 1

#define BLINK_DELAY 500

#define WALLKICK_ENABLE true
#define GRAVITY_ENABLE true

// General
void Initialize();
void SetOptions(int argc, char* argv[]);

// Tetris
void UpdateGame();

// Input
void UpdateInputAsync();
void* UpdateSwitchInput(void* inputManager);

// Device
void DrawMap(unsigned char map[][MAP_COL]);
void DrawNextBlock();

// Animation
void DrawLevelUp(unsigned int level);
void DrawPause();
void DrawGameOver();
void DrawFinalScore(unsigned int finalScore, unsigned int finalLevel);

// Global Variables
// Tetris
TetrisGame Tetris;
TickTimer GameUpdateTimer;
unsigned int CurrentLevel;
unsigned int FinalScore;

// Input
InputManager Input;
TickTimer InputUpdateTimer;
pthread_t InputUpdateThread;

// Device
unsigned char DotMatrix[DOT_MATRIX_ROW][DOT_MATRIX_COL];

// Animation
TickTimer BlinkTimer;

int main(int argc, char* argv[])
{
	OpenDevice();

	Initialize();
	SetOptions(argc, argv);

	RunTetris(&Tetris);
	RunTimer(&GameUpdateTimer);
	RunTimer(&InputUpdateTimer);
	RunTimer(&BlinkTimer);

	UpdateInputAsync();
	UpdateGame();

	DrawGameOver();
	DrawFinalScore(FinalScore, CurrentLevel);

	CloseDevice();

	return 0;
}

void Initialize()
{
	// Tetris
	InitializeTetris(&Tetris);
	InitializeTickTimer(&GameUpdateTimer, GAME_UPDATE_SPEED);
	CurrentLevel = 0;
	FinalScore = 0;

	// Input
	InitializeInputManager(&Input);
	InitializeTickTimer(&InputUpdateTimer, INPUT_UPDATE_SPEED);
	InputUpdateThread = pthread_create(&InputUpdateThread, NULL, UpdateSwitchInput, NULL);

	// Animation
	InitializeTickTimer(&BlinkTimer, BLINK_DELAY);
}

void SetOptions(int argc, char* argv[])
{
	static struct option gameOption[] =
	{
		{ "no_wallkick", 0, 0, 0 },
		{ "no_gravity", 0, 0, 0 },
		{ 0, 0, 0, 0 },
	};
	int option;
	int optionIndex;
	bool wallKickOption = WALLKICK_ENABLE;
	bool gravityOption = GRAVITY_ENABLE;

	while ((option = getopt_long(argc, argv, "", gameOption, &optionIndex)) != -1)
	{
		switch (option)
		{
		case 0:
			if (strcmp(gameOption[optionIndex].name, "no_wallkick") == 0)
			{
				wallKickOption = false;
			}
			else if (strcmp(gameOption[optionIndex].name, "no_gravity") == 0)
			{
				gravityOption = false;
			}
            break;
		default:
			printf("SetOptions: Invaild command-line argument\n");
			break;
		}
	}

	Tetris.GameMap.AllowWallKick = wallKickOption;
	Tetris.GameCore.EnableGravity = gravityOption;
}

void UpdateGame()
{
	InputInfo inputInfo;

	while (true)
	{
		if (IsTimerReady(&GameUpdateTimer) == false)
		{
			continue;
		}

		inputInfo = Dequeue(&Input.InputQueue);
		ReadUserInput(&Tetris, &inputInfo);

		UpdateTetris(&Tetris);
		RenderTetrisMap(&Tetris, DotMatrix);

		if (CurrentLevel < Tetris.GameInfo.Level)
		{
			CurrentLevel = Tetris.GameInfo.Level;
			DrawLevelUp(CurrentLevel);
		}

		if (Tetris.GameInfo.IsRunning == false)
		{
			DrawPause();
			continue;
		}

		DrawMap(DotMatrix);
		DrawNextBlock();

		if (IsTetrisGameOver(&Tetris) == true)
		{
			CurrentLevel = Tetris.GameInfo.Level;
			FinalScore = Tetris.GameInfo.Score;
			break;
		}
	}
}

void UpdateInputAsync()
{
	int result = 0;
	pthread_join(InputUpdateThread, (void**)& result);
}

void* UpdateSwitchInput(void* unused)
{
    unsigned char switchStatus[PUSH_SWITCH_NUMBER];
    memset(switchStatus, 0, sizeof(switchStatus));

    while (true)
    {
        if (IsTimerReady(&InputUpdateTimer) == false)
        {
            continue;
        }

        GetSwitchStatus(switchStatus);
        HandleInput(&Input, switchStatus);
    }
}

void DrawMap(unsigned char map[][MAP_COL])
{
	SetDotMatrix(map);
}

void DrawNextBlock()
{
	unsigned char renderedNextBlock = 0;

	RenderNextBlock(&Tetris.GameMap, &renderedNextBlock);
	SetLED(renderedNextBlock);
}

void DrawLevelUp(unsigned int level)
{
	int row, col;
	const static int ALPHABET_X = 0;
	const static int ALPHABET_Y = 0;
	const static int NUMBER_X = 4;
	const static int NUMBER_Y = 0;

	for (row = 0; row <= 5; row++)
	{
		memset(DotMatrix[row], 0, sizeof(unsigned char) * DOT_MATRIX_COL);
	}

	for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
	{
		for (col = 0; col < DOT_SMALL_FONT_COL; col++)
		{
			DotMatrix[row + ALPHABET_Y][col + ALPHABET_X] = DOT_SMALL_ALPHABET_L_FONT[row][col];
			DotMatrix[row + NUMBER_Y][col + NUMBER_X] = DOT_SMALL_NUMBER_FONT[level][row][col];
		}
	}

	PauseTetris(&Tetris);
	PauseTimer(&GameUpdateTimer);
	PauseTimer(&InputUpdateTimer);

	DrawMap(DotMatrix);
	usleep(1500000);

	ResumeTimer(&GameUpdateTimer);
	ResumeTimer(&InputUpdateTimer);
	ResumeTetris(&Tetris);
}

void DrawPause()
{
	static bool blinker = false;

	memset(DotMatrix, 0, DOT_MATRIX_SIZE);

	if (IsTimerReady(&BlinkTimer) == true)
	{
		blinker = !blinker;
	}

	if (blinker == false)
	{
		memcpy(DotMatrix, DOT_PAUSE_FONT, DOT_MATRIX_SIZE);
	}

	DrawMap(DotMatrix);
}

void DrawGameOver()
{
	int row;
	unsigned char fullCol[DOT_MATRIX_COL] = { 1, 1, 1 ,1 ,1, 1 ,1 };

    usleep(500000);
    for (row = DOT_MATRIX_ROW - 1; row >= 0; row--)
    {
        memcpy(DotMatrix[row], fullCol, sizeof(unsigned char) * DOT_MATRIX_COL);
        DrawMap(DotMatrix);
        usleep(150000);
    }

    for (row = 0; row < DOT_MATRIX_ROW; row++)
    {
        memset(DotMatrix[row], 0, sizeof(unsigned char) * DOT_MATRIX_COL);
        DrawMap(DotMatrix);
        usleep(150000);
    }
}

int MakeNumberMatrix(bool* matrix[DOT_SMALL_FONT_ROW], int number)
{
	int digitCount;
	int colLength;
	int row, col;
	int index, pivot;

	digitCount = (number == 0) ? 1 : (int)log10(number) + 1;
	colLength = digitCount * (DOT_SMALL_FONT_COL + 1);

	for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
	{
		matrix[row] = (bool*)malloc(sizeof(bool) * colLength);
		memset(matrix[row], 0, sizeof(bool) * colLength);
	}

	for (index = digitCount - 1, pivot = 0; index >= 0; index--, pivot += DOT_SMALL_FONT_COL + 1)
	{
		for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
		{
			for (col = 0; col < DOT_SMALL_FONT_COL; col++)
			{
				matrix[row][pivot + col] = DOT_SMALL_NUMBER_FONT[number / (int)pow(10, index)][row][col];
			}
		}
		number -= number / (int)pow(10, index) * (int)pow(10, index);
	}

	return colLength;
}

void DrawFinalScore(unsigned int finalScore, unsigned int finalLevel)
{
	const static int ITERATION = 3;
	int iterator;
	int pivot;
	int row, col;
	int marginRow = 2;
	bool *scoreNumberMatrix[DOT_SMALL_FONT_ROW];
	int scoreNumberColLength;

	memset(DotMatrix, 0, DOT_MATRIX_SIZE);
	scoreNumberColLength = MakeNumberMatrix(scoreNumberMatrix, finalScore);

    SetLED((unsigned char)finalLevel);
	
	for (iterator = 0; iterator < ITERATION; iterator++)
	{
		for (pivot = -DOT_MATRIX_COL; pivot <= DOT_SCORE_FONT_COL; pivot++)
		{
			for (row = 0; row < DOT_SCORE_FONT_ROW; row++)
			{
				for (col = 0; col < DOT_MATRIX_COL; col++)
				{
					if (pivot + col < 0 || pivot + col >= DOT_SCORE_FONT_COL)
					{
						DotMatrix[row + marginRow][col] = 0;
					}
					else
					{
						DotMatrix[row + marginRow][col] = DOT_SCORE_FONT[row][col + pivot];
					}
				}
			}
			DrawMap(DotMatrix);
			usleep(100000);
		}

		for (pivot = -DOT_MATRIX_COL; pivot <= scoreNumberColLength; pivot++)
		{
			for (row = 0; row < DOT_SMALL_FONT_ROW; row++)
			{
				for (col = 0; col < DOT_MATRIX_COL; col++)
				{
					if (pivot + col < 0 || pivot + col >= scoreNumberColLength)
					{
						DotMatrix[row + marginRow][col] = 0;
					}
					else
					{
						DotMatrix[row + marginRow][col] = scoreNumberMatrix[row][col + pivot];
					}
				}
			}
			DrawMap(DotMatrix);
			usleep(200000);
		}
		usleep(1000000);
	}
}
