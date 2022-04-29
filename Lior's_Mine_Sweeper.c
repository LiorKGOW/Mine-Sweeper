
/**********************************************************************
   ID - 208547992
   Name - Lior Keren

   The program is a game called Mine Sweeper.
   It recieves from the Player a number representing a choice of size for the board of the game,
   and each "Turn" it recieves from the player a row and a column representing a spot in the board that the player had chosen.

   First of all, the program asks the Player to choose a Size for the board.
   After that, it sets up the board, and plants Mines randomly (the number of mines in the board is according to the size of the Board -> the square root of the number of spots in the Board).
   Each turn, the program asks the player to enter a row a column, and a letter (F/f/O/o). According to his choice it executes the following:
   1. If the player has hit a mine, the program reveals the entire board, and the game ends.
   2. if he hasn't, the program reveals the spot the player has chosen.
   3. If it is "empty" (it doeasn't have any mines around it),
   it reveals all of the empty spots that is near it, and the spots that contain numbers next to each empty spot that was opened. (New In Update 1.2)
   4. If it is near a mine (it contains a number representing the number of mines planted next to it), it reveals the number.
   5. In Update 1.2, the player now has an option to Flag a spot he suspects by pressing F/f after choosing row and column in each turn.
   If the player wants to open the spot normally, he needs to press O/o after choosing a spot.
   At any point in the game the player has the option of opening a Flagged spot simply by choosing it again and pressing O/o after (normally).

   the purpose of the game is to reveal all of the spots that are NOT mines.
   If the player has succeded to do so, it prints an according message.

   Good Luck !!

 *********************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZE 22   // Max Size of the Board
#define EXIT - (SIZE * SIZE)

typedef struct RowCol {
	int row;
	int col;
}RowCol;

// Explanation about the Functions used at the head of each one (after main).

RowCol StartGame(char[][SIZE], char[][SIZE]);
void RandomBoard(char[][SIZE], RowCol);
int Turn(char[][SIZE], char[][SIZE], RowCol);
void PrintBoard(char[][SIZE], RowCol);
void ClearNear(char[][SIZE], char[][SIZE], RowCol, int, int, int[]);

/********************************************************************************************/
void main() {

	char Board[SIZE][SIZE];   // the real board of the game. Contains the board that we are playing with. It is hidden from the player.
	char Player[SIZE][SIZE];   // The current board that the player has already been found. it is constantly being updated according to the Player's choices (each turn).
	int counter = 0;   // counts how many spots has already been revealed to the Player. Indicator when to stop the game.
	RowCol Size;   //  The chosen Size of Board (indicator: if Size.row is 0 -> don't play. Choice was 0!).
	int NumofBombs;   // The number of the bombs in the Board.

	Size = StartGame(Board, Player);   // Setting up the game, and getting the Size of the board.

	if (Size.row != 0) {   // Only if the player has chosen NOT to Exit.

		NumofBombs = (int)sqrt((double)Size.row * Size.col);   // The number of the bombs in the Board is the square root of the number of spots in the Board.

		printf("Good luck! have fun!! \n\n");

		printf("\n\n");
		PrintBoard(Player, Size);

		while (counter >= 0 && counter < Size.row * Size.col - NumofBombs) {   // while the player haven't revaeled all the required spots, continue playing another Turn.
			counter += Turn(Board, Player, Size);   // adds up the number of spots thats were revealed to the player

			// WIN or LOSE Check:

			if (counter == Size.row * Size.col - NumofBombs) {   // Win (the player has spoted all of the Non-Mine spots)
				PrintBoard(Board, Size);
				printf("Congratulation! You've won :) \n");
			}
			else {
				if (counter < 0) {   // Lose
					PrintBoard(Board, Size);
					printf("Game Over! You Lost! Better Luck next time \n");
				}
				else
					PrintBoard(Player, Size);   // Show the current board state, and continue playing
			}
		}
		printf("Thank you for playing ! :)\n");
	}
	else
		printf("Don't you want to play?? :( \nWell... See you next time! \n");
}

/********************************************************************************************
* StartGame sets up the game.It gets the Boardand the Player Board it assumes they are empty, and can be overwritten.
* Starts the interaction with the player with an opening message.
* If the player has chosen to Exit(0), the function returns 0.
* If he chose 1 - 4, it arranges the gameand sets up the boards for the game(according to the chosen size by the player).
* Setting Player to "Hidden" (by 'X')
* Then, Generates a random board in order to make the game a little more interesting;) (with the help of RandomBoard).

* There is an exception for row = 1 & col = 1. in this case, the function sets Player[0][0] as '*' and returns Size({ 1,1 }).
* This causes all of the board spots to be mines(the number of spots in the board is 1 * 1 which is 1, and the number of mines is the square root of(1 * 1) which is 1),
* which means that the player has "revealed" all of the non - mine spots, and by doing that he actually won.

* Startgame returns the size of the board that was chosen by the player in the start of the function in Size
* (Size.row = number of rows, Size.col = number of columns), Or 0 if the player has chosen to Exit.
*/
RowCol StartGame(char Board[][SIZE], char Player[][SIZE]) {

	RowCol Size = { 0,0 };   // the initial value is NOT used, except for option 4, a custom sized Board. 
	int Choice = 5;
	while (!(Choice >= 0 && Choice <= 4)) {   // while Choice is NOT valid
		printf("Welcome to Lior's Minesweeper 1.2! \n\n");
		printf("Please choose one of the following options and enter it's number: \n\n");
		printf("1 - For a size of 8X8 \n\n");
		printf("2 - For a size of 12X12 (NEW !!)\n\n");
		printf("3 - For a size of 15X15 (NEW !!)\n\n");
		printf("4 - For a custom size (NEW !!)\n\n");
		printf("0 - Exit \n\n");
		scanf("%i", &Choice);

		if (!(Choice >= 0 && Choice <= 4))   // Error message
			printf("You chose incorrectly. Please try again.\n\n");
	}
	// 0 <= Choice <= 4

	switch (Choice) {
	case 0:
		Size.row = 0;
		Size.col = 0;
		return Size;
		break;
	case 1:
		Size.row = 8;
		Size.col = 8;
		break;
	case 2:
		Size.row = 12;
		Size.col = 12;
		break;
	case 3:
		Size.row = 15;
		Size.col = 15;
		break;

	case 4:   // Custom Size:
		while (!(Size.row >= 1 && Size.row <= 22 && Size.col >= 1 && Size.col <= 22))   // while Size.row and Size.col are NOT valid, ask the user 1 more time.
		{
			printf("Please enter two numbers, the first is for the number of rows, and the second is for the number of columns of the Board. \n");
			printf("Please enter 2 numbers between 1 and 22 (including 1 and 22). \n");
			scanf("%i", &Size.row);
			scanf("%i", &Size.col);

			// Error message:
			if (!(Size.row >= 1 && Size.row <= 22 && Size.col >= 1 && Size.col <= 22))
				printf("Invalid choice! Please enter a valid choice!! \n");
		}
		// 1 <= Size.row <= 22 && 1 <= Size.col <= 22
		break;

	default:   // Empty, no use because 0 <= Choice <= 4
		break;
	}

	if (Size.row == 1 && Size.col == 1) {   // Make an exception for row = 1 & column = 1.
		// This causes all of the board spots to be mines (the square root of (1*1) is 1),
		// Which means that the player has "revealed" all of the non-mine spots, and by doing that he actually won.

		Player[0][0] = '*';
		return Size;
	}

	// Set up Player as "Hidden":

	for (int i = 0; i < Size.row; i++) {
		for (int j = 0; j < Size.col; j++) {
			Player[i][j] = 'X';
		}
	}

	// Generate a random Board:

	RandomBoard(Board, Size);

	return Size;
}

/*************************************************************************************************************************
* RandomBoard gets the Board, and the size of the board
* The function sets up the game :
* plants mines randomlyand adjusting the indicators of the bombs.
* The number of bombs in the Board is the square root of the number of spots in the board that was chosen by the player at the start of the game.

* RandomBoard doesn't return anything, just changes the values inside Board.
* It adjusts the Board according the rules in a random way.
*/
void RandomBoard(char Board[][SIZE], RowCol Size) {

	int NumofBombs;   // the number of bombs in the board

	// First of all, setting up Board as 0's :

	for (int i = 0; i < Size.row; i++) {
		for (int j = 0; j < Size.col; j++) {
			Board[i][j] = '0';
		}
	}

	//  Then, plants random bombs inside the board, and adjusting the counters around them:
	srand(time(NULL));   // In order to use Random function effectively.

	NumofBombs = (int)sqrt((double)Size.row * Size.col);   // The number of the bombs in the Board is the square root of the number of spots in the Board.

	for (int i = 1; i <= NumofBombs; i++) {     // plant NumofBombs random mines
		int r1 = rand() % Size.row;   // gets a random number between 0 and Size.row -1
		int r2 = rand() % Size.col;   // gets a random number between 0 and Size.col -1
		if (Board[r1][r2] == '*')     // If the spot is occupied, skip it.
			i--;
		else {   // Else, plant a bomb and adjust the spots next to it accordingly.
			Board[r1][r2] = '*';

			// Adding 1 to the "counters" of the mine indicators in the spots around the bomb that was just planted (Board[r1][r2] is a mine).

			for (int l = r1 - 1; l <= r1 + 1; l++) {
				for (int k = r2 - 1; k <= r2 + 1; k++) {
					if (0 <= l && 0 <= k && l < Size.row && k < Size.col && Board[l][k] != '*')   // if it exists (inside the Board)
						Board[l][k]++;    // Add 1 to the Mine indicaors.
				}
			}
		}
	}
}

/*************************************************************************************************************************
* Turn gets the Board, the Player Board, and the Size of the Boards.
* Turn also gets a rowand column from the player(the function assumes that he enters a number).
* In case that the player has already entered that spot before, it requests another pair, until a valid pair has been entered.
* Valid choices are : -1 : for the rowand a positive number for column for the Cheating option
* 0 - 7 : for both rowand column(which means the spot is inside the board).
* Turn checks what does the spot that the player has chosen contains, and adjusting Player board accordingky for an option that ISN'T a bomb (1-8 / ' ' for '0')
* and adjusts Counter according to the number of spots that were "revealed" to the player.
* In case of a bomb '*', it sets counter as the constant EXIT, in order to Exit the game.
* In Update 1.2, the player has the option to Flag a spot in the Board.
* After the player have entered a rowand a column, he needs to press o / O in order to reveal the chosen spot,
* or f / F in order to flag a spot in the Board.The flagged spot is indicated by 'F' and it can be revealed at any time of the game.

*Turn returns the number of spots that were revealed by the player in that specific turn(Counter).
* A negative number will be returned only if the player has "hit" a bomb.
*/
int Turn(char Board[][SIZE], char Player[][SIZE], RowCol Size) {

	int counter[1] = { 0 };   // Counts the number of spots that has already been revealed to the player, if equals to -1, the player has lost.
							 // it is an array of ints, because we want to send it to ClearNear which is a Recursion function.
							 // we send it to ClearNear by reference, and that helps her count how many spots were revealed.
					   // If it equals to the number of spot WITHOUT mines, the player has won (SIZE * SIZE - SIZE).
	int row = 0, col = 0;   // The chosen spot of the player
	char ch;                // The chosen action of the player towards that spot. ( F / f / O / o )

	printf("Please enter your move, row and column, and after O / F : \n O - to open a spot \n F - to flag a spot \n");
	scanf("%i", &row);
	scanf("%i", &col);
	scanf(" %c", &ch);

	while (!((ch == 'f' || ch == 'F' || ch == 'o' || ch == 'O') && 0 <= row && row < Size.row && 0 <= col && col < Size.col && (Player[row][col] == 'X' || Player[row][col] == 'F'))) {  // continue asking for new pairs until the choices are valid.

		bool check = false;   // is used every time in every iteration of the loop.
							  // indicates if a condition had already happened.

		if (row == -1 && col >= 0)
			break;
		if (0 <= row && row < Size.row && 0 <= col && col < Size.col) {   // if the player has already chosen this spot
			if (Player[row][col] != 'X' && Player[row][col] != 'F') {
				printf("You have already chosen this spot! Please enter a different spot! \n");
				check = true;
			}
		}
		if (!check && !(0 <= row && row < Size.row && 0 <= col && col < Size.col)) {
			printf("There is no such place on the board, please enter a valid choice! \n");
			check = true;
		}

		// the spot is in the Board & The player hasn't chosen this spot.
		if (!check && (ch != 'f' || ch != 'F' || ch == 'o' || ch == 'O')) {

			printf("\nThere is no such option. Please enter a valid choice! \n After choosing a spot, insert O / F ! \n Well.. you can use o / f too if you insist :) \n");
		}

		PrintBoard(Player, Size);

		printf("Please enter your move, row and column, and after O / F : \n");
		scanf("%i", &row);
		scanf("%i", &col);
		scanf(" %c", &ch);
	}

	// Legal row & column:

	//   Cheating Option ;) If row is -1, The game will reveal the column number (that was entered by the player) of spots WITHOUT mines, by order.
	//   This option works only for a positive number of column.
	if (row == -1)
	{
		for (int i = 0; i <= Size.row && col > 0; i++)
			for (int j = 0; j <= Size.col && col > 0; j++) {
				if ((Player[i][j] == 'X' || Player[i][j] == 'F') && Board[i][j] != '*') {
					Player[i][j] = Board[i][j];
					col--;
					counter[0]++;
				}
			}
		if (counter[0] > 0)
			printf("\n CHEATER ! ! ! \n\n");
	}
	else   // Legal input and not -1:
		switch (ch) {
		case 'f':
		case 'F':
			Player[row][col] = 'F';
			break;
		case 'o':
		case 'O':

			if ('1' <= Board[row][col] && Board[row][col] <= '8') {   // If the spot contains a mine indicator,
				Player[row][col] = Board[row][col];                   // Reveal the spot to the player
				counter[0]++;                                         // And counts the spot as revealed.
			}
			else {  // Board[row][col] == '*' / '0'
				if (Board[row][col] == '*') {             // If the player has chosen a spot with a bomb
					counter[0] = EXIT;                       // End the game
				}
				else {   // Board[row][col] == '0'        // If the player has chosen a spot without bombs around it ('0'), put in that spot in Player ' ',
															  // and clear all the spots next to it, until you reach a number:
					ClearNear(Board, Player, Size, row, col, counter);   // we call ClearNear and give her counter by reference!

						// (counter returned)
				}
			}
			break;
		default:   // empty. there are no other choices, so we won't get to this point.
			break;
		}

	return counter[0];
}

/*************************************************************************************************************************
* PrintBoard gets a Board, and the Size of it.
* The function prints it on the screen so the player can see it.
* it could get Board(which means the game is over).
* it could get Player(which is usually the case).

* The function doesn't return anything, just prints the board it was given (according to Size).
*/
void PrintBoard(char Board[][SIZE], RowCol Size) {

	// Print the Board that was given if the Size of the row is 1:

	if (Size.row == 1)
		if (Size.col == 1) {
			printf("        0  \n");
			printf("   ________\n");
			printf("    0 | %c |\n", Board[0][0]);
			printf("   ________\n");
		}
		else {   // row == 1 & col >1

			// First row:
			for (int i = 0; i < Size.col; i++) {
				if (i == 0)
					printf("        %i   ", i);
				
				else   // i > 0
					if (i <= 8)
						printf("%i   ", i);
					else   // i > 8 -> fix 1 space when printing (i is a two digit number)
						printf("%i  ", i);
			}
			printf("\n");

			// Second row:
			printf("   ____");
			for (int i = 0; i < Size.col; i++) {
				printf("____");
			}
			printf("\n");

			// Third row:
			for (int i = 0; i <= Size.col; i++) {
				if (i == 0)
					printf("    %i |", i);
				
				else
					if (Board[0][i-1] != '0')
						printf(" %c |", Board[0][i-1]);
					else
						printf(" %c |", ' ');
			}
			printf("\n");

			// Fourth row:
			printf("   ____");
			for (int i = 0; i < Size.col; i++) {
				printf("____");
			}

			printf("\n");
		}

	// Print the Board that was given if the Size of the row is more then 1:

	else {

		// These variables help the loops identify and print the "Pre Board" logistics:
		bool PreZero = true;   // In charge of the First line of numbers at the top of the board.
		bool PreOne = true;    // In charge of the second line of underlines '_' .
		bool PreJ = true;      // In charge of the start of each row until the first '|'.

		for (int i = 0; i < Size.row; i++) {

			if (i == 1 && PreOne)              // Only for one iteration of the outer loop (Once for PreOne).
				printf("   ____");

			for (int j = 0; j < Size.col; j++) {

				if (i == 0 && PreZero) {       // Only for one iteration of the outer loop (once for PreZero).

					if (j == 0)                // For the beginnig of the row.
						printf("        %i   ", j);
					else {                      // j > 0, For the rest of the row.
						if (j <= 8)
							printf("%i   ", j);
						else   // j > 8 -> fix 1 space when printing (j is a two digit number)
							printf("%i  ", j);
					}
				}

				else {
					if (i == 1 && PreOne)          // Only for one iteration of the outer loop (once for PreOne).
						printf("____");
					else {
						if (!PreZero && !PreOne) {     // For the rest of the board:
							if (j == 0 && PreJ) { // Only for the start of each row, PreJ
								if (i <= 9) {
									printf("    %i |", i);
									PreJ = false;          // No longer Pre Row logistics 
									j--;                   // Nullify J to print the rest of the row of the board without disturbance.
								}
								else {   // i > 9 -> fix 1 space when printing (i is a two digit number)
									printf("   %i |", i);
									PreJ = false;          // No longer Pre Row logistics 
									j--;                   // Nullify J to print the rest of the row of the board without disturbance.
								}
							}
							else {         // For the rest of the row:
								if (Board[i][j] != '0')
									printf(" %c |", Board[i][j]);
								else
									printf(" %c |", ' ');
							}
						}
					}
				}
			}
			if (PreOne && !PreZero) {   // Only after the second row (because !PreZero==true after the first row): adjusting PreOne to false;
				PreOne = false;
				i -= 2;   // nullify i to print the rest of the board without disturbance (except the first and the second rows for logistics).
			}
			if (PreZero)   // only after the first row: adjusting PreZero to false.
				PreZero = false;

			printf("\n");
			PreJ = true;   // sets PreJ back to true for another row (for the logistics for Pre Row).
		}

		// Printing a closure for the board: (according to SIZE)

		printf("   ____");
		for (int i = 0; i < Size.col; i++)
			printf("____");
		printf("\n");
	}
}

/*************************************************************************************************************************
* ClearNear gets the Board, and Playerand the Size of the Boards.
* It also gets the position that the Player has chosen in this turn(row + col), and a pointer to counter "array" by reference.
* The function assumes that the location is in the board and the content of Board[row][col] is ' '.

* The function "Reveals" to the Player all of the connected "free space".
* ClearNear also counts how many spots were reveled by it.

* ClearNear doesn't return anything by value, It just changes the values inside Player,
* and returns by reference counter(which contains how many spots were revealed)
*/
void ClearNear(char Board[][SIZE], char Player[][SIZE], RowCol Size, int row, int col, int counter[]) {

	// base case 1: out of the bounds of the Board
	if (!(row >= 0 && row < Size.row && col >= 0 && col < Size.col))
		return;

	// else, row && col are within the bounds of the Board
	// base case 2: if Board[row][col] isn't ' ', stop opening nearby spots.
	if (Board[row][col] != '0') {
		if (Player[row][col] == 'X' || Player[row][col] == 'F') {  // if the spot hasn't been revealed yet

			Player[row][col] = Board[row][col];   // reveal it

			counter[0]++;               // add one to counter
		}
		return;
	}

	// else Board[row][col] == '0'
	if (Player[row][col] == 'X' || Player[row][col] == 'F') {   // if this spot has not been revealed yet

		Player[row][col] = ' ';      // reveal it

		counter[0]++;                // add one to counter

		// and call the ClearNear for each spot around the current one:
		for (int i = row - 1; i <= row + 1; i++)
			for (int j = col - 1; j <= col + 1; j++)
				if (!(i == row && j == col))
					ClearNear(Board, Player, Size, i, j, counter);
	}
}

/*************************************************************************************************************************/