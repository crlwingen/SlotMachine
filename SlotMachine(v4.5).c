/*============================================================================*
* Title       : Casino Royale (Slot Machine)
* Description : A slot machine game made with C.
* Filename    : SlotMachine.c
* Version     : v4.5
* Author      : Gensaya, Carl Jerwin F.
* Yr&Sec&Uni  : BSCS 1-1N PUP Main
* Subject     : Programming 2
*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

int intTemp,    intCtr,      intCtr2,     intChoice,    intSlotUp[4], intSlotDown[4],
	intSpinDur, intSlotSlow, intSpeed,    intSpinCount, intFlag = 0,
	intBet,   intLastBet, intMoney,    intSlots[3], intPrize, intFlag, intScore;

char strPlayName[10], strTemp[50], strTemp1[50], strTemp2[50],
	 strLoadName[10], chrChoice, strTxtName[50], strLogFile[50];	 
	 
struct playProfile {
	char strName[20];
	int  intHScore;
	int intRound;
} play[50];

struct playTemp {
	char strTname[20];
	int  intTscore;
} temp[50];

void fnSlotConfig();					      // CHECKING EXISTENSE OF CONFIG.INI
void fnOption();							  // CHANGE SETTINGS OF THE GAME
void fnNewGame(); 							  // START NEW GAME
void fnSlotMecha();							  // SPINS THE SLOT
void fnSlotMachine();						  					  // SLOT MACHINE FRAME
void fnSlots(int intColumn, int intSlotNum, int intUpDown); 	 // SLOT MACHINE REELS
void fnPrizeChecker(int intFlag);			  // CALCULATES THE PAY-OUT PRIZE
void fnHighScore();

void fnCoorXY(short int x, short int y );     // CURSOR POSITIONING
void fnTitleScreen();							      // WELCOME SCREEN
void fnGameMenu();							  // MAIN MENU
void fnLoadGame(); 							  // LOAD GAME VIA PLAYER NAME
void fnSaveGame(); 							  // SAVE CURRENT GAME STATE
void fnShowScores();
void fnHelp();								  // GAME INSTRUCTIONS
void fnAbout();	 							  // CREDITS ON THE CREATORS
void fnExit();								  // EXIT GAME

FILE *SlotLog,								  // FILE CREATOR FOR SLOT ROLLS RESULT
	 *SlotConfig,							  // CONFIGURATION FILE
	 *SlotSaveFile,							  // SAVE FILE .TXT FILE
	 *SlotScores;


/*============================================================================*
     *  Function   : main
     *  Params     : None 
     *  Returns    : Void
     *  Description: Main function of the program.
*=============================================================================*/
int main() { 
	fnSlotConfig();
	return 0;
} // End of main(void)


/*============================================================================*
     *  Function   : fnCoorXY
     *  Params     : short int x: X-coordinate of the cursor placement.
					 short int y: Y-coordinate of the cursor placement.
     *  Returns    : Void
     *  Description: Cursor placement repositioning.
*=============================================================================*/
void fnCoorXY(short int x, short int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition( GetStdHandle ( STD_OUTPUT_HANDLE ), pos );
} // End of void fnCoorXY( short int x, short int y )


/*============================================================================*
     *  Function   : fnSlotConfig
     *  Params     : none
     *  Returns    : Void
     *  Description: Checks for a config file existence for setting up the
                     game. If there is none, prompts the user for the desired
                     starting set-up of the game.
*=============================================================================*/
void fnSlotConfig() {
	SetConsoleTitle("\"They see me rollin', they waitin'.\"  - Slot Reels");
	srand ( time(NULL) );
	system("cls");
	system("mode 100, 50");
		
	SlotConfig = fopen("SlotConfig.ini", "r");
	
	if( ! SlotConfig ) {
		fnCoorXY(15, 7); printf("The .ini file not found. Input settings manually.");
		fnCoorXY(15, 10); printf("Enter Spin Duration (in secs):                          "); 
		scanf("%i", &intSpinDur); fflush(stdin);
		fnCoorXY(15, 11); printf("Enter Spin Duration before stopping (in secs):          "); 
		scanf("%i", &intSlotSlow); fflush(stdin);
		fnCoorXY(15, 12); printf("Enter Spin Speed (1: Fastest to 10: Slowest) :          "); 
		scanf("%i", &intSpeed);
		
		if( intSpeed > 10 || intSpeed <= 0) {
			fnCoorXY(18, 15); printf("!!Error");
			getch(); fnSlotConfig();
			
		}
		
		fnCoorXY(18, 15); printf("~SlotConfig.ini file created. Enjoy playing! :)");
		getch();
		SlotConfig = fopen("SlotConfig.ini", "a");
		fprintf(SlotConfig, "SpinDuration=%i\nSlowSlow_In=%i\nSlotSpeed=%i", intSpinDur, intSlotSlow, intSpeed);
		fclose(SlotConfig);
	
	} else {
		while ( ! feof(SlotConfig) )
			fscanf(SlotConfig, "%[^=]=%i\n%[^=]=%i\n%[^=]=%i", strTemp, &intSpinDur, strTemp, &intSlotSlow, strTemp, &intSpeed);
	
	}	
	
	fclose(SlotConfig);
	fnTitleScreen(); 
} // End of void fnSlotConfig()

/*============================================================================*
     *  Function   : fnTitleScreen
     *  Params     : None
     *  Returns    : Void
     *  Description: Welcome screen of the game.
*=============================================================================*/
void fnTitleScreen() {
	
	system("cls");
	int intDelay = 50;
	
	for(intCtr = 0; intCtr < 3; intCtr++) {	
		system("cls");
		fnCoorXY(2, 9);  printf("------------------------------------------------------------------------------------------");
		fnCoorXY(2, 19); printf("------------------------------------------------------------------------------------------");
		fnCoorXY(2, 11);  printf("  CCCC   AAAA   SSS  IIII NN     N  OOOO     RRRRRR    OOOO Y      Y AAAA  LL    EEEEE !! \n");
		Sleep(intDelay);
		fnCoorXY(2, 12); printf(" C    C A    A S   S  II  NN     N O    O    R     R  O    O Y    Y A    A LL    EE    !! \n");
		Sleep(intDelay);
	    fnCoorXY(2, 13); printf(" C      A    A S      II  N N    N O    O    R     R  O    O  Y  Y  A    A LL    EE    !! \n");
	    Sleep(intDelay);
	  	fnCoorXY(2, 14); printf(" C      A    A SSSS   II  N  N   N O    O    RRRRRR   O    O   YY   A    A LL    EEEE  !! \n");
	  	Sleep(intDelay);
		fnCoorXY(2, 15); printf(" C      AAAAAA     S  II  N   N  N O    O    R    R   O    O   YY   AAAAAA LL    EE    !! \n");
		Sleep(intDelay);
		fnCoorXY(2, 16); printf(" C   C  A    A S   S  II  N    NNN O    O    R     R  O    O   YY   A    A LL    EE         \n");      
		Sleep(intDelay);
		fnCoorXY(2, 17); printf("  CCCC  A    A  SSS  IIII N     NN  OOOO     R      R  OOOO    YY   A    A LLLLL EEEEE !! \n");
		Sleep(intDelay);
	
	} fnCoorXY(33, 22); printf("Press any key to continue...");
	  getch(); getch();
  	  fnHighScore();  
	  fnGameMenu();
} // End of void fnTitleScreen()

/*============================================================================*
     *  Function   : fnGameMenu
     *  Params     : None
     *  Returns    : Void
     *  Description: Main menu of the game.
*=============================================================================*/
void fnGameMenu() {
	intCtr 			= 0;
	intMoney     	= 5000;
	intBet     		= 0;
	intSpinCount	= 1;
	intLastBet 		= 0;
	intScore 		= 0;
	intSlots[0]     = 0;
	intSlots[1]     = 0;
	intSlots[2]     = 0;
	intSlots[3]     = 0;
	
	system("cls");
	fnCoorXY(9, 6);   printf("-->");
	fnCoorXY(60, 23); printf("( Press 'W' ^  |           )");
	fnCoorXY(60, 24); printf("(           |  v Press 'S' )");
	fnCoorXY(60, 26); printf("( Press 'Enter' to Select  )");
	
	while(1) {
			
		fnCoorXY(33, 1);  printf("==========================");
		fnCoorXY(33, 2);  printf(" Casino Royale: MAIN MENU");
		fnCoorXY(33, 3);  printf("==========================");
		fnCoorXY(13, 6);  printf("[1] NEW GAME");
		fnCoorXY(13, 8);  printf("[2] LOAD GAME");
		fnCoorXY(13, 10); printf("[3] HIGH SCORES");
		fnCoorXY(13, 12); printf("[4] HELP");
		fnCoorXY(13, 14); printf("[5] CREDITS");
		fnCoorXY(13, 16); printf("[6] OPTIONS");
		fnCoorXY(13, 18); printf("[7] EXIT");
		
		chrChoice = getch();
		
		if(chrChoice == 'w' || chrChoice == 'W') {
			if(intCtr <= 0) {
				intCtr = 0;
				fnCoorXY(9, 6); printf("-->");
			} else {
				intCtr-=2;
				fnCoorXY(9, 6 + intCtr - 2); printf("   ");
				fnCoorXY(9, 6 + intCtr);     printf("-->");
				fnCoorXY(9, 6 + intCtr + 2); printf("   ");
			}
		}
		else if(chrChoice == 's' || chrChoice == 'S') {
			if(intCtr >= 12) {
				intCtr = 12;
				fnCoorXY(9, 18); printf("-->");
			} else {
				intCtr+=2;
				fnCoorXY(9, 6 + intCtr - 2); printf("   ");
				fnCoorXY(9, 6 + intCtr);     printf("-->");
				fnCoorXY(9, 6 + intCtr + 2); printf("   ");
			}
		} else if(chrChoice == '\r') {
			if( intCtr == 0 ) {
				fnCoorXY(42, 6); printf("|\t   Enter player name: ");
				scanf("%s", &strPlayName);
				SlotLog = fopen("SlotLog.txt", "a");
				fprintf(SlotLog, "Player Nam -> %s\n", strPlayName);
				fclose(SlotLog);
				fnNewGame();
			
			} else if( intCtr == 2 )
				fnLoadGame();
			
			 else if( intCtr == 4 )
				fnShowScores();
			
			  else if( intCtr == 6 )	
				fnHelp();
				
			  else if( intCtr == 8 )
			  	fnAbout();
			  
			  else if( intCtr == 10 )
			  	fnOption();
			  	
			  else if( intCtr == 12 ) {
				fnCoorXY(42, 18); printf("|\t   Are you sure (Y/N)? : ");
				chrChoice = getch();
				
				if( chrChoice == 'Y' || chrChoice == 'y' )
					fnExit();
			    
				else
					fnTitleScreen();
					
			}
		}	
	}
} // End of void fnGameMenu(int intBet)

/*============================================================================*
     *  Function   : fnNewGame
     *  Params     : None
     *  Returns    : Void
     *  Description: Game screen.
*=============================================================================*/
void fnNewGame() {
	system("cls");
	
	fnSlotMachine();
	fnCoorXY(30, 10); printf("Round No. %i", intSpinCount);
	fnCoorXY(30, 11); printf("-----------");
	fnCoorXY(59, 15); printf("%i", intSlotUp[0]);
	fnCoorXY(59, 17); printf("%i", intSlots[0]);
	fnCoorXY(59, 19); printf("%i", intSlotDown[0]);
	fnCoorXY(65, 15); printf("%i", intSlotUp[1]);
	fnCoorXY(65, 17); printf("%i", intSlots[1]);
	fnCoorXY(65, 19); printf("%i", intSlotDown[1]);
	fnCoorXY(71, 15); printf("%i", intSlotUp[2]);
	fnCoorXY(71, 17); printf("%i", intSlots[2]);
	fnCoorXY(71, 19); printf("%i", intSlotDown[2]);
	fnCoorXY(77, 15); printf("%i", intSlotUp[3]);
	fnCoorXY(77, 17); printf("%i", intSlots[3]);
	fnCoorXY(77, 19); printf("%i", intSlotDown[3]);
	fnCoorXY(7, 24);  printf("                              ");
	fnCoorXY(7, 25);  printf("                              ");
	fnCoorXY(5, 17);  printf("                        ");
	fnCoorXY(33, 1);  printf("=========================");
	fnCoorXY(33, 2);  printf(" Casino Royale: NEW GAME");
	fnCoorXY(33, 3);  printf("=========================");
	fnCoorXY(30, 36);  printf("[Game Status] ");
	fnCoorXY(33, 37); printf("Player Name         : %s", strPlayName);
	fnCoorXY(33, 38); printf("Now on Round        : # %i", intSpinCount);
	fnCoorXY(33, 39); printf("Last Combination    : %i, %i, %i, %i", intSlots[0], intSlots[1], intSlots[2], intSlots[3]);
	fnCoorXY(33, 40); printf("Last Payback        : T %i", intPrize);
	fnCoorXY(33, 41); printf("Last Bet            : T %i", intLastBet);
	
	if(intScore < intMoney)
		intScore = intMoney;
	
	fnCoorXY(33, 42); printf("Highest Money Bank  : T %i", intScore);
	intBet = 0;
	
	if( intMoney == 0 ) {
		fnCoorXY(5, 14); printf("   No money left: GAME OVER!");
		SlotScores = fopen("SlotScores.txt", "a");
		fprintf(SlotScores, "0\t%s\t%i\t%i\n",strPlayName, intSpinCount - 1, intScore);
		fclose(SlotScores);
		fnHighScore(); getch();
		fnTitleScreen();
				
	} while(1) {
		fnCoorXY(33, 1);  printf("=========================");
		fnCoorXY(33, 2);  printf(" Casino Royale: NEW GAME");
		fnCoorXY(33, 3);  printf("=========================");
		fnCoorXY(5, 6);   printf("---------------------------------------------------------------------------------------");
		fnCoorXY(5, 7);   printf("|       W: Increase Bet     |       S: Decrease Bet     |        Space: Spin Slot     |");
		fnCoorXY(5, 8);   printf("---------------------------------------------------------------------------------------");
		fnCoorXY(6, 13);  printf("   Player : %s\n         Money  : T %i", strPlayName, intMoney);
		fnCoorXY(6, 15);  printf("   Bet    : T %i", intBet);
		
		for( intCtr = 0; intCtr < 23; intCtr++ ) {
			fnCoorXY(44, 9 + intCtr);  printf("|");
			fnCoorXY(5,  9 + intCtr);  printf("|");
			fnCoorXY(91, 9 + intCtr); printf("|");
			
		}

		fnCoorXY( 4, 32); printf(" ---------------------------------------------------------------------------------------");
		fnCoorXY( 4, 33); printf(" |              P: Main Menu               |                 O: Save & Menu            |");
		fnCoorXY( 4, 34); printf(" ---------------------------------------------------------------------------------------");
		
		chrChoice = getch();
		
		if( chrChoice == 'w' || chrChoice == 'W' ) {
			fnCoorXY(25, 16); printf("                     ");
		
			if( intMoney > 0 ) { 
				intBet+= 50;
				intMoney-= 50;
				fnCoorXY(6, 16);  printf("                  ");
				fnCoorXY(20, 14); printf("                  ");
				fnCoorXY(20, 14); printf("%i", intMoney);
				fnCoorXY(14, 15); printf("                  ");
				fnCoorXY(14, 15); printf("%i", intBet);
				
			} else if( intMoney == 0 ) {
				fnCoorXY(25, 16); printf("Not enough tokens!");
				
			}
		} else if ( chrChoice == 's' || chrChoice == 'S') {
			fnCoorXY(25, 16); printf("                       ");
			
			if( intBet > 0 ) { 	
				intBet-= 50;
				intMoney+= 50;
				fnCoorXY(20, 14); printf("                  ");
				fnCoorXY(20, 14); printf("%i", intMoney);
				fnCoorXY(14, 15); printf("                  ");
				fnCoorXY(14, 15); printf("%i", intBet);
				
			} else if( intBet == 0 ) {
				fnCoorXY(25, 16); printf("     No bet left!");
				
			}
		} else if( chrChoice == ' ' ) {
			fnCoorXY(25, 16); printf("                  ");
			
			if( intBet == 0 ) {
				fnCoorXY(6, 19); printf("   ~Bet something first.");
			
			} else {	
				fnCoorXY(6, 19); printf("   ~Slots are rolling.  ");
				break;
				
			}
		} else if( chrChoice == 'p' || chrChoice == 'P') {
			fnCoorXY(16, 33); printf("Are you sure (Y/N)? ");
			chrChoice = getch();
			
				if( chrChoice == 'y' || chrChoice == 'Y' ) {
					fclose(SlotLog);
					fnGameMenu();
					
				} else
					fnNewGame();
					
		} else if( chrChoice == 'o' || chrChoice == 'O') {
			fnCoorXY(58, 33); printf("Are you sure (Y/N)? ");
			chrChoice = getch();
			
				if( chrChoice == 'y' || chrChoice == 'Y' ) {
					fclose(SlotLog);
					fnSaveGame();
					
				} else
					fnNewGame();
					
		}
	} fnSlotMecha();
	
} // End of void fnNewGame() 

/*============================================================================*
     *  Function   : fnLoadGame
     *  Params     : None
     *  Returns    : Void
     *  Description: Load existing saved games.
*=============================================================================*/
void fnLoadGame() {
	system("cls");
	intFlag = 0;
	
	fnCoorXY(33, 1);  printf("==========================");
	fnCoorXY(33, 2);  printf(" Casino Royale: LOAD GAME");
	fnCoorXY(33, 3);  printf("==========================");
	fnCoorXY(30, 11); printf("=================================");
	fnCoorXY(30, 19); printf("=================================");
	
	SlotSaveFile = fopen("SlotSaveFiles.txt", "r");
	
	if ( ! SlotSaveFile ) {
		fnCoorXY(34, 15); printf("There are no saved files.");
		getch(); 
		fclose(SlotSaveFile);
		fnGameMenu();
	
	} else {
		fnCoorXY(30, 6);  printf("Enter Player Name to be Loaded: ");
		scanf("%s", &strLoadName);
		
		while( fscanf(SlotSaveFile, "%[^:]: %s\n %[^:]: %i %[^:]: %i", strTemp, strPlayName, strTemp, &intMoney, strTemp, &intSpinCount) != EOF ) {
			if( strcmp(strPlayName, strLoadName) == 0 )
				intFlag = 1;
				
			if( intFlag == 1 ) {
				fnCoorXY(31, 13); printf("Player Name:\t%s", strPlayName);
				fnCoorXY(31, 14); printf("Token:\t\t%i", intMoney);
				fnCoorXY(31, 15); printf("Spin Count:\t%i", intSpinCount);
				fnCoorXY(31, 17); printf("Is this your save file(Y/N)? ");
				chrChoice = getch();
				
				if(chrChoice == 'y' || chrChoice == 'Y') {
					getch();
					SlotLog = fopen("SlotLog.txt", "a");
					fprintf(SlotLog, "\n\nPlayer Name: %s\n\n", strPlayName);
					fclose(SlotSaveFile);
					fnNewGame();
				
				} else {
					fnCoorXY(31, 13); printf("                                 ");
					fnCoorXY(31, 14); printf("                                 ");
					fnCoorXY(31, 15); printf("                                 ");
					fnCoorXY(31, 17); printf("                                   ");
					intFlag = 0;
				}
			}	
		}
	}  
	
	fnCoorXY(37, 15); printf("Save file not found!  ");
	fclose(SlotSaveFile);
	getch(); fnGameMenu();
} // End of void fnLoadGame()

/*============================================================================*
     *  Function   : fnSaveGame
     *  Params     : None
     *  Returns    : Void
     *  Description: Save current game status.
*=============================================================================*/
void fnSaveGame() {
	SlotSaveFile = fopen("SlotSaveFiles.txt", "a");
	system("attrib +h SlotSaveFiles.txt");
	
	fprintf(SlotSaveFile, "\nPlayerName:%s\nToken:%i\nSpinCount:%i\n\n", strPlayName, intMoney + intBet, intSpinCount);
	
	fclose(SlotSaveFile);
	fnGameMenu();
	
} // End of void fnSaveGame()

/*============================================================================*
     *  Function   : fnHelp
     *  Params     : None
     *  Returns    : Void
     *  Description: Contains how-to-play section of the game.
*=============================================================================*/
void fnHelp() {
	system("cls");
	
	fnCoorXY(33, 1);  printf("=====================");
	fnCoorXY(33, 2);  printf(" Casino Royale: HELP");
	fnCoorXY(33, 3);  printf("=====================");
	fnCoorXY(5, 6);   printf("-----------");
	fnCoorXY(5, 5);   printf("How to Play:\n\n\n       Add bet by pressing the [W] button,\n       decrease bet by pressing the [S] button");
	printf("\n       and pressing [Space] key to make the slots spin.");
	fnCoorXY(5, 15);  printf("Prize Multiplier:");
	fnCoorXY(5, 16);  printf("----------------");
	fnCoorXY(7, 18);  printf("|  7  |  7  |  7  |  7  |\t: Bet x 15");
	fnCoorXY(7, 20);  printf("|  =  |  =  |  =  |  =  |\t: Bet x 12");
	fnCoorXY(7, 22);  printf("|  7  |  7  |  7  | ANY | or");
	fnCoorXY(7, 23);  printf("|  7  |  7  | ANY |  7  | or");
	fnCoorXY(7, 24);  printf("|  7  | ANY |  7  |  7  | or");
	fnCoorXY(7, 25);  printf("| ANY |  7  |  7  |  7  |\t: Bet x 10");
	fnCoorXY(7, 27);  printf("|  7  |  7  |  7  | ANY | or");
	fnCoorXY(7, 28);  printf("|  7  |  7  | ANY |  7  | or");
	fnCoorXY(7, 29);  printf("|  7  | ANY |  7  |  7  | or");
	fnCoorXY(7, 30);  printf("| ANY |  7  |  7  |  7  |\t: Bet x  2");
	
	getch(); fnGameMenu();
	
} // End of void fnHelp()

/*============================================================================*
     *  Function   : fnAbout
     *  Params     : None
     *  Returns    : Void
     *  Description: About section of the game.
*=============================================================================*/
void fnAbout() {
	system("cls");
	fnCoorXY(33, 1);  printf("========================");
	fnCoorXY(33, 2);  printf(" Casino Royale: CREDITS");
	fnCoorXY(33, 3);  printf("========================");
	fnCoorXY(8, 8);   printf("-----------------------------------------------");
	fnCoorXY(8, 7);   printf("A slot machine game created by BSCS 1-1D GRP. 7:\n\n\n\t  Masangkay, Erm M.");
	printf("\n\n\t  Ilagan, Ma. Elpedia P.\n\n\t  Pancho, Trisha A.\n\n\t  Gensaya, Carl Gerwin F. (Programmer)\n\n\t  Olaño, Emilson B.");
	fnCoorXY(8, 23);  printf("Special Mentions:");
	fnCoorXY(8, 24);  printf("----------------");
	fnCoorXY(10, 26); printf("The Reddit's LearnProgramming Community.");
	fnCoorXY(10, 28); printf("Redditor: MARKZILLA   (       Slot Spinning Delay Help     )");
	fnCoorXY(10, 30); printf("Raymel Franciso       (  Windows Size Fix & Beta Testing   )");
	fnCoorXY(10, 32); printf("Andrew Santilices     (  Reading .txt Help & Beta Testing  )");
	fnCoorXY(10, 34); printf("Christian Joy Relato  (            Beta Testing            )");
	
	getch(); fnGameMenu();
	
} // End of void fnAbout()

/*============================================================================*
     *  Function   : fnOption
     *  Params     : None
     *  Returns    : Void
     *  Description: Change current setting of the game.
*=============================================================================*/
void fnOption() {
	system("cls");
	fnCoorXY(33, 1); printf("=======================");
	fnCoorXY(33, 2); printf(" Casino Royale: OPTION");
	fnCoorXY(33, 3); printf("=======================");
	fnCoorXY(8, 5);  printf("-----------------\n");
	printf("\tCurrent Settings:"); 
	fnCoorXY(8, 7);  printf("-----------------\n");
	printf("\n\t  [1] Spin Duration(in secs)\t|  %i\n\n\t  [2] Slot Slow Time(in secs)\t|  %i\n\n\t  [3] Spinning Speed\t\t|  %i", intSpinDur, intSlotSlow, intSpeed);
	fnCoorXY(10, 15); printf("~Do you wanna change the settings (Y/N)? ");
	chrChoice = getch();

	if( chrChoice == 'Y' || chrChoice == 'y') {
		SlotConfig = fopen("SlotConfig.ini",  "w+");
		fnCoorXY(10, 18); printf("Enter Spin Duration (in secs.)\t|  "); 
		scanf("%i", &intSpinDur); fflush(stdin);
		fnCoorXY(10, 20); printf("Enter Slot Slow Time (in secs.)\t|  ");
		scanf("%i", &intSlotSlow); fflush(stdin);
		fnCoorXY(10, 22); printf("Enter Slot Spin Speed\t\t\t|  ");
		scanf("%i", &intSpeed);
		
		if( intSpeed >= 0 && intSpeed <= 10 ) {
			fnCoorXY(10, 26); printf("DONE!");
			fprintf(SlotConfig ,"SpinDuration=%i\nSlotSlow_In=%i\nSlotSpeed=%i", intSpinDur, intSlotSlow, intSpeed);
			fclose(SlotConfig);
			getch(); fnOption();
		
		} else {
			intSpeed = 1;
			fnCoorXY(10, 26); printf("ERROR! (Check Speed Input)");
			fclose(SlotConfig);
			getch(); fnOption();
			
		}
	} else 
		fnGameMenu();
	
}

/*============================================================================*
     *  Function   : fnExit
     *  Params     : None
     *  Returns    : Void
     *  Description: Close the game.
*=============================================================================*/
void fnExit() {
	system("cls");
	fnCoorXY(30, 8);  printf("      \" If you must play,");
	Sleep(400);
	fnCoorXY(30, 10); printf("decide three thing upon the start.");
	Sleep(400);
	fnCoorXY(30, 12); printf("      the RULES OF THE GAME,");
	Sleep(400);
	fnCoorXY(30, 14); printf("           the STAKES,");
	Sleep(400);
	fnCoorXY(30, 16); printf("     and the QUITTING TIME. \"\n\n\n\n\n");
	getch(); exit(0);
	
} // End of void fnExit()

/*============================================================================*
     *  Function   : fnSlotMecha
     *  Params     : None
     *  Returns    : Void
     *  Description: Slot machine animations.
*=============================================================================*/
void fnSlotMecha() {
	
	fnSlotMachine();
	
	fnCoorXY(88, 14); printf(" ");
	fnCoorXY(88, 15); printf(" ");
	fnCoorXY(88, 16); printf(" ");
	Sleep(250);
	fnCoorXY(88, 18); printf("|");
	fnCoorXY(88, 19); printf("|");
	fnCoorXY(88, 20); printf("O");
	Sleep(250);
	fnCoorXY(88, 18); printf(" ");
	fnCoorXY(88, 19); printf(" ");
	fnCoorXY(88, 20); printf(" ");
	Sleep(250);
	fnCoorXY(88, 16); printf("|");
	fnCoorXY(88, 15); printf("|");
	fnCoorXY(88, 14); printf("O");

	int intSlotCol[4];
    	intSlotCol[0] = 59;
    	intSlotCol[1] = 65;
    	intSlotCol[2] = 71;
		intSlotCol[3] = 77;	
	time_t 	StartTime = time(0);
	
    time_t 	SlotStopTime[4];
    	   	SlotStopTime[0] = StartTime + intSpinDur + intSlotSlow;
    	   	SlotStopTime[1] = StartTime + intSpinDur + ( intSlotSlow * 2 );
    	   	SlotStopTime[2] = StartTime + intSpinDur + ( intSlotSlow * 3 );
    	   	SlotStopTime[3] = StartTime + intSpinDur + ( intSlotSlow * 4 );

    clock_t SlotUpdate[4];
    		SlotUpdate[0] = 0;
    		SlotUpdate[1] = 0;
    		SlotUpdate[2] = 0;
    		SlotUpdate[3] = 0;

    	
      while(1) {	
      	time_t CurrentTime = time(0);
    
        for(intCtr = 0; intCtr < 4; ++intCtr) {	
            if( CurrentTime >= SlotStopTime[intCtr] ) { 		// DETERMINES THE SLOT THE BE PROCESSED
            	fnCoorXY(intSlotCol[intCtr], 21); printf("o");
                continue;
            
			} clock_t SysClock = clock();
            
			if( SysClock < SlotUpdate[intCtr] ) {  				// DETERMINES THE SLOT TO BE UPDATED
			    continue;
            
			} time_t TimeDifference = SlotStopTime[intCtr] - CurrentTime;
            
			if( TimeDifference <= intSlotSlow) {
			    fnSlots(intSlotCol[intCtr], intCtr, intCtr);
                SlotUpdate[intCtr] += (clock_t)( .5 * CLOCKS_PER_SEC);
           
           } else {
		        fnSlots(intSlotCol[intCtr], intCtr, intCtr);
                SlotUpdate[intCtr] += (clock_t)( (.01 * intSpeed) * CLOCKS_PER_SEC);
                
        	}
        } if ( CurrentTime == SlotStopTime[3] )											
        		break;	
    } // THE WHILE(1) LOOP BREAKER
    
	fnCoorXY(6, 19); printf("   Slots have stopped.   ");
	fnPrizeChecker(intFlag); getch();
	fnCoorXY(6, 19); printf("   Place your bet.        ");
	getch();
	intBet = 0; fnNewGame();
} // End of void fnSlotMecha()

/*============================================================================*
     *  Function   : fnSlotMachine
     *  Params     : None
     *  Returns    : Void
     *  Description: Slot machine skeleton.
*=============================================================================*/
void fnSlotMachine() {
	fnCoorXY(46, 11); printf("    x^---------------------------------^x\n");
	fnCoorXY(46, 12); printf("    |oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOo|\n");
	fnCoorXY(46, 13); printf("    \\__________________________________/\n");
	fnCoorXY(46, 14); printf("     |    \\--^--.--^--.--^--.--^--/    |  O");
	fnCoorXY(46, 15); printf("     |    |     |     |     |     |    |  |");
	fnCoorXY(46, 16); printf("    _|    |-----+-----+-----+-----|    |  |");
	fnCoorXY(46, 17); printf("  o[ |  ->|     |     |     |     |<-  |]-+");
	fnCoorXY(46, 18); printf("    -|    |-----+-----+-----+-----|    |");
	fnCoorXY(46, 19); printf("     |    |     |     |     |     |    |");
	fnCoorXY(46, 20); printf("     |    /~-v-~.~-v-~.~-v-~.~-v-~ \\   |");
	fnCoorXY(46, 21); printf("      \\__(                         )__/");
	fnCoorXY(46, 22); printf("       |``````````````````````````````|");
	fnCoorXY(46, 23); printf("       /oooooooooooooooooooooooooooooo\\");
	fnCoorXY(46, 24); printf("       |______________________________|");
	fnCoorXY(46, 25); printf("       |                              |");
	fnCoorXY(46, 26); printf("       |         Payback T            |");
	fnCoorXY(46, 27); printf("       Y______________________________Y");
	fnCoorXY(46, 28); printf("         ||                       ||");
	fnCoorXY(46, 29); printf("        _||                       ||_");
	
} // End of void fnSlotMachine()

/*============================================================================*
     *  Function   : fnSlots
     *  Params     : None
     *  Returns    : Void
     *  Description: Content of the slot machine's reels.
*=============================================================================*/
void fnSlots(int intColumn, int intSlotNum, int intUpDown) {
	intSlots[intSlotNum]   			 = rand() % 7 + 2;
	intSlotUp[intSlotNum]            = intSlots[intSlotNum] + 1;
	intSlotDown[intSlotNum]          = intSlots[intSlotNum] - 1; 
	fnCoorXY(intColumn, 15); printf("%i", intSlotUp[intSlotNum]);
	fnCoorXY(intColumn, 17); printf("%i", intSlots[intSlotNum]);
	fnCoorXY(intColumn, 19); printf("%i", intSlotDown[intSlotNum]);

} // End of void fnSlots(int intColumn, int intSlotNum)

/*============================================================================*
     *  Function   : fnPrizeChecker
     *  Params     : None
     *  Returns    : Void
     *  Description: Game prize calculator.
*=============================================================================*/
void fnPrizeChecker(int intFlag) {
	intFlag = 1;
	intLastBet = intBet;
	
	SlotLog = fopen("SlotLog.txt", "a");
	fnCoorXY(7, 22); printf("------------------------------------");
	if( intSlots[0] == 7 && intSlots[1] == 7 && intSlots[2] == 7 && intSlots[3] == 7) {
		intMoney = intBet * 10 + intMoney; intPrize = intBet * 15; 
		fnCoorXY(6, 20);  printf("   JACKPOT! : T %i", intPrize);
		fnCoorXY(74, 26); printf("%i", intPrize);
		fnCoorXY(7, 26);  printf("Congratulations!");
		fnCoorXY(7, 27);  printf("You just hit a JACKPOT!");
		
	} else if( intSlots[0] == 7 && intSlots[1] == 7 && intSlots[2] == 7 || intSlots[0] == 7 && intSlots[1] == 7 && intSlots[3] == 7 || 
			   intSlots[0] == 7 && intSlots[2] == 7 && intSlots[3] == 7 || intSlots[1] == 7 && intSlots[2] == 7 && intSlots[3] == 7 ) {
		intMoney = intBet * 10 + intMoney;	 intPrize = intBet * 10;
		fnCoorXY(6, 20);  printf("   Prize  : S %i", intPrize);
		fnCoorXY(74, 26); printf("%i", intPrize);
		fnCoorXY(7, 26);  printf("  Pretty nice. Keep Spinnin,");
		fnCoorXY(7, 27);  printf("  Lucky Ones!");
		
	} else if( intSlots[0] == intSlots[1] && intSlots[1] == intSlots[2] && intSlots[2] == intSlots[3]) {
		intMoney = intBet * 6 + intMoney; intPrize = intBet * 12;
		fnCoorXY(6, 20);  printf("   Prize  : S %i", intPrize);
		fnCoorXY(74, 26); printf("%i", intPrize);
		fnCoorXY(7, 26);  printf("  Woah! That was a nice spin. Cheers!");
		
	} else if( intSlots[0] == intSlots[1] || intSlots[0] == intSlots[2] || intSlots[0] == intSlots[3] || intSlots[1] == intSlots[2] ||
			   intSlots[1] == intSlots[3] || intSlots[2] == intSlots[3]) { 
		intMoney = intBet * 2 + intMoney; intPrize = intBet * 2;
		fnCoorXY(6, 20);  printf("   Prize  : S %i", intPrize);
		fnCoorXY(74, 26); printf("%i", intPrize);
		fnCoorXY(7, 26);  printf("   Not bad. Keep on going.");
		
	} else {
		intFlag = 0;
		intPrize = 0;
		fnCoorXY(6, 20);  printf("   Prize  : T 0");
		fnCoorXY(74, 26); printf("%i", intPrize);
		fnCoorXY(7, 26);  printf("   Awww. Too bad. Spin again");
		fnCoorXY(7, 27);  printf("   and test your luck!");
	
	} intTemp = intMoney + intBet;
	
	if( intFlag == 1 ) 
		fprintf(SlotLog, "%i : %i %i %i %i  = T %i + %i\t\tTotal : T % i\n", intSpinCount, intSlots[0], intSlots[1], intSlots[2], intSlots[3], intMoney - intPrize, intPrize, intMoney);
	
	else if( intFlag == 0 )
		fprintf(SlotLog, "%i : %i %i %i %i  = T %i - %i\t\tTotal : T % i\n", intSpinCount, intSlots[0], intSlots[1], intSlots[2], intSlots[3], intTemp, intBet, intTemp - intBet);
		
	intSpinCount++;
} // End of void fnPrizeChecker(int intFlag)

/*============================================================================*
     *  Function   : fnHighScore
     *  Params     : None
     *  Returns    : Void
     *  Description: Reads the high scores ranking of all game sessions.
*=============================================================================*/
void fnHighScore() {
	int intStructSize, intTemp2;
	SlotScores = fopen("SlotScores.txt", "r");
	intCtr = 0;
	
	while(fscanf(SlotScores, "%i %s %i %i", &intTemp, play[intCtr].strName, &play[intCtr].intRound, &play[intCtr].intHScore) != EOF) {
			intCtr++;
		
	}
	intStructSize = intCtr;
	fclose(SlotScores);
	
	for(intCtr = 0; intCtr < intStructSize - 1; intCtr++) {
		for(intCtr2 = 0; intCtr2 < intStructSize - 1 - intCtr; intCtr2++) {
			if(play[intCtr2].intHScore < play[intCtr2 + 1].intHScore) {
				
				temp[intCtr2].intTscore = play[intCtr2 + 1].intHScore;
				strcpy(temp[intCtr2].strTname, play[intCtr2 + 1].strName);
				intTemp2 = play[intCtr2 + 1].intRound;
				
				play[intCtr2 + 1].intHScore = play[intCtr2].intHScore;
				strcpy(play[intCtr2 + 1].strName, play[intCtr2].strName);
				play[intCtr2 + 1].intRound = play[intCtr2].intRound;
				
				play[intCtr2].intHScore = temp[intCtr2].intTscore;
				strcpy(play[intCtr2].strName, temp[intCtr2].strTname);
				play[intCtr2].intRound = intTemp2;
			}
		}
	}
	SlotScores = fopen("SlotScores.txt", "w");
	
	for(intCtr = 0; intCtr < intStructSize; intCtr++) {
		fprintf(SlotScores,"%i\t%s\t%i\t%i\n", intCtr + 1, play[intCtr].strName, play[intCtr].intRound, play[intCtr].intHScore);
	}
	
	fclose(SlotScores);
} // End of void fnHighScore()

/*============================================================================*
     *  Function   : fnShowScores
     *  Params     : None
     *  Returns    : Void
     *  Description: Displays the high scores ranking of all game sessions.
*=============================================================================*/
void fnShowScores() {
		system("cls");
		intCtr = 0;
		char strTemp3[10];
		
		fnCoorXY(33, 1);  printf("============================");
		fnCoorXY(33, 2); printf(" Casino Royale: HIGH SCORES");
		fnCoorXY(33, 3);  printf("============================");
		SlotScores = fopen("SlotScores.txt", "r");
		
		fnCoorXY(23, 7); printf(" RANK             NAME              ROUND        SCORE");
		fnCoorXY(23, 8); printf("------          ------            -------      -------");
			
		while(fscanf(SlotScores, "%s %s %s %s", strTemp, strTemp1, strTemp2, strTemp3) != EOF) {
			intCtr+=2;
			fnCoorXY(24, 9 + intCtr); printf("[%s]", strTemp);
			fnCoorXY(41, 9 + intCtr); printf("%s", strTemp1);
			fnCoorXY(59, 9 + intCtr); printf("%s", strTemp2);
			fnCoorXY(72, 9 + intCtr); printf("%s", strTemp3);
		}
		
		fclose(SlotScores);
		getch();
		fnGameMenu();
} // End of void fnShowScores()
