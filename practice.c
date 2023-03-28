// BY: LILIAN ULLOA
// CS 211
// DATE: MARCH 10, 2023
// PROGRAM2: WEAVER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// GLOBAL VARIABLES
char filename[] = "words.txt";
char **info = NULL;
char word1[81];
char word2[81];
char play[81];
char inputLine[81];

// READFILE OPENS THE FILE AND CREATES A LIST BASED ON THE USER SELECTED SIZE 
int readFile(int size) {
	FILE *filePtr = fopen(filename, "r"); // "r" means we open the file for reading
	if(filePtr == NULL) {
		printf("Error: could not open %s for reading\n", filename);
		exit(-1);	
	}
	char line[81];
	int llen = 0;
	int counter = 0;

	while(fscanf(filePtr, "%s", line) != EOF) {
		if(strlen(line) == size) {
			info = realloc(info, (counter + 1) * sizeof(char *));
			llen = strlen(line);
			//printf("Size of line: %d and line: %s\n", llen, line); 
			info[counter] = malloc(llen * sizeof(char));
			strcpy(info[counter], line);
			printf("Info: %s\n", info[counter]);
			counter++;
		}
	}
	fclose(filePtr);
	return counter;
}

// CHECKEXACT TAKES IN THE SIZE USER SELECTED AND ITERATED THROUGH PREVIOUS WORD AND CURRENT WORD
// CHECK++ IS EXECUTED ONLY WHEN LETTER AT INDEX IS NOT EQUAL 
// CHECKS IF CHECK IS EQUAL TO 1 AND RETURNS VALUE BASED ON THIS 
int checkExact(int size, char *p) {
	int check = 0;
	for(int i = 0; i < size; i++) {
		if(p[i] == word1[i]) {
			continue;
		}
		check++;
	}
	
	if(check != 1) {
		printf("Your word, '%s', is not exactly 1 character different. Try again.\n", p);
		return 0;
	}

	return 1;
}

// CHECKSIZE TAKES IN THE SIZE USER SELECTED AND CHECKS LENGTH BY ITERATING THROUGH POINTER
int checkSize(int max, char *p) {
	int counter = 0;	
	char* x = p;

	while(*x != '\0') {
		x++;
		counter++;
	}
	
	if(counter != max) {
		printf("Your word, '%s', is not a %d-letter word. Try again.\n", p, max);
		return 0;
	}
	
	return 1;
}

// CHECKDICTIONARY TAKES IN THE TOTAL # OF WORDS IN FILE OF USER SELECTED SIZE AND CHECKS IF WORD IS FOUND IN FILE
// BY ITERATING THROUGH THE LIST CREATED IN READFILE
int checkDictionary(int count, char *p) {
	
	for(int i = 0; i < count; i++) {
		if(strcmp(info[i], p) == 0) {
			return 1;
		}
	}
	printf("Your word, '%s', is not a valid dictionary word. Try again.\n", p);

	return 0;
}

int main() {	
	srand(1);
	int user = 0;
	int size = 0;
	int num = 0;

	while(user != 3) {
		if(user == 0) {
			printf("Weaver is a game where you try to find a way to get from the starting word to the ending word.\n");
			printf("You can change only one letter at a time, and each word along the way must be a valid word.\n");
			printf("Enjoy!\n\n");
		}
		if(user == 2 || user == 0) {
			printf("How many letters do you want to have in the words? ");
			scanf("%d%*c", &size);
			num = readFile(size);
			printf("Number of %d-letter words found: %d.\n\n", size, num); // print out how many words in file of that size
		}
		// variables for input, clear variables before input
		int valid = 0;
		inputLine[0] = '\0';
		word1[0] = '\0';
		word2[0] = '\0';
	
		// WHILE USER INPUT IT NOT VALID, CONTINUE LOOP
		while(valid != 1) {
			printf("Enter starting and ending words, or 'r' for either for a random word: ");
			fgets(inputLine, 81, stdin);
			inputLine[strlen(inputLine) - 1] = '\0';	
			sscanf(inputLine, "%s%s", word1, word2);
			
			if(*word1 == 'r' && (strlen(word1) == 1)) {
				strcpy(word1, info[rand() % num]);
			}
			if(*word2 == 'r' && (strlen(word2) == 1)) {
				strcpy(word2, info[rand() % num]);
			}

	
			// CHECK IF WORDS ARE VALID
			if(checkSize(size, word1) && checkDictionary(num, word1)) {
				if(checkSize(size, word2) && checkDictionary(num, word2)) {
					valid = 1;
				}
			}
		}	

		// AFTER INPUT IS SUCCESSFUL, PRINT STARTING AND ENDING WORDS
		printf("Your starting word is: %s.\n", word1);
		printf("Your ending word is: %s.\n", word2);
		printf("\nOn each move enter a word of the same length that is at most 1 character different and is also in the dictionary.\n");
	        printf("You may also type in 'q' to quit guessing.\n");
	
		// VARIABLES FOR PLAYING GAME
		int won = 0;
		int move = 1;
	
		// STORES INITIAL WORD 
		char start[81];
		start[0] = '\0';
		play[0] = '\0';
		strcpy(start, word1);
		
		// WHILE USER IS STILL GUESSING...
		while(won != 1) {
			// PROMPT USER TO ENTER INPUT AND REPLACE \N WITH NULL CHARACTER 
			printf("\n%d. Previous word is '%s'. Goal word is '%s'. Next word: ", move, word1, word2);
			fgets(play, 81, stdin);
			play[strlen(play) - 1] = '\0';

			// IF USER ENTERS 'q' STOP PRINTING 
			if(*play == 'q') {
				break;
			}
		
			// CHECK IF INPUT IS VALID
			if(checkSize(size, play) && checkDictionary(num, play) && checkExact(size, play)) {
				strcpy(word1, play);
				if(strcmp(word1, word2) == 0) {
					won = 1;
					break;
				}
				move++;	// UPDATES MOVE ONLY IF WORD IS VALID
			}	
		}
	
		if(won == 1) {
			printf("Congratulations! You changed '%s' into '%s' in %d moves.\n", start, word2, move);
		}
		
		printf("\nEnter: \t1 to play again,\n");
		printf("\t2 to change the number of letters in the words and then play again, or\n");
		printf("\t3 to exit the program.\n");
		printf("Your choice --> ");
		scanf("%d%*c", &user);
	
		if(user == 2 || user == 3) {
			for(int i = 0; i < num - 1; i++) {
				free(info[i]);
			}
		}
	}
	
	if(user == 3) {
		printf("\nThanks for playing!\n");
		printf("Exiting...\n");
	}
	
	free(info);

	return 0;
	
}

