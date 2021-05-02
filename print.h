#include <windows.h>

// =======================================================================
// FUNCTIONS FOR PRINTING
// =======================================================================

void setColor(int n){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, n);
}

void resetColor(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
}
void printStar(int n){ // used to be a star, not anymore but i kept the name para less hassle
	setColor(4);
	int i;
	int c = 177;
	for(i=0; i<n; i++)
		printf("%c%c%c%c%c%c", c, c, c, c, c, c);
	resetColor();
	printf("\n");
}

void printDivider(int n){ // prints one block to horizontally separate table cells
	setColor(4);
	char c = 219;
	printf("%c ", c);
	resetColor();
}

void printVDivider(int n){ // prints alternate vertical divider
	setColor(4);
	int i;
	for(i=0; i<n; i++)
		printf("------");
	resetColor();
	printf("\n");
}


