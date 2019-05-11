/**
 * CONNECT FOUR GAME
 * 
 * This program is a C implementation of the game Connect four
 * 
 * Date : 2019-05-06
 * Last update : 2019-05-12
 * Version : 1.0
 * Author : E. Zemmouri
 * 
 */ 


#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int num;        // Player number (1 or 2)
    char name[101]; // Player name
    char color;     // discs' color
    int discs;      // number of discs
} player;

typedef struct {
    char **grid; // a 2d-array of chars
    int n; // number of lines
    int m; // number of columns
} board;


void welcome();
board init_board (int, int);
void free_board (board);
void display_game (board, player, player);
int full_col(board b, int j);
int four_on_line(board b, int i, int j);
int four_on_col(board b, int i, int j);
int four_on_diag1(board b, int i, int j);
int four_on_diag2(board b, int i, int j);
int check_four (board b, int i, int j);


int play_turn(board b, player p);


void clear_screen();
void clear_buffer();

void welcome(){

    clear_screen();

    printf("\n===========================================================================\n");
    printf("===                    Welcome to Connect Four Game                     ===\n");
    printf("===========================================================================\n\n");

    printf("    Connect Four is a two-player connection game in which the players\n");
    printf("    first choose a color (X for red, O for yellow) and then take turns\n");
    printf("    dropping one colored disc from the top into a 7-column, 6-row \n");
    printf("    vertically suspended grid. The pieces fall straight down, occupying\n");
    printf("    the lowest available space within the column.\n");
    printf("    The objective of the game is to be the first to form a horizontal,\n");
    printf("    vertical, or diagonal line of four of one's own discs.\n");
    printf("    The player who first connect 4 of his own discs will be the winner.\n\n");
    printf("===========================================================================\n\n\n");
}


board init_board (int lines, int columns){
    board b;
    int i, j;

    b.n = lines;
    b.m = columns;
    b.grid = malloc(b.n*sizeof(char *));
    for (i=0; i<b.n; i++){
        b.grid[i] = malloc(b.m*sizeof(char));
        for (j = 0; j<b.m; j++)
            b.grid[i][j] = ' ';
    }
    return b;
}

void free_board (board b){
    int i;
    for (i=0; i<b.n; i++){
        free(b.grid[i]);
    }
    free(b.grid);
}



void display_game (board b, player p1, player p2){
    int i, j;
    clear_screen();

    printf("\n            Player -1-\t%s  (%c)\n", p1.name, p1.color);
    printf("                      \tYou still have %d discs \n", p1.discs);

    printf("            Player -2-\t%s  (%c)\n", p2.name, p2.color);
    printf("                      \tYou still have %d discs \n", p2.discs);

    printf("            Here is the BOARD : \n\n");
    printf("          |");
    for (j=1; j<=b.m; j++) printf("%2d  ", j);
    printf("\b|\n");
    printf("          |");
    for (j=0; j<b.m; j++) printf("----");
    printf("\b|\n");
    for (i=0; i<b.n; i++){
        printf("          |");
        for (j=0; j<b.m; j++){
            if (b.grid[i][j] == 'X') printf("\033[1;31m");
            if (b.grid[i][j] == 'O') printf("\033[01;33m");
            printf(" %c ", b.grid[i][j]);
            printf("\033[0m");
            printf("|");
            
        }
        printf("\n          |");
        for (j=0; j<b.m; j++) printf("----");
        printf("\b|\n");
    }
    printf("\n");
}

int full_col(board b, int j){
    if (b.grid[0][j] != ' ') return 1;
    else return 0;
}

int four_on_line(board b, int i, int j){
    int k, count=0;
    k = j;
    while (k < b.m && b.grid[i][k] == b.grid[i][j]){
        k++;
        count++;
    }
    k = j-1;
    while (k >= 0 && b.grid[i][k] == b.grid[i][j]){
        k--;
        count++;
    }
    
    return (count >= 4) ? 1 : 0;
}

int four_on_col(board b, int i, int j){
    int k, count=0;
    k = i;
    while (k < b.n && b.grid[k][j] == b.grid[i][j]){
        k++;
        count++;
    }
    k = i-1;
    while (k >= 0 && b.grid[k][j] == b.grid[i][j]){
        k--;
        count++;
    }
    
    return (count >= 4) ? 1 : 0;
}

int four_on_diag1(board b, int i, int j){
    int k, l, count=0;
    k = i, l=j;
    while (k < b.n && l< b.m && b.grid[k][l] == b.grid[i][j]){
        k++; l++;
        count++;
    }
    k = i-1; l = j-1;
    while (k >= 0 && l >= 0 && b.grid[k][l] == b.grid[i][j]){
        k--; l--;
        count++;
    }
    
    return (count >= 4) ? 1 : 0;
}

int four_on_diag2(board b, int i, int j){
    int k, l, count=0;
    k = i, l=j;
    while (k >= 0 && l< b.m && b.grid[k][l] == b.grid[i][j]){
        k--; l++;
        count++;
    }
    k = i+1; l = j-1;
    while (k < b.n && l >= 0 && b.grid[k][l] == b.grid[i][j]){
        k++; l--;
        count++;
    }
    
    return (count >= 4) ? 1 : 0;
}

int check_four (board b, int i, int j){
    return (four_on_line(b,i,j) || four_on_col(b,i,j) || four_on_diag1(b,i,j) || four_on_diag2(b,i,j));
}

int drop_disc (board b, int j, char c){
    int i = 0;
    while (i < b.n && b.grid[i][j] == ' '){
        i++;
    }
    i--;
    b.grid[i][j] = c;
    return i;   
}

int play_turn(board b, player p){
    int j, i, four;

    printf("            Player %d - %s, where to drop disc? (choose 1 to %d) : ", p.num, p.name, b.m);
    scanf("%d", &j);
    if (j<1 || j>b.m) {
        printf("Column %d is wrong, you missed this turn !\n", j);
        return 0;
    }
    if (full_col(b, j-1)) {
        printf("Column %d is full, you missed this turn !\n", j);
        return 0;
    }
    j = j-1;
    i = drop_disc(b, j, p.color);
    four = check_four(b, i, j);
    
    return four;
}


void clear_screen(){
    #ifdef WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void clear_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF );
}

int main(int argc, char const *argv[]){

    player p1 = {1, "A", 'X', 21};
    player p2 = {2, "B", 'O', 21};
    int n = 6, m = 7;
    char yes = 'y';
    int turn = 1;
    int win = 0;

    welcome();

    printf("Do you want to use default setting (y/n) ?");
    yes = getchar();
    clear_buffer();
    if (yes != 'y' && yes != 'Y'){
        printf("Give Palyer 1 name : ");
        gets(p1.name);
        printf("Choose palyer 1 color (X or O) : ");
        p1.color = getchar();
        clear_buffer();

        printf("\nGive Palyer 2 name : ");
        gets(p2.name);
        printf("Choose palyer 2 color (X or O) : ");
        p2.color = getchar();
        clear_buffer();

        printf("\nGive grid size (lines and columns) 6x7 is a good choice : ");
        scanf("%d%d", &n, &m);
        p1.discs = (n*m)/2;
        p2.discs = n*m - p1.discs;
    }
    board b = init_board(n, m);

    printf("\nChoose the player to be the first to play.\n");
    printf("1 for %s and 2 for %s : ", p1.name, p2.name);
    scanf("%d", &turn);

    display_game(b, p1, p2);

    win = 0;
    while (win == 0 && (p1.discs != 0 || p2.discs !=0)){
        if (turn == 1){
            win = play_turn(b, p1);
            p1.discs --;
            display_game(b, p1, p2);
            if(win) printf("            Congratulations %s ! You win.\n", p1.name);
        }
        else{
            win = play_turn(b, p2);
            p2.discs --;
            display_game(b, p1, p2);
            if(win) printf("            Congratulations %s ! You win.\n", p2.name);
        }
        turn = (turn%2) + 1;
    }

    printf("            Game Over ...\n\n");

    free_board(b);
    
    return 0;
}
