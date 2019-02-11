// Yun Ho Jung - github.com/Dolcevice
// CPP - Snake Game
// GNU General Public License v3.0
// Unattributed replication of the code is forbidden
// Inspired from original code by N. Vitanovic, at https://www.youtube.com/watch?v=E_-lMZDi7Uw

#include <iostream>
// For console related activities
#include <conio.h>
#include <windows.h>
// For user input sanitization
#include <string>
#include <sstream>

using namespace std;

// Class that defines the necessary components of the snake game
class Snake
{
	public:

	// Defining constant variables for the snake class
		const int width = 60; // Width of the game screen
		const int height = 9; // Height of the game screen

	// Defining variables for the snake class
		// The coordinates for the head of the snake
		int headX = width / 2;
		int headY = height / 2;
		int score = 0; // Player's score
		int nTail = 0; // Length of the tail
		int updateDelay = 33; // Time the program idles before creating another frame - 33 is 30 frames per second
		int fruitMax = 1; // How many fruits are in game at a time

		bool gameover = false; // Flag for the status of the game
		bool pause = false; // Flag if the game is paused
		bool isEasy;

	// Creating the array that holds the potential positions of tail elements 100x100
		int tailX[100] = { 0 }; // Initialize to 0 so we don't end up accessing random memory locations
		int tailY[100] = { 0 };

		int fruitX[5], fruitY[5]; // Coordinates of the fruit - up to 5 fruits at once

	// Creating the enumeration for the snake class
		enum eDirection { STOPPED = 0, LEFT, RIGHT, UP, DOWN}; // Enumerated directions of the snake
		eDirection dir = STOPPED; // Creating an enumeration object direction
		eDirection currentDir = STOPPED; // This second direction variable will be used to prevent a 180 suicide of the snake


	// Function that draws the horizontal wall
		void drawHorizontalWall()
		{
			// Draws 61 #s
			// This is more efficient
			cout << "#############################################################" << "\n";

			// This comment is a for loop for drawing the horizontal wall. Because this is inefficient then just printing a set amount of #s, I will leave it there.
			// This may be used when width is changed.
			/*
			for (int wallCount = 0; wallCount < width+1; wallCount++)
				cout << "#";
			cout << "\n";
			*/
		} // End function

	// Function that draws the snake, the fruit, and the vertical wall
		void drawSnake()
		{
			system("cls"); // Call the system to clear the screen
			drawHorizontalWall(); // Draw the horizontal wall

			// For each row:
			for (int row = 0; row < height; row++)
			{
				// For each Column
				for (int col = 0; col < width; col++)
				{
					// Draw the first wall of the row
					if (col == 0)
					{
						cout << "#";
					}
					// Draw the head of the snake
					else if (row == headY && col == headX)
					{
						cout << "0";
					}
					// Draw the first fruit
					else if (row == fruitY[0] && col == fruitX[0])
					{
						cout << "F";
					}
					// Draw the fruits, tails, or draw nothing
					else
					{
						// Flag for printing an empty cell
						bool doNotPrintAnything = true;
						for (int fruitCount = 1; fruitCount < fruitMax; fruitCount++)
						{
							if (row == fruitY[fruitCount] && col == fruitX[fruitCount])
							{
								cout << "F";
								doNotPrintAnything = false;
								continue;
							}
						}
						// If the coordinate matches the tail coordinate, print the tails
						for (int tailElementCounter = 0; tailElementCounter < nTail; tailElementCounter++)
						{
						    // For x coordinates
						    if (tailX[tailElementCounter] == col && tailY[tailElementCounter] == row)
                            {
                                cout << "o";
                                doNotPrintAnything = false;
                            }
						}
						// If the flag is true, print empty cell
						if (doNotPrintAnything)
						{
							cout << " ";
						}
					}// End if
					// Draw the last wall for the row
					if (col == width - 1)
					{
						cout << "#";
					}
				}// End loop
				// Move on to the next row
				cout << "\n";
			}// End loop
			// Draw the bottom wall
			drawHorizontalWall();
			// Show the score of the player and other useful information
			cout << "Score:" << score << "\n";
			// Shows the update delay and fps. FPS is calculated by 1000 / delay in milliseconds
			cout << "Current updateDelay: " << updateDelay << " (FPS: " << 1000.0f / float(updateDelay) << ")" << "\n";
			cout << "WASD to move, eat the fruit to grow the snake as long as possible!" << "\n";
			cout << "Press '-' speed up the game and '=' to slow down the game" << "\n";
			cout << "Press 'p' to pause" << "\n";
			cout << "Press 'x' to exit" << "\n";
		} // End Function


        // Function that takes the input from the keyboard and modifies the direction
		void controlSnake()
		{
			// Check for keyboard input
			if (_kbhit())
			{
				// Take action depending on the input
				switch (_getch())
				{
					// WASD controls the movement
					case 'a':
               		 	dir = LEFT;
                		break;
					case 'd':
						dir = RIGHT;
						break;
					case 'w':
						dir = UP;
						break;
					case 's':
						dir = DOWN;
						break;
					// Extra functions
					case 'x': // End the game
						gameover = true;
						break;
					case 'p': // Pause the game
						pause = true;
						break;
					case '=': // Increase the amount of time the game sleeps before creating another frame
						if (updateDelay < 200) // Max is 200
						{
							updateDelay += 20;
							cout << "Increasing updateDelay to " << updateDelay << "\n";
						}
						else
						{
							cout << "updateDelay is already at maximum." << "\n";
						}
						break;
					case '-': // Decrease the amount of time the game sleeps before creating another frame
						if (updateDelay > 20) // Minimum is 20
						{
							updateDelay -= 20;
							cout << "Decreasing updateDelay to " << updateDelay << "\n";
						}
						else
						{
							cout << "updateDelay is already at minimum." << "\n";
						}
						break;
					default:
						break;
				} // End Switch
			} // End if
		} // End Function


		// Function that deals with the logic of the snake game
		void moveSnake()
		{
		    // Store
		    int prevX = tailX[0];
            int prevY = tailY[0];
            int holdX;
            int holdY;
            // We store the current position of the first tail in to the variable previous.
            // Same for the vertical coordinates
			tailX[0] = headX; // Then, the current tail position becomes the current position of the head.
			tailY[0] = headY; // Same for the vertical coordinates
			// Then, same transfer of location is used for just the tails.
			for (int elementCount = 1; elementCount < nTail; elementCount++) // We start from 1 since we already took care of 0 above
			{
			    holdX = tailX[elementCount]; // Store the tail's position into temporary storage.
				holdY = tailY[elementCount]; // Same for the vertical coordinates
			    tailX[elementCount] = prevX; // The tail's position becomes the previous coordinates
				tailY[elementCount] = prevY; // Same for vertical coordinates
				prevX = holdX; // Previous position becomes the held position from the tail's position
				prevY = holdY; // Same for the vertical coordinates
			}
		} // End Function


		// Function that changes the coordinates of the snake based on the current direction of the snake
		void directionSnake()
		{
		    switch (dir)
		    {
		        case LEFT:
				// We do not want the player to be able to 180 and self-destruct - same for all other cases
					if (currentDir == RIGHT && nTail > 1)
					{
						dir = RIGHT;
						break;
					}
					break;
                case RIGHT:
				// We do not want the player to be able to 180 and self-destruct - same for all other cases
					if (currentDir == LEFT && nTail > 1)
					{
						dir = LEFT;
						break;
					}
					break;
                case UP:
				// We do not want the player to be able to 180 and self-destruct - same for all other cases
					if (currentDir == DOWN && nTail > 1)
					{
						dir = DOWN;
						break;
					}
					break;
                case DOWN:
				// We do not want the player to be able to 180 and self-destruct - same for all other cases
					if (currentDir == UP && nTail > 1)
					{
						dir = DOWN;
						break;
					}
					break;
                default:
                    break;
		    }
			switch (dir)
			{
				// Cases depend on the direction
				// Change the coordinates depending on the direction, then change current direction
                case LEFT:
                    headX--;
                    currentDir = LEFT;
                    break;

				case RIGHT:
                    headX++;
                    currentDir = RIGHT;
                    break;

				case UP:
                    headY--;
                    currentDir = UP;
                    break;

				case DOWN:
                    headY++;
                    currentDir = DOWN;
                    break;

                default:
                    break;
				// Otherwise, do nothing
			}
		} // End function

        // Function that checks if the snake hit the walls, and processes logic depending on the mode
		void checkBoundWrapSnake()
		{
			// If the game is on easymode, let the snake wrap around the wall
			if (isEasy)
			{
				if (headX >= width)
				{
					headX = 1; // Wrap to beginning
				}
				else if (headX < 1)
				{
					headX = width - 2; // Wrap to the end
				}
				if (headY >= height)
				{
					headY = 0; // Wrap to the beginning
				}
				else if (headY < 0)
				{
					headY = height - 2; // Wrap to the end
				}
			// If the game is on normal mode, gameover if collision with wall
			}
			else
			{
			    // End the game if hit the wall
			    if (headX >= width || headX < 1)
				{
					gameover = true;
				}
			    else if (headY >= height || headY < 0)
				{
					gameover = true;
				}

			} // End if
		}// End function


		// Function that checks if the snake ate the fruit and updates the length of the snake
		void eatFruitSnake()
		{
			// For all the fruits spawned
			for (int fruitCount = 0; fruitCount < fruitMax; fruitCount++)
			{
				// If the coordinates of the head match the coordinates of the fruit
				if (headX == fruitX[fruitCount] && headY == fruitY[fruitCount])
				{
					score += 10; // Plus 10 to the score!
					nTail++; // Increase the tail length
					generateFruitSnake(fruitCount); // Generate a replacement fruit
				}
			}
		}// End Function


		// Function that checks if snake hit its own tail
		void hitOwnTailSnake()
		{
			for (int tailCount = 0; tailCount < nTail; tailCount++)
			{
			    // If the tail is longer than 2 and it's positions overlap
				if (nTail > 2 && tailX[tailCount] == headX && tailY[tailCount] == headY)
				{
					gameover = true;
					cout << "You hit your own tail!" << "\n";
       			}
   			}
		}// End Function


		// Function that spawns a fruit of the desired label
		void generateFruitSnake(int fruitCount)
		{
			// Assign a random position for the fruit
			fruitX[fruitCount] = rand() % width;
			fruitY[fruitCount] = rand() % height;
			// Make sure they don't overlap with any other fruit
			for (int xCount = 0; xCount < fruitMax; xCount++)
			{
			    // If they overlap, try somewhere else
				if (xCount != fruitCount && fruitX[xCount] == fruitX[fruitCount])
				{
					fruitX[fruitCount] = rand() % width;
				}
			}
			for (int yCount = 0; yCount < fruitMax; yCount++)
			{
			    // Same for vertical coordinates
			    // I personally don't want the fruits to be on the same row or column
				if (yCount != fruitCount && fruitY[yCount] == fruitY[fruitCount])
				{
					fruitY[fruitCount] = rand() % height;
				}
            // This may become a problem when there are too many fruits
            // For now this is fine
			}
		} // End function


		// Function that generals ALL the fruits
		void generateFruitInALoop(int fruitMax)
		{   // Loop for the number of fruits
			for (int fruitCount = 0; fruitCount < fruitMax; fruitCount++)
			{
			    // Call generation method
				generateFruitSnake(fruitCount);
			}
		}// End Function


		// Function that allows the user to customize the game
		int customizeSnake()
		{
			// Variable that holds user's choice
			string userChoiceOfFruit;
			int convertedChoiceOfFruit;

			while (true)
			{
				cout << "Enter the number of fruits to be spawned, 1 to 5: " << "\n";
				getline(cin, userChoiceOfFruit);

				stringstream ssChoice(userChoiceOfFruit);

				// Check if cin failed
				if (!(ssChoice >> convertedChoiceOfFruit))
				{
					cin.clear();
					cout << "Please enter a valid number" << "\n";
					continue;
				}
				// Check if the input was between 1 to 5
				else if (convertedChoiceOfFruit > 5 || convertedChoiceOfFruit < 1)
				{
					cout << "Please enter a number between 1 and 5" << "\n";
					continue;
				}
				else
				{
					break;
				}
			} // End loop
			return convertedChoiceOfFruit; // This will return the number of fruit to be spawned - converts from the ascii code.
		} // End function


        // Function that allows the users to choose easy mode
		void easyMode()
		{
		    // Define a variable that holds user choice
			string userChoice;

			while (true)
			{
			    // Prompt user
				cout << "Turn easymode - no gameover for hitting the wall - on? (y/n)" << "\n";
				getline(cin, userChoice);

				if (userChoice == "y" || userChoice == "Y")
				{
					isEasy = true;
					break;
				}
				else if (userChoice == "n" || userChoice == "N")
				{
					isEasy = false;
					break;
				}
				else
				{   // Ask again if invalid input
					cin.clear();
					cout << "Invalid input" << "\n";
				}
			}
		}// End Function
};// End Class

// Class that deal with extra functions
class ExtraFunctions
{
	public:
        // Enumeration that contains better readability for status
		enum eStatus {APPROVED = 0, DISAPPROVED, UNRECOGNIZED};


        // Function that prints out the tutorial
		void tutorial()
		{
			cout << "Welcome to C++ Snake!" << "\n";
			cout << "Use WASD to move, P to pause, and X to exit the game" << "\n";
			cout << "Use '-' to speed up the game and '=' to slow down the game" << "\n";
			cout << "Eating the fruit makes the snake longer. If you hit your own tail, the game will end." << "\n";
			cout << "Created by Yun Ho Jung, github.com/Dolcevice" << "\n";
			cout << "Inspired from orignal code by N. Vitanovic, at https://www.youtube.com/watch?v=E_-lMZDi7Uw" << "\n";
			cout << "But I pretty much rewrote the most things" << "\n";
			cout << "************************************************************" << "\n";
		}// End Function


        // Function that can enable or disable cursor
		void showCursor(bool wantCursor)
		{
		    // Create a new handle object
			HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
            // Create a new console_cursor_info object
			CONSOLE_CURSOR_INFO cursorInfo;
			GetConsoleCursorInfo(out, &cursorInfo);
			cursorInfo.bVisible = wantCursor; // Set the cursor visibility depending on the bool that is passed in
			SetConsoleCursorInfo(out, &cursorInfo);
		}// End Function


        // Function that takes in the user choice for replay
		int replayPrompt()
		{
		    // Create variables that hold user input and status
			string userResponse;
			eStatus result;
            // Prompt user
			cout << "Would you like to play again? Enter 'y' to play again or 'n' to exit " << "\n";
			// Get user choice
			getline(cin, userResponse);
            // Branching check
			if (userResponse == "y" || userResponse == "Y")
			{
				result = APPROVED;
			}
			else if (userResponse == "n" || userResponse == "N")
			{

				result = DISAPPROVED;
			}
			else
			{
				cin.clear();
				result = UNRECOGNIZED;
			}
			return result;
		}// End Function


        // Function that replays the game depending on the result from replay prompt
		void replay()
		{
		    // Loop
			while(true)
			{
			    // Assign the return from replayPrompt to userChoice
				int userChoice = replayPrompt();
				// Compare to the enumerated values
				if (userChoice == UNRECOGNIZED)
				{
				    // Loop again
					cout << "Invalid input: " << "\n";
				}
				else if(userChoice == DISAPPROVED)
				{
				    // Exit the entire program
					exit(EXIT_SUCCESS);
				}
				else if(userChoice == APPROVED)
				{
				    // Restart the game
					break;
				}
			}
		}// End Function
};// End Class


// Main!
int main()
{
    // Loop
    while(true)
	{
	// Creating objects
        Snake snakeGame; // Create an object in the Snake class
		ExtraFunctions ext; // Create an object in the ExtraFunctions class

        ext.tutorial(); // Call tutorial from ExtraFunctions
		snakeGame.easyMode(); // Call easyMode from Snake
        snakeGame.fruitMax = snakeGame.customizeSnake(); //Set fruit max to return value from customizeSnake
       	snakeGame.generateFruitInALoop(snakeGame.fruitMax); // Generate Fruits based on fruitMax

        // Loop game until game over
        while(!snakeGame.gameover)
        {
			ext.showCursor(false); // Call showCursor from ExtraFunctions class
            snakeGame.drawSnake(); // Call drawSnake from Snake Class
            snakeGame.controlSnake(); // Call controlSnake from Snake Class
            // If pause, pause
            if (snakeGame.pause)
            {
                system("pause"); // Call pause from snake
                snakeGame.pause = false; // Order windows console to pause - assuming the OS is windows. Then again, we are already using system("cls") anyways so we are assuming that this is windows
            }
            // Logic Steps of the game
            // What these methods do are documented in their respective classes
            snakeGame.moveSnake(); // Call moveSnake from Snake
            snakeGame.directionSnake(); // Call directionSnake from Snake
            snakeGame.checkBoundWrapSnake(); // Call checkBoundWrapSnake from Snake
            snakeGame.hitOwnTailSnake(); // Call hitOwnTailSnake from Snake
            snakeGame.eatFruitSnake(); // CAll eatFruitSnake from Snake

            Sleep(snakeGame.updateDelay); // Sleep until creating another frame
        }

        ext.showCursor(true); // Show the cursor again
		ext.replay(); // Call replay from Extra Functions Class
		system("cls"); // Clear the screen
	}
    return 0;
} // End Main



