
	//Program 1: Wumpus, version 2 
	//CS 211, UIC, Fall 2022
	//System: Replit
	//Author: Sheqere Lilo

#include <stdio.h>
#include <stdbool.h>	// for the bool type in C
#include <stdlib.h>		// for srand
#include <ctype.h>		// for toupper()

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

// Used to more conveniently pass all game information between functions.
struct GameInfo {
	int moveNumber;  
	int playerRoom;  
	int wumpusRoom;  
	int pitRoom1;    
	int pitRoom2;     
	int batsRoom1; 
	int batsRoom2;   
	int arrowRoom;   
}; 


//   functions checkForHazards() and inRoomWithBats()
void checkForHazards(struct GameInfo *theGameInfo, bool *playerIsAlive, int **rooms);

//--------------------------------------------------------------------------------
void displayCave()
{
    printf( "\n"
			"       ______18______             \n"
    		"      /      |       \\           \n"
    		"     /      _9__      \\          \n"
    		"    /      /    \\      \\        \n"
    		"   /      /      \\      \\       \n"
    		"  17     8        10     19       \n"
    		"  | \\   / \\      /  \\   / |    \n"
    		"  |  \\ /   \\    /    \\ /  |    \n"
    		"  |   7     1---2     11  |       \n"
    		"  |   |    /     \\    |   |      \n"
    		"  |   6----5     3---12   |       \n"
    		"  |   |     \\   /     |   |      \n"
    		"  |   \\       4      /    |      \n"
    		"  |    \\      |     /     |      \n"
    		"  \\     15---14---13     /       \n"
    		"   \\   /            \\   /       \n"
    		"    \\ /              \\ /        \n"
    		"    16---------------20           \n"
    		"\n");
}


//--------------------------------------------------------------------------------
void displayInstructions()
{
    printf( "Hunt the Wumpus:                                             \n"
    		"The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
    		"room has 3 tunnels leading to other rooms.                   \n"
    		"                                                             \n"
    		"Hazards:                                                     \n"
        	"1. Two rooms have bottomless pits in them.  If you go there you fall and die.   \n"
        	"2. Two other rooms have super-bats.  If you go there, the bats grab you and     \n"
        	"   fly you to some random room, which could be troublesome.  Then those bats go \n"
        	"   to a new room different from where they came from and from the other bats.   \n"
    		"3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and    \n"
        	"   is too heavy for bats to lift.  Usually he is asleep.  Two things wake       \n"
        	"    him up: Anytime you shoot an arrow, or you entering his room.  The Wumpus   \n"
        	"    will move into the lowest-numbered adjacent room anytime you shoot an arrow.\n"
        	"    When you move into the Wumpus' room, then he wakes and moves if he is in an \n"
        	"    odd-numbered room, but stays still otherwise.  After that, if he is in your \n"
        	"    room, he snaps your neck and you die!                                       \n"
        	"                                                                                \n"
        	"Moves:                                                                          \n"
        	"On each move you can do the following, where input can be upper or lower-case:  \n"
        	"1. Move into an adjacent room.  To move enter 'M' followed by a space and       \n"
        	"   then a room number.                                                          \n"
        	"2. Shoot your guided arrow through a list of up to three adjacent rooms, which  \n"
        	"   you specify.  Your arrow ends up in the final room.                          \n"
        	"   To shoot enter 'S' followed by the number of rooms (1..3), and then the      \n"
        	"   list of the desired number (up to 3) of adjacent room numbers, separated     \n"
        	"   by spaces. If an arrow can't go a direction because there is no connecting   \n"
        	"   tunnel, it ricochets and moves to the lowest-numbered adjacent room and      \n"
        	"   continues doing this until it has traveled the designated number of rooms.   \n"
        	"   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You  \n"
        	"   automatically pick up the arrow if you go through a room with the arrow in   \n"
        	"   it.                                                                          \n"
        	"3. Enter 'R' to reset the person and hazard locations, useful for testing.      \n"
        	"4. Enter 'C' to cheat and display current board positions.                      \n"
        	"5. Enter 'D' to display this set of instructions.                               \n"
        	"6. Enter 'P' to print the maze room layout.                                     \n"
        	"7. Enter 'X' to exit the game.                                                  \n"
        	"                                                                                \n"
        	"Good luck!                                                                      \n"
        	" \n\n");
}//end displayInstructions()


//--------------------------------------------------------------------------------
// Return true if randomValue is already in array
int alreadyFound(int randomValue,		
                 int *randomNumbers,	
                 int limit)				
{
    int returnValue = false;
    
    
    for( int i = 0; i<limit; i++) {
        if( randomValue == randomNumbers[i]) {
            returnValue = true;  
            break;
        }
    }
    
    return returnValue;
} 


//--------------------------------------------------------------------------------

void setUniqueValues(int *randomNumbers,	
                     int size)             
{
    int randomValue = -1;
    
    for( int i = 0; i<size; i++) {
        do {
            randomValue = rand() % NUMBER_OF_ROOMS + 1;   
        } while (alreadyFound(randomValue, randomNumbers, i));
        randomNumbers[i] = randomValue;
    }
} 

//--------------------------------------------------------------------------------
void setRoomConnections( int * **rooms)	
{
	*rooms = malloc(sizeof(int *) * 21);
	for(int i=0; i<21; i++) {
		(*rooms)[i] = malloc(sizeof(int) * 3);
	}
    // Room 0 is unused, to help avoid off-by-one indexing issues
	(*rooms)[0][0] = 0; (*rooms)[0][1] = 0; (*rooms)[0][2] = 0;			// Room 0 (unused)
	(*rooms)[1][0] = 2; (*rooms)[1][1] = 5; (*rooms)[1][2] = 8;			// Room 1
	(*rooms)[2][0] = 1; (*rooms)[2][1] = 3; (*rooms)[2][2] = 10;		// Room 2
	(*rooms)[3][0] = 2; (*rooms)[3][1] = 4; (*rooms)[3][2] = 12;		// Room 3
	(*rooms)[4][0] = 3; (*rooms)[4][1] = 5; (*rooms)[4][2] = 14;		// Room 4
	(*rooms)[5][0] = 1; (*rooms)[5][1] = 4; (*rooms)[5][2] = 6;			// Room 5
	(*rooms)[6][0] = 5; (*rooms)[6][1] = 7; (*rooms)[6][2] = 15;		// Room 6
	(*rooms)[7][0] = 6; (*rooms)[7][1] = 8; (*rooms)[7][2] = 17;		// Room 7
	(*rooms)[8][0] = 1; (*rooms)[8][1] = 7; (*rooms)[8][2] = 9;			// Room 8
	(*rooms)[9][0] = 8; (*rooms)[9][1] = 10; (*rooms)[9][2] = 18;		// Room 9
	(*rooms)[10][0] = 2; (*rooms)[10][1] = 9; (*rooms)[10][2] = 11;		// Room 10
	(*rooms)[11][0] = 10; (*rooms)[11][1] = 12; (*rooms)[11][2] = 19;	// Room 11
	(*rooms)[12][0] = 3; (*rooms)[12][1] = 11; (*rooms)[12][2] = 13;	// Room 12
	(*rooms)[13][0] = 12; (*rooms)[13][1] = 14; (*rooms)[13][2] = 20;	// Room 13
	(*rooms)[14][0] = 4; (*rooms)[14][1] = 13; (*rooms)[14][2] = 15;	// Room 14
	(*rooms)[15][0] = 6; (*rooms)[15][1] = 14; (*rooms)[15][2] = 16;	// Room 15
	(*rooms)[16][0] = 15; (*rooms)[16][1] = 17; (*rooms)[16][2] = 20;	// Room 16
	(*rooms)[17][0] = 7; (*rooms)[17][1] = 16; (*rooms)[17][2] = 18;	// Room 17
	(*rooms)[18][0] = 9; (*rooms)[18][1] = 17; (*rooms)[18][2] = 19;	// Room 18
	(*rooms)[19][0] = 11; (*rooms)[19][1] = 18; (*rooms)[19][2] = 20;	// Room 19
	(*rooms)[20][0] = 13; (*rooms)[20][1] = 16; (*rooms)[20][2] = 19;	// Room 20
} // end setRoomConnections(...)


//--------------------------------------------------------------------------------
// Set the Wumpus, player, bats and pits in distinct random rooms
void initializeGame(struct GameInfo *gameInfo)  
{
    int *randomNumbers;
	randomNumbers = malloc(sizeof(int) * 7);
    
    // Create a set of unique random values 1..20 to be used as room locations.
    setUniqueValues(randomNumbers, 7);
	gameInfo->playerRoom = randomNumbers[0];
	gameInfo->wumpusRoom = randomNumbers[1];
	gameInfo->pitRoom1 = randomNumbers[2];   gameInfo->pitRoom2 = randomNumbers[3];
	gameInfo->batsRoom1 = randomNumbers[4];  gameInfo->batsRoom2 = randomNumbers[5];
    gameInfo->arrowRoom = randomNumbers[6];
    
    gameInfo->moveNumber = 1;

	// Free up the space from randomNumbers
	free(randomNumbers);
}// end initializeGame(...)


//--------------------------------------------------------------------------------
// Returns true if nextRoom is adjacent to current room, else returns false.
int roomIsAdjacent( int tunnels[ 3],     // Array of adjacent tunnels
                    int nextRoom)        // Desired room to move to
{
    return( tunnels[0] == nextRoom ||
            tunnels[1] == nextRoom ||
            tunnels[2] == nextRoom
          );
}


//--------------------------------------------------------------------------------
void displayCheatInfo(struct GameInfo gameInfo)
{
    printf( "Cheating! Game elements are in the following rooms: \n"
    		"Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow  \n"
    		"%4d %5d %6d %5d %5d %5d %5d \n\n", 
			gameInfo.playerRoom,
			gameInfo.wumpusRoom,
			gameInfo.pitRoom1,
			gameInfo.pitRoom2,
			gameInfo.batsRoom1,
			gameInfo.batsRoom2,
			gameInfo.arrowRoom
		);
}


//--------------------------------------------------------------------------------
void displayRoomInfo(int **rooms, struct GameInfo gameInfo)
{
    // Retrieve player's current room number and display it
    int currentRoom = gameInfo.playerRoom;
    printf("You are in room %d. ", currentRoom);
    
    // Retrieve index values of all 3 adjacent rooms
    int room1 = rooms[currentRoom][0];
    int room2 = rooms[currentRoom][1];
    int room3 = rooms[currentRoom][2];
    
    // Display hazard detection message if Wumpus is in an adjacent room
    int wumpusRoom = gameInfo.wumpusRoom;
    if( room1 == wumpusRoom || room2 == wumpusRoom || room3 == wumpusRoom) {
        printf("You smell a stench. ");
    }
    
    // Display hazard detection message if a pit is in an adjacent room
    int pit1Room = gameInfo.pitRoom1;
    int pit2Room = gameInfo.pitRoom2;
    if( room1 == pit1Room || room1 == pit2Room ||
        room2 == pit1Room || room2 == pit2Room ||
        room3 == pit1Room || room3 == pit2Room
      ) {
        printf("You feel a draft. ");
    }

	// Display hazard detection message if bats are in an adjacent room
    int batsRoom1 = gameInfo.batsRoom1;
    int batsRoom2 = gameInfo.batsRoom2;
    if( room1 == batsRoom1 || room1 == batsRoom2 ||
        room2 == batsRoom1 || room2 == batsRoom2 ||
        room3 == batsRoom1 || room3 == batsRoom2
      ) {
        printf("You hear rustling of bat wings. ");
    }
    
    printf("\n\n");
}


//--------------------------------------------------------------------------------
void inRoomWithBats(
        struct GameInfo *gameInfo,    
        bool *playerIsAlive,  
        int **rooms)     
{
    int randomNewRoom = -1;    // Used to transport player to a new room
    
    do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS + 1;
    } while (randomNewRoom == gameInfo->playerRoom);
    
    int playerOldRoom = gameInfo->playerRoom;
    gameInfo->playerRoom = randomNewRoom;
    printf("Woah... you're flying! \n");
    printf("You've just been transported by bats to room %d.\n", gameInfo->playerRoom);
    
    do {
        randomNewRoom = rand() % NUMBER_OF_ROOMS + 1;
    } while (randomNewRoom == gameInfo->playerRoom ||
             randomNewRoom == gameInfo->batsRoom1  ||
             randomNewRoom == gameInfo->batsRoom2 );
    
  
    if( playerOldRoom == gameInfo->batsRoom1) {
        // Change batsRoom1, since that is where the player came from
        gameInfo->batsRoom1 = randomNewRoom;
    }
    else {
        // Change batsRoom2, since that is where the player came from
        gameInfo->batsRoom2 = randomNewRoom;
    }
    
    int playerRoom = gameInfo->playerRoom;
    if( playerRoom == gameInfo->batsRoom1 || playerRoom == gameInfo->batsRoom2 ||
        playerRoom == gameInfo->pitRoom1 || playerRoom == gameInfo->pitRoom2 ||
        playerRoom == gameInfo->wumpusRoom) {
        checkForHazards(gameInfo, playerIsAlive, rooms);
    }
}//end inRoomWithBats(...)


//--------------------------------------------------------------------------------
void checkForHazards(
         struct GameInfo *gameInfo,	// Hazards location and game info
         bool *playerIsAlive,		// player is alive, but could die depending on the hazards
         int **rooms)				// Cave network of rooms
{
    // Retrieve the room the player is in
    int playerRoom = gameInfo->playerRoom;
      
    // Check for the Wumpus
    if( playerRoom == gameInfo->wumpusRoom) {
      
        if( gameInfo->wumpusRoom %2 == 1) {
            // You got lucky and the Wumpus moves away
            printf( "You hear a slithering sound, as the Wumpus slips away. \n"
					"Whew, that was close! \n");
            gameInfo->wumpusRoom = rooms[ playerRoom][ 0];  // Choose the lowest-numbered adjacent room
        }
        else {
            // Wumpus kills you
            printf(	"You briefly feel a slimy tentacled arm as your neck is snapped. \n"
					"It is over.\n");
            *playerIsAlive = false;
            return;
        }
    } //end if( playerRoom...

	// Check whether there is a pit
    if( playerRoom == gameInfo->pitRoom1 || playerRoom == gameInfo->pitRoom2) {
        // player falls into pit
        printf("Aaaaaaaaahhhhhh....   \n");
        printf("    You fall into a pit and die. \n");
        *playerIsAlive = false;
        return;
    }


    if( gameInfo->playerRoom == gameInfo->batsRoom1 || gameInfo->playerRoom == gameInfo->batsRoom2) {
        inRoomWithBats(gameInfo, playerIsAlive, rooms);
    }
}//end checkForHazards(...)


//--------------------------------------------------------------------------------
void shootArrow(int **rooms,          			// Array of connected rooms
                struct GameInfo *gameInfo,      // Locations of hazards, user, Wumpus and arrow
                bool *playerIsAlive,     		// Whether or not player is alive
                bool *wumpusIsAlive)     		// Whether or not Wumpus is alive
{
    int *roomsForArrowTravel;			// Array of up to 3 rooms for arrow to travel through
    int howManyRoomsForArrow = 0;		// How many rooms the user supplies for arrow to travel through
    
    // Ask user to enter number of rooms for arrow to travel through
    printf("Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves: ");
    scanf("%d", &howManyRoomsForArrow);
    if( howManyRoomsForArrow > 3) {
        printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
        howManyRoomsForArrow = 3;
    }

	
	roomsForArrowTravel = malloc(sizeof(int) * howManyRoomsForArrow);

	// Store room numbers into the array
    for( int i=0; i<howManyRoomsForArrow; i++) {
        scanf("%d", &roomsForArrowTravel[ i]);
    }
    
    // Move the arrow through those rooms, ensuring they are adjacent
    gameInfo->arrowRoom = gameInfo->playerRoom;  // arrow starts in player room
    for( int i = 0; i < howManyRoomsForArrow; i++) {
        // Ensure room is adjacent before traveling through it
        if( roomIsAdjacent( rooms[ gameInfo->arrowRoom], roomsForArrowTravel[i]) ) {
            // Room was adjacent, so move arrow there
            gameInfo->arrowRoom = roomsForArrowTravel[i];
            // Kill Wumpus if it is there
            if( gameInfo->arrowRoom == gameInfo->wumpusRoom) {
                *wumpusIsAlive = false;
                printf( "Wumpus has just been pierced by your deadly arrow! \n"
                		"Congratulations on your victory, you awesome hunter you.\n");
                return;
            }
            // Kill player if path went to a second room and then came right back
            if( gameInfo->arrowRoom == gameInfo->playerRoom) {
                *playerIsAlive = false;
                printf( "You just shot yourself.  \n"
                		"Maybe Darwin was right.  You're dead.\n");
                return;
            }
        }
        else {
            // Error in user input, as indicated room is not adjacent.
			printf("Room %d is not adjacent.  Arrow ricochets...\n", roomsForArrowTravel[i]);
            // Move arrow to lowest numbered adjacent rooms
            gameInfo->arrowRoom = rooms[ gameInfo->arrowRoom][ 0];
          
            // If it richochets into room where user is, user dies
            if( gameInfo->arrowRoom == gameInfo->playerRoom) {
                *playerIsAlive = false;
                printf( "You just shot yourself, and are dying.\n"
                		"It didn't take long, you're dead.\n");
                return;
            }
            
            // If it ricochets into room where Wumpus is, Wumpus dies
            if( gameInfo->arrowRoom == gameInfo->wumpusRoom) {
                *wumpusIsAlive = false;
                printf( "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                		"Accidental victory, but still you win!\n");
                return;
            }
        }//end else
        
    }//end for( int i = 0; i < howManyRoomsForArrow
    
   
    gameInfo->wumpusRoom = rooms[ gameInfo->wumpusRoom][ 0];

	// Free up the space for roomsForArrowTravel
	free(roomsForArrowTravel);
}//end shootArrow(...)


//--------------------------------------------------------------------------------

void resetPositions(struct GameInfo *theGameInfo)
{
    printf("Enter the room locations (1..20) for player, wumpus, pit1, pit2, bats1, bats2, and arrow: \n");

	scanf(" %d %d %d %d %d %d %d",  &theGameInfo->playerRoom, &theGameInfo->wumpusRoom, 
                        			&theGameInfo->pitRoom1, &theGameInfo->pitRoom2,
									&theGameInfo->batsRoom1, &theGameInfo->batsRoom2,
									&theGameInfo->arrowRoom);
    printf("\n");
}


//--------------------------------------------------------------------------------
int main(void) {
	int **rooms;       
                                
    struct GameInfo gameInfo;   
    bool playerIsAlive = true;   
    bool wumpusIsAlive = true; 
    char typeOfMove;            
    int nextRoom;               
    
    
    // srand(time(0));
    srand(1);

	// Set the room connection values (allocated using malloc)
	setRoomConnections(&rooms);
    
    // Set random initial values for player, Wumpus, bats and pits
    initializeGame(&gameInfo);

	// Main playing loop.  Break when player dies, or player kills Wumpus
    while( playerIsAlive && wumpusIsAlive) {
        
		// Display current room information: Room number, hazards detected
		displayRoomInfo(rooms, gameInfo);
		
		// Prompt for and handle move
		printf("%d. Enter your move (or 'D' for directions): ", gameInfo.moveNumber);
		scanf(" %c", &typeOfMove);							
		typeOfMove = toupper(typeOfMove);  // Make uppercase to facilitate checking
		
		
		if( typeOfMove == 'D') {
			displayCave();
			displayInstructions();
			continue;       // Loop back up to reprompt for the same move number
		}
		else if( typeOfMove == 'P') {
			// To assist with play, display the cave layout
			displayCave();
			continue;       // Loop back up to reprompt for the same move number
		}
		else if( typeOfMove == 'M') {
			// Move to an adjacent room,
			scanf(" %d", &nextRoom);
			
			if( roomIsAdjacent( rooms[ gameInfo.playerRoom], nextRoom)) {
				gameInfo.playerRoom = nextRoom;        // move to a new room
				
				// Check if pit or wumpus is present in this new room
				checkForHazards(&gameInfo, &playerIsAlive, rooms);

				// Pick up arrow if it is in this new room
				if( gameInfo.arrowRoom == gameInfo.playerRoom) {
					printf("Congratulations, you found the arrow and can once again shoot.\n");
					gameInfo.arrowRoom = -1;   // -1 indicates player has arrow
				}
			}
			else {
				printf("Invalid move.  Please retry. \n");
				continue;        // Doesn't count as a move, so retry same move.
			}
		}
		else if( typeOfMove == 'S') {
			// arrowRoom value of -1 means it is with the player, which must be true for arrow to be shot
			if( gameInfo.arrowRoom == -1) {
				shootArrow(rooms, &gameInfo, &playerIsAlive, &wumpusIsAlive);
			}
			else {
				printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
			}
		}
		else if( typeOfMove == 'C') {
			// Display Cheat information
			displayCheatInfo(gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
		else if( typeOfMove == 'R') {
			// Specify resetting the hazards and player positions, useful for testing
			resetPositions( &gameInfo);
			continue;        // Doesn't count as a move, so retry same move.
		}
		else if( typeOfMove == 'X') {
			// Exit program
			playerIsAlive = false;     // Indicate player is dead as a way to exit playing loop
			break;
		}

		// Increment the move number
		gameInfo.moveNumber = gameInfo.moveNumber + 1;
    } //end while( playerIsAlive && wumpusIsAlive)
	for(int i=0; i<21; i++) {
		free( rooms[i] );
	}
	// Next free the space used for the 20 room pointers
	free(rooms);

    printf("\nExiting Program ...\n");
	return 0;
} 
