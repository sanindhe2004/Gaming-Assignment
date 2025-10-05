#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 10


char* createBoard(int size);
void initializeBoard(char *board, int size);
void displayBoard(char *board, int size);
int isValidMove(char *board, int size, int row, int col);
int checkWin(char *board, int size, char symbol);
int checkDraw(char *board, int size);
void playerMove(char *board, int size, char symbol);
void computerMove(char *board, int size, char computerSymbol, char userSymbol);
void logBoardToFile(char *board, int size, FILE *fp);
void freeBoard(char *board);

int main() {
    int size, choice;
    char *board;
    FILE *fp;

    srand(time(NULL)); 
    
    printf("Welcome to the TIC TAC TOC GAME!\n");

    printf("Enter board size N (3 <= N <= 10): ");
    scanf("%d", &size);

    if (size < 3 || size > MAX) {
        printf("Invalid board size.\n");
        return 0;
    }

    board = createBoard(size);
    fp = fopen("game_log.txt", "a");
    if (!fp) {
        printf("Error opening log file.\n");
        return 1;
    }

    printf("\nSelect Game Mode:\n");
    printf("1. Two Players (User vs User)\n");
    printf("2. User vs Computer\n");
    printf("3. Three Players\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &choice);

    initializeBoard(board, size);
    displayBoard(board, size);

    int turn = 0, totalPlayers = (choice == 3) ? 3 : 2;
    char symbols[3] = {'X', 'O', 'Z'};
    int gameOver = 0;

    while (!gameOver) {
        int current = turn % totalPlayers;
        printf("\nPlayer %c's turn.\n", symbols[current]);

        if (choice == 2 && symbols[current] == 'O') {
            computerMove(board, size, 'O', 'X');
        } else {
            playerMove(board, size, symbols[current]);
        }

        displayBoard(board, size);
        logBoardToFile(board, size, fp);

        if (checkWin(board, size, symbols[current])) {
            printf("Player %c wins!\n", symbols[current]);
            fprintf(fp, "Player %c wins!\n\n", symbols[current]);
            gameOver = 1;
        } else if (checkDraw(board, size)) {
            printf("It's a draw!\n");
            fprintf(fp, "It's a draw!\n\n");
            gameOver = 1;
        } else {
            turn++;
        }
    }

    fclose(fp);
    freeBoard(board);
    return 0;
}

char* createBoard(int size) {
    return (char*)malloc(size * size * sizeof(char));
}


void initializeBoard(char *board, int size) {
    for (int i = 0; i < size * size; i++)
        board[i] = ' ';
}


void displayBoard(char *board, int size) {
    printf("\n  ");
    for (int i = 0; i < size; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("  ");
        for (int j = 0; j < size; j++) printf("--");
        printf("-\n%d ", i);
        for (int j = 0; j < size; j++)
            printf("|%c", board[i * size + j]);
        printf("|\n");
    }
    printf("  ");
    for (int j = 0; j < size; j++) printf("--");
    printf("-\n");
}


int isValidMove(char *board, int size, int row, int col) {
    return (row >= 0 && row < size && col >= 0 && col < size && board[row * size + col] == ' ');
}


int checkWin(char *board, int size, char symbol) {
    for (int i = 0; i < size; i++) {
        int row = 1, col = 1;
        for (int j = 0; j < size; j++) {
            if (board[i * size + j] != symbol) row = 0;
            if (board[j * size + i] != symbol) col = 0;
        }
        if (row || col) return 1;
    }
    int diag1 = 1, diag2 = 1;
    for (int i = 0; i < size; i++) {
        if (board[i * size + i] != symbol) diag1 = 0;
        if (board[i * size + (size - 1 - i)] != symbol) diag2 = 0;
    }
    return diag1 || diag2;
}


int checkDraw(char *board, int size) {
    for (int i = 0; i < size * size; i++)
        if (board[i] == ' ') return 0;
    return 1;
}


void playerMove(char *board, int size, char symbol) {
    int row, col;
    while (1) {
        printf("Enter row and col (0..%d): ", size - 1);
        scanf("%d %d", &row, &col);
        if (isValidMove(board, size, row, col)) {
            board[row * size + col] = symbol;
            break;
        } else {
            printf("Invalid move. Try again.\n");
        }
    }
}


void computerMove(char *board, int size, char computerSymbol, char userSymbol) {
    int bestRow = -1, bestCol = -1;

   
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (isValidMove(board, size, i, j)) {
                board[i * size + j] = computerSymbol;
                if (checkWin(board, size, computerSymbol)) {
                    return;
                }
                board[i * size + j] = ' ';
            }
        }
    }

    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (isValidMove(board, size, i, j)) {
                board[i * size + j] = userSymbol;
                if (checkWin(board, size, userSymbol)) {
                    board[i * size + j] = computerSymbol;
                    return;
                }
                board[i * size + j] = ' ';
            }
        }
    }

    
    do {
        bestRow = rand() % size;
        bestCol = rand() % size;
    } while (!isValidMove(board, size, bestRow, bestCol));

    board[bestRow * size + bestCol] = computerSymbol;
    printf("Computer placed at (%d, %d)\n", bestRow, bestCol);
}


void logBoardToFile(char *board, int size, FILE *fp) {
    fprintf(fp, "Current Board:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            fprintf(fp, "%c ", board[i * size + j]);
        fprintf(fp, "\n");
    }
    fprintf(fp, "---------------------\n");
}


void freeBoard(char *board) {
    free(board);
}

