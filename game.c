#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *logfile;
int n;
int size;
char **board;

void initialization();
void displayboard();
void logmove(char symbol, int row, int col);
void playermove(char symbol);
void computermove(char symbol);
void playgame(int mode);
int validmove(int row, int col);
int checkwin(char symbol);
int checkdraw();

int main() {
    srand(time(NULL));

    logfile = fopen("game_log.txt", "w");
    if (!logfile) {
        printf("Error opening log file.\n");
        return 1;
    }

    initialization();

      board = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        board[i] = malloc(n * sizeof(char));
    }

    displayboard();

    int mode;
    printf("Choose the game mode:\n");
    printf("1. User vs User\n");
    printf("2. User vs Computer\n");
    printf("3. Multiplayers\n");
    scanf("%d", &mode);
 
    playgame(mode);

    fclose(logfile);

    return 0;
}

void initialization() {
        printf("Welcome to THe TIC TAC TOE GAME!\n");
	printf("================================\n");
	printf("Enter board size N(3 <= N <= 10): ");
	scanf("%d", &n);
}

void displayboard() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            board[i][j] = ' ';

    printf("\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf(" %c ", board[i][j]);
            if (j < n - 1) printf("|");
        }
        printf("\n");
        if (i < n - 1) {
            for (int j = 0; j < n; j++) {
                printf("---");
                if (j < n - 1) printf("+");
            }
            printf("\n");
        }
    }
    printf("\n");
}

int validmove(int row, int col) {
    return row >= 0 && row < n && col >= 0 && col < n && board[row][col] == ' ';
}

void logmove(char symbol, int row, int col) {
    fprintf(logfile, "Player %c moved to (%d, %d)\n", symbol, row + 1, col + 1);
}


int checkwin(char symbol) {
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


int checkdraw() {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}


void playermove(char symbol) {
    int row, col;
    do {
        printf("Player %c, enter row and column (0-%d): ", symbol, size - 1);
        scanf("%d %d", &row, &col);
    } while (!validmove(row, col));
    board[row][col] = symbol;
    logmove(symbol, row, col);
}


void computermove(char symbol) {
    int row, col;
    do {
        row = rand() % size;
        col = rand() % size;
    } while (!validmove(row, col));
    printf("Computer %c plays at (%d, %d)\n", symbol, row, col);
    board[row][col] = symbol;
    logmove(symbol, row, col);
}


void playgame(int mode) {
    char player1 = 'X';
    char player2 = 'O';
    char player3 = 'Z';
    int turn = 0;

    while (1) {
        displayboard();
        char current;

        if (mode == 3) {
            if (turn % 3 == 0) {
                current = player1;
                playermove(current);
            } else if (turn % 3 == 1) {
                current = player2;
                computermove(current);
            } else {
                current = player3;
                computermove(current);
            }

            if (checkwin(current)) {
                displayboard();
                if (current == player1)
                    printf("Human player %c wins!\n", current);
                else
                    printf("Computer player %c wins!\n", current);
                fprintf(logfile, "Player %c wins!\n", current);
                break;
            }
        } else if (mode == 1) {
            current = (turn % 2 == 0) ? player1 : player2;
            playermove(current);
            if (checkwin(current)) {
                displayboard();
                printf("Player %c wins!\n", current);
                fprintf(logfile, "Player %c wins!\n", current);
                break;
            }
        } else if (mode == 2) {
            if (turn % 2 == 0) {
                playermove(player1);
                if (checkwin(player1)) {
                    displayboard();
                    printf("You win!\n");
                    fprintf(logfile, "Player %c wins!\n", player1);
                    break;
                }
            } else {
                computermove(player2);
                if (checkwin(player2)) {
                    displayboard();
                    printf("Computer wins!\n");
                    fprintf(logfile, "Player %c wins!\n", player2);
                    break;
                }
            }
        }

        if (checkdraw()) {
            displayboard();
            printf("It's a draw!\n");
            fprintf(logfile, "Game ended in a draw.\n");
            break;
        }

        turn++;
    }
}


