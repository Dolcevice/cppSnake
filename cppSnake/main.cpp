// CS1337.010 - Yun Ho Jung
// Assignment 1 - Snake Game
// GNU General Public License v3.0

#include <iostream>
#include <conio.h> // for _kbhit and _getch in the input routine
#include <windows.h> // for the sleep function in main

using namespace std;

enum eStatus {APPROVED = 0, DISAPPROVED, UNRECOGNIZED};

int playAgain();

// Class that defines the necessary components of the snake game
class Snake
{
	public:

	// Defining constant variables for the snake class
		const int width = 20; // Width of the game screen
		const int height = 20; // Height of the game screen

	// Defining variables for the snake class
		int headX = width / 2;
		int headY = height / 2; // Head of the snake
		int score = 0; // Player's score
		int nTail = 0; // Length of the tail
		int sleepAmt = 33; // Time the program idles before creating another frame
		//33 is 30 frames per second
		int fruitMax = 1; // How many fruits are in game at a time
		bool gameover = false; // Flag for the status of the game
		bool pause = false; // Flag if the game is paused


	// Creating the array that holds the potential positions of tail elements
		int tailX[100], tailY[100]; // Two hundred by hundred arrays for the tail location
		int fruitX[5], fruitY[5]; // Coordinates of the fruit - up to 5 fruits at once
	// Creating the enumeration for the snake class
		enum eDirection { STOPPED = 0, LEFT, RIGHT, UP, DOWN}; // Numerized directions of the snake
		eDirection dir = STOPPED; // Creating an enumeration object dir

	// Function that draws the horizontal wall
		void drawHorizontalWall()
		{
			// Draws 22 #s
			cout << "#####################" << endl;

			// This comment is a for loop for drawing the horizontal wall. Because this is inefficient then just printing a set amount of #s, I will leave it there.
			// This may be used when width is changed.
			/*
			for (int i = 0; i < width+2; i++)
				cout << "#";
			cout << endl;
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
					}
					// Draw the last wall for the row
					if (col == width - 1)
					{
						cout << "#";
					}

				}
				// Move on to the next row
				cout << endl;

			}
			// Draw the bottom wall
			drawHorizontalWall();
			// Show the score of the player
			cout << "Score:" << score << endl;
			cout << "Current SleepAmt: " << sleepAmt << endl;
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
					case '+': // Increase the amount of time the game sleeps before creating another frame
						if (Snake::sleepAmt < 200) // Max is 200
						{
							Snake::sleepAmt += 20;
							cout << "Increasing sleepAmt to " << sleepAmt << endl;
						}
						else
						{
							cout << "sleepAmt is already at maximum." << endl;
						}
						break;
					case '-': // Decrease the amount of time the game sleeps before creating another frame
						if (sleepAmt > 20) // Minimum is 20
						{
							sleepAmt -= 20;
							cout << "Decreasing sleepAmt to " << sleepAmt << endl;
						}
						else
						{
							cout << "sleepAmt is already at minimum." << endl;
						}
						break;
					default:
						break;
				} // End switch
			} // End if
		} // End Function

		// This variable will be used to prevent a 180 suicide of the snake
		eDirection currentDir = STOPPED;
		// Previous positions of the snake's head

		// Temporary storage for position


		// Function that deals with the logic of the snake game
		void moveSnake()
		{
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
			    holdX = tailX[elementCount]; // Store thetail's position into temporary storage.
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
			}// End switch
		} // End function

		void checkBoundWrapSnake()
		{
			// Because game does not end when the snake hits the wall, we allow the snake to wrap around the wall
			if (headX >= width) headX = 1; else if (headX < 1) headX = width - 2;
			if (headY >= height) headY = 0; else if (headY < 0) headY = height - 2;
		}
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
					generateFruitSnake(fruitCount);
				}
			}
		}
		// Function that checks if snake hit its own tail
		void hitOwnTailSnake()
		{
			for (int tailCount = 0; tailCount < nTail; tailCount++)
			{
				if (nTail > 2 && tailX[tailCount] == headX && tailY[tailCount] == headY)
				{
					gameover = true;
					cout << "You hit your own tail!" << endl;
       			}
   			}
		}
		// Function that spawns a fruit of the desired label
		void generateFruitSnake(int fruitCount)
		{
			// Assign a random position for the fruit
			fruitX[fruitCount] = rand() % width;
			fruitY[fruitCount] = rand() % height;
			// Make sure they don't overlap with any other fruit
			for (int xCount = 0; xCount < fruitMax; xCount++)
			{
				if (xCount != fruitCount && fruitX[xCount] == fruitX[fruitCount])
				{
					fruitX[fruitCount] = rand() % width;
				}
			}
			for (int yCount = 0; yCount < fruitMax; yCount++)
			{
				if (yCount != fruitCount && fruitY[yCount] == fruitY[fruitCount])
				{
					fruitY[fruitCount] = rand() % height;
				}

			}
		} // End function
		// Function that allows the user to customize the game
		int customizeSnake()
		{
			// Variable that holds user's choice
			char userChoiceOfFruit;
			int convertedChoiceOfFruit;
			while (true)
			{
				cout << "Enter the number of fruits to be spawned, 1 to 5: " << endl;
				cin.get(userChoiceOfFruit);
				// Check if cin failed
				if (!cin.good())
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					cout << "Please enter a valid number" << endl;
					continue;
				}
				// Check if the input was between 1 to 5
				else if (int(userChoiceOfFruit) > 53 || int(userChoiceOfFruit) < 49) // 53 == 5, 49 == 1 in ASCII
				{
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					cout << "Please enter a number between 1 and 5" << endl;
					continue;
				}
				else
				{
				    cin.clear();
					cin.ignore(INT_MAX, '\n');
					break;
				}
			} // End loop
            convertedChoiceOfFruit = int(userChoiceOfFruit) - 48;
			return convertedChoiceOfFruit; // This will return the number of fruit to be spawned - converts from the ascii code.
		} // End function

		// Function that shows the tutorial for the game
		void tutorialSnake()
		{
			cout << "Welcome to C++ Snake!" << endl;
			cout << "Use WASD to move, P to pause, and X to exit the game" << endl;
			cout << "Use '-' to speed up the game and '+' to slow down the game" << endl;
			cout << "Eating the fruit makes the snake longer. If you hit your own tail, the game will end." << endl;
			cout << "Created by Yun Ho Jung, github.com/Dolcevice" << endl;
			cout << "Inspired from orignal code by N. Vitanovic, at https://www.youtube.com/watch?v=E_-lMZDi7Uw" << endl;
			cout << "But I pretty much rewrote the most things" << endl;
			cout << "************************************************************" << endl;
        }

};

void showCursor(bool wantCursor)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = wantCursor; // Set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

// Main!
int main()
{
    while(true)
	{
	// Creating object
        Snake snakeGame; // This should have activated class constructor

        snakeGame.tutorialSnake();
        snakeGame.fruitMax = snakeGame.customizeSnake();
        showCursor(false);

        for (int fruitCount = 0; fruitCount < snakeGame.fruitMax; fruitCount++)
        {
            snakeGame.generateFruitSnake(fruitCount);
        }
        // Update game until game over

        while(!snakeGame.gameover)
        {
            snakeGame.drawSnake(); // Draw
            snakeGame.controlSnake(); // Input
            // If pause, pause
            if (snakeGame.pause)
            {
                system("pause");
                snakeGame.pause = false; // Order windows console to pause - assuming the OS is windows. Then again, we are already using system("cls") anyways so we are assuming that this is windows
            }
            // Logic Steps of the game
            // What these methods do are documented in their respective classes
            snakeGame.moveSnake();
            snakeGame.directionSnake();
            snakeGame.checkBoundWrapSnake();
            snakeGame.hitOwnTailSnake();
            snakeGame.eatFruitSnake();

            Sleep(snakeGame.sleepAmt); // Wait until creating another frame
        }

        showCursor(true);

		while(true)
		{
			int userChoice = playAgain();
			if (userChoice == 2)
			{
				cout << "Invalid input: " << endl;
			}
			else if(userChoice == 1)
			{
				exit(EXIT_SUCCESS);
			}
			else if(userChoice == 0)
			{
				break;
			}
		}

	}

    return 0;
} // End Function

int playAgain()
{
	char userResponse;
	eStatus result;


	cout << "Would you like to play again? Enter 'y' to play again or 'n' to exit " << endl;
	cin.get(userResponse);

	if (userResponse == 'y')
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		result = APPROVED;
	}
	else if (userResponse == 'n')
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		result = DISAPPROVED;
	}
	else
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		result = UNRECOGNIZED;
	}

	return result;
}

