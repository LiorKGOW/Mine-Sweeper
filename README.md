# Mine-Sweeper

This program is a game called "Mine Sweeper"

It recieves from the Player a number representing a choice of size for the board of the game,
and each "Turn" it recieves from the player a row and a column representing a spot in the board that the player had chosen.

First of all, the program asks the Player to choose a Size for the board.
After that, it sets up the board, and plants Mines randomly (the number of mines in the board is according to the size of the Board -> the square root of the number of spots in the Board).
Each turn, the program asks the player to enter a row a column, and a letter (F/f/O/o). According to his choice it executes the following:

1.  If the player has hit a mine, the program reveals the entire board, and the game ends.
2.  if he hasn't, the program reveals the spot the player has chosen.
3.  If it is "empty" (it doeasn't have any mines around it),
    it reveals all of the empty spots that is near it, and the spots that contain numbers next to each empty spot that was opened. (New In Update 1.2)
4.  If it is near a mine (it contains a number representing the number of mines planted next to it), it reveals the number.
5.  In Update 1.2, the player now has an option to Flag a spot he suspects by pressing F/f after choosing row and column in each turn.
    If the player wants to open the spot normally, he needs to press O/o after choosing a spot.
    At any point in the game the player has the option of opening a Flagged spot simply by choosing it again and pressing O/o after (normally).

the purpose of the game is to reveal all of the spots that are NOT mines.
If the player has succeded to do so, it prints an according message.

Good Luck !!
