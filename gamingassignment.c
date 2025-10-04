#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10

char board[MAX_SIZE][MAX_SIZE];
int size;
FILE *logFile; // Log file pointer

// Initialize the board
void initBoard(int n) {
    size = n;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = ' ';
}

// Display the board
void displayBoard() {
    printf("\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf(" %c ", board[i][j]);
            if (j < size - 1) printf("|");
        }
        printf("\n");
        if (i < size - 1) {
            for (int j = 0; j < size; j++) {
                printf("---");
                if (j < size - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

// Check if move is valid
int isValidMove(int row, int col) {
    return row >= 0 && row < size && col >= 0 && col < size && board[row][col] == ' ';
}

// Log move
void logMove(char symbol, int row, int col) {
    fprintf(logFile, "Player %c moved to (%d, %d)\n", symbol, row, col);
}

// Check for win condition
int checkWin(char symbol) {
    for (int i = 0; i < size; i++) {
        int rowCount = 0, colCount = 0;
        for (int j = 0; j < size; j++) {
            if (board[i][j] == symbol) rowCount++;
            if (board[j][i] == symbol) colCount++;
        }
        if (rowCount == size || colCount == size) return 1;
    }

    int diag1 = 0, diag2 = 0;
    for (int i = 0; i < size; i++) {
        if (board[i][i] == symbol) diag1++;
        if (board[i][size - i - 1] == symbol) diag2++;
    }
    return diag1 == size || diag2 == size;
}

// Check for draw
int isDraw() {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

// Player move
void playerMove(char symbol) {
    int row, col;
    do {
        printf("Player %c, enter row and column (0-%d): ", symbol, size - 1);
        scanf("%d %d", &row, &col);
    } while (!isValidMove(row, col));
    board[row][col] = symbol;
    logMove(symbol, row, col);
}

// Computer move
void computerMove(char symbol) {
    int row, col;
    do {
        row = rand() % size;
        col = rand() % size;
    } while (!isValidMove(row, col));
    printf("Computer %c plays at (%d, %d)\n", symbol, row, col);
    board[row][col] = symbol;
    logMove(symbol, row, col);
}

// Main game loop
void playGame(int mode) {
    char player1 = 'X';
    char player2 = 'O';
    char player3 = 'Z';
    int turn = 0;

    while (1) {
        displayBoard();
        char current;

        if (mode == 3) {
            if (turn % 3 == 0) {
                current = player1;
                playerMove(current);
            } else if (turn % 3 == 1) {
                current = player2;
                computerMove(current);
            } else {
                current = player3;
                computerMove(current);
            }

            if (checkWin(current)) {
                displayBoard();
                if (current == player1)
                    printf("Human player %c wins!\n", current);
                else
                    printf("Computer player %c wins!\n", current);
                fprintf(logFile, "Player %c wins!\n", current);
                break;
            }
        } else if (mode == 1) {
            current = (turn % 2 == 0) ? player1 : player2;
            playerMove(current);
            if (checkWin(current)) {
                displayBoard();
                printf("Player %c wins!\n", current);
                fprintf(logFile, "Player %c wins!\n", current);
                break;
            }
        } else if (mode == 2) {
            if (turn % 2 == 0) {
                playerMove(player1);
                if (checkWin(player1)) {
                    displayBoard();
                    printf("You win!\n");
                    fprintf(logFile, "Player %c wins!\n", player1);
                    break;
                }
            } else {
                computerMove(player2);
                if (checkWin(player2)) {
                    displayBoard();
                    printf("Computer wins!\n");
                    fprintf(logFile, "Player %c wins!\n", player2);
                    break;
                }
            }
        }

        if (isDraw()) {
            displayBoard();
            printf("It's a draw!\n");
            fprintf(logFile, "Game ended in a draw.\n");
            break;
        }

        turn++;
    }
}

// Entry point
int main() {
    srand(time(NULL));
    int n, mode;

    printf("Welcome to TicTacToe!\n");
    do {
        printf("Enter board size (3 to 10): ");
        scanf("%d", &n);
    } while (n < 3 || n > 10);

    initBoard(n);

    printf("Choose Game Mode:\n");
    printf("1. User vs User\n");
    printf("2. User vs Computer\n");
    printf("3. Multiplayer (1 human + 2 computers)\n");
    scanf("%d", &mode);

    logFile = fopen("game_log.txt", "w");
    if (!logFile) {
        printf("Error opening log file.\n");
        return 1;
    }

    playGame(mode);
    fclose(logFile);

    return 0;
}


