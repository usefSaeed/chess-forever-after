# chess-forever-after

Full implementation of a CMD cheess gamehaving all possible features save, load, undo, redo… etc

Description:
Our application is a multiplayer chess game application coded by C programming language and designed to be an online multiplayer game, where each player makes a move in the chess board alternatively till one of them wins by checkmating the other. The game can end in draw in some cases too.

Features provided:
• On each move, we give each player the opportunity to make some non-move options:
o Resign the game.
o Submit a draw request.
 If other player agrees, game ends in draw.
o Save the game.
o Submit a load request.
 If other player agrees, the last game saved in the application will be loaded.
o Submit an undo request.
 If there has been at least one move since the beginning of the game.
• If the other player agrees, the last move will be undo-ed.
• NB: one move = both players entered one valid input each.
o Submit a redo request.
 If there has been at least one undo since the last move,
• If the other player agrees, the last move will be redo-ed.
o End the game in draw when threefold repetition takes place.
o Submit a restart request.
 If the other player agrees, the game will be restarted (feature added after video and some sample runs)..
• We ask for the 2 players’ names at the beginning of the game so we can call them by it.
o We give then the option to go ahead as player 1 and player 2.
o We ask them to either:
 Declare who will be player 1.
 Let the game choose player 1 randomly.
4
• Game can be undo-ed to the beginning of the game even if it was loaded (‘even if it was loaded’  feature added after video and some sample runs).
• Game can be redo-ed to the last move executed.
• All special moves are applied such as:
o Castling
o En passant
o Promotion
• Game ends in draw in the following cases:
o 2 players agreed to end in draw.
o 1 player decided to end in draw when threefold repetition took place.
o Fivefold repetition.
o Dead position.
o Stalemate.
• Game ends by a player who won in the following cases:
o If this player checkmated the other player.
o If the other player resigned.
• After the game ends, players have the ability to play again if there’s a mutual agreement to that (with no need od restarting application).
o They are called by their names without asking in the second to last games (feature added after video and some sample runs).
____________________________________________________________
5
Overview of Design:
• It is preferrable to play the game with a full screen of the console application.
• The game starts by asking for the names of 2 players, and ask them to indicate who is player 1.
• Then, it shows the user manual on the screen followed by the initial board which has white pieces in the bottom and black on top.
• Right after one player enters a valid input, a new board appears on screen having all the moves since the beginning of the game.
• After each move, a message is printed indicating which player’s turn it is then.
• The lowercase letters belong to player 1 and the uppercase letters belong to player 2.
• After each move, the board reflects about the x-axis at its centre to give better visualization for each player on his turn (feature added after video and some sample runs).
• The block of ‘#’ is a black block and the block of ‘-’ and ‘|’ is a white block.
• When a white piece is captured, it shows up in the next move under the “White pieces Captured” label on the left of the board.
• When a black piece is captured, it shows up in the next move under the “Black pieces Captured” label on the right of the board.
• In each of the following conditions, an indicator message shows up:
o Promotion.
o Check.
o Checkmate.
o Stalemate.
o Threefold right to end the game in draw.
o Fivefold.
o A player won.
o Draw.
o Saving game.
o Dead Position.
6
• A message is printed indicating the non-move options for each player on every turn.
• All invalid inputs are covered and a message indicating that shows up.
o Ex:
 User wants to move a piece that is not his.
 User wants to move his piece in a wrong way.
 User wants to make a move that will result in his king. being checked.
 User enters any wrong input by mistake.
• After the game ends, the 2 players have the option to play again.
o If not program terminates in 5 seconds (feature added after video and some sample runs).
____________________________________________________________
7
Design/implementation assumptions made:
• Game will not be more than 300 moves.
o As the longest chess game ever played, with all rules applied was 277 moves.
• Player 2 cannot undo just his move as it will be player 1’s turn after that. He can only undo the last move for the both of them when it is his turn and when player 1 agrees and vice versa.
• Player 2 cannot play when the program says it is player 1’s turn and vice versa.
• Player can save game, end in draw by threefold repetition and resign without taking permission of the other player.
• If one player closes application without saving, we assume he took the permission of the other player in real life and they decided that they do not want to complete the game. So, their progress is not saved.
• Both players actually know about all the valid moves of chess as our application alerts the user when an invalid input is entered, but it does not provide him with the valid inputs to choose from them.
• When game is saved, the saved game before it is overwritten as there is only one file for saving and loading.
• User shouldn’t change anything in the file where we save the board upon request “SaveMESaveME”.
_________________________________________________________
8
Further Necessary Details:
Any player who approaches the game should know the following:
• All possible moves for all pieces and when these moves become invalid.
• Check: When current king is threatened by an enemy piece.
• Checkmate: When there is no valid more for current player that makes his king not checked.
• Stalemate: When current player has no valid moves at all including the fact that his king is not checked.
• Dead position: When no sequence of valid moves can get any player to win, so game ends with draw right away.
• Threefold Repetition: When the same board gets repeated 3 times with same conditions including player’s turn, current player has ability to declare a draw.
• Fivefold Repetition: When the same board gets repeated 5 times with same conditions including player’s turn, game ends with draw as it is not going anywhere and there is no progress being made.
_________________________________________________________
