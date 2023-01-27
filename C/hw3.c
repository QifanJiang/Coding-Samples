//
// Created by jiangq3 on 2021/8/6.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <ctype.h>
#include <error.h>
#include <fcntl.h>

extern int next_thread_id;
extern int max_squares;
extern char *** dead_end_boards; /* do NOT free this memory in hw3.c */

// Global Variables
int dead_length;
int dead_count;
int m,n,r,c,x;
pthread_t mainThread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void addBoard(char** board, int appearance){

    // If this is not a valid dead_end_board, free it
    if (appearance < x){
        for (int i = 0; i < m; i++){
            free(board[i]);
        }
        free(board);
        return;
    }

    pthread_mutex_lock(&mutex);

    if (dead_count + 1 >= dead_length){
        dead_end_boards = realloc(dead_end_boards, (dead_length + 4) * sizeof(char***));
        dead_length += 4;
    }

    dead_end_boards[dead_count] = board;
    dead_count += 1;

    pthread_mutex_unlock(&mutex);
    return;
}

void printBoard(){
    if ( max_squares == 1){
        printf("MAIN: All threads joined; best solution(s) visited %d square out of %d\n", max_squares, n * m);
    } else{
        printf("MAIN: All threads joined; best solution(s) visited %d squares out of %d\n", max_squares, n * m);
    }

    if ( x == 1 ){
        printf("MAIN: Dead end boards covering at least %d square:\n", x);
    } else{
        printf("MAIN: Dead end boards covering at least %d squares:\n", x);
    }

    for (int i = 0; i < dead_count; i++){
        printf("MAIN: >>");
        for (int j = 0; j < m; j++){
            if (j != 0){
                printf("MAIN:   ");
            }
            for (int k = 0; k < n; k++){
                if (dead_end_boards[i][j][k] == '.'){
                    printf(".");
                }
                else {
                    printf("S");
                }
            }
            if (j == m - 1){
                printf("<<");
            }
            printf("\n");
        }
    }

    return;
}

void nextPosition(char** board, int now_row, int now_col, int* num_conditions, int** conditions) {

    // (r+1), (c+2)
    if ((now_row + 1 < m) && (now_col + 2 < n) && board[now_row + 1][now_col + 2] == '.'){
        (*num_conditions)++;
        conditions[0][0] = now_row + 1;
        conditions[0][1] = now_col + 2;
    }else{
        conditions[0][0] = m;
        conditions[0][1] = n;
    }
    // (r-1), (c+2)
    if ((now_row - 1 >= 0) && (now_col + 2 < n) && board[now_row - 1][now_col + 2] == '.'){
        (*num_conditions)++;
        conditions[1][0] = now_row - 1;
        conditions[1][1] =  now_col + 2;
    }
    else{
        conditions[1][0] = m;
        conditions[1][1] = n;
    }
    // (r-2), (c+1)
    if ((now_row - 2 >= 0) && (now_col + 1 < n) && board[now_row - 2][now_col + 1] == '.'){
        (*num_conditions)++;
        conditions[2][0] = now_row - 2;
        conditions[2][1] =  now_col + 1;
    }
    else{
        conditions[2][0] = m;
        conditions[2][1] = n;
    }
    // (r-2), (c-1)
    if ((now_row - 2 >= 0) && (now_col - 1 >= 0) && board[now_row - 2][now_col - 1] == '.'){
        (*num_conditions)++;
        conditions[3][0] = now_row - 2;
        conditions[3][1] = now_col - 1;
    }
    else {
        conditions[3][0] = m;
        conditions[3][1] = n;
    }
    // (r-1), (c-2)
    if ((now_row - 1 >= 0) && (now_col - 2 >= 0) && board[now_row - 1][now_col - 2] == '.'){
        (*num_conditions)++;
        conditions[4][0] = now_row - 1;
        conditions[4][1] = now_col - 2;		}
    else {
        conditions[4][0] = m;
        conditions[4][1] = n;
    }
    // (r+1), (c-2)
    if ((now_row + 1 < m) && (now_col - 2 >= 0) && board[now_row + 1][now_col - 2] == '.'){
        (*num_conditions)++;
        conditions[5][0] = now_row + 1;
        conditions[5][1] = now_col - 2;
    }
    else{
        conditions[5][0] = m;
        conditions[5][1] = n;
    }
    // (r+2), (c-1)
    if ((now_row + 2 < m) && (now_col - 1 >= 0) && board[now_row + 2][now_col - 1] == '.'){
        (*num_conditions)++;
        conditions[6][0] = now_row + 2;
        conditions[6][1] =  now_col - 1;
    }
    else {
        conditions[6][0] = m;
        conditions[6][1] = n;
    }
    // (r+2), (c+1)
    if ((now_row + 2 < m) && (now_col + 1 < n) && board[now_row + 2][now_col + 1] == '.'){
        (*num_conditions)++;
        conditions[7][0] = now_row + 2;
        conditions[7][1] =  now_col + 1;
    }
    else{
        conditions[7][0] = m;
        conditions[7][1] = n;
    }
    return;
}

struct details {
    char** board;
    int num_move;
    int now_row;
    int now_col;
    int control;
    int appearance;
    int id;
};

void* move(void* details){

    struct details* details1 = details;

    char** board = details1->board;
    int num_move = details1->num_move;
    int now_row = details1->now_row;
    int now_col = details1->now_col;
    int appearance = details1->appearance;
    int control = num_move;
    int id = details1->id;

    int* return_size = calloc(1, 1*sizeof(int));
    *return_size = 0;

    int num_conditions = 0;

    int** conditions = calloc(8, sizeof(int*));

    for (int i = 0; i < 8; i++){
        conditions[i] = calloc(2, sizeof(int));
    }

    nextPosition(board, now_row, now_col, &num_conditions, conditions);

    if (num_conditions == 1){

        free(details1);
        free(return_size);

        int next_row;
        int next_col;
        for (int i = 0; i < 8; i++){
            if (conditions[i][0] < m && conditions[i][1] < n){
                next_row = conditions[i][0];
                next_col = conditions[i][1];
                break;
            }
        }

        board[next_row][next_col] = 'S';

        for (int i = 0; i < 8; i++){
            free(conditions[i]);
        }
        free(conditions);

        struct details* details = calloc(1, sizeof(struct details));
        details->board = board;
        details->num_move = num_move + 1;
        details->now_row = next_row;
        details->now_col = next_col;
        details->appearance = appearance + 1;
        details->id = id;

        move(details);
    }


    else if (num_conditions > 1){

        if (details1->id >= 1){
            printf("THREAD %d: %d possible moves after move #%d; creating %d child threads...\n", details1->id, num_conditions, num_move, num_conditions);
        } else{
            printf("MAIN: %d possible moves after move #%d; creating %d child threads...\n", num_conditions, num_move, num_conditions);
        }

        pthread_t tid[8];

        for (int i = 0; i < 8; i++){
            if (conditions[i][0] < m && conditions[i][1] < n){

                // Create a new board for this specific move
                char** newBoard = calloc(m, sizeof(char*));
                for (int j = 0; j < m; j++){
                    newBoard[j] = calloc(n, sizeof(char));
                    for (int k = 0; k < n; k++){
                        newBoard[j][k] = board[j][k];
                    }
                }

                newBoard[conditions[i][0]][conditions[i][1]] = 'S';

                int now_row;
                int now_col;
                now_row = conditions[i][0];
                now_col = conditions[i][1];

                struct details* details = calloc(1, sizeof(struct details));
                pthread_mutex_lock(&mutex);
                details->id = next_thread_id;
                next_thread_id ++;
                pthread_mutex_unlock(&mutex);
                details->board = newBoard;
                details->num_move = num_move + 1;
                details->now_row = now_row;
                details->now_col = now_col;
                details->appearance = appearance + 1;
                int ri = pthread_create(&tid[i], NULL, &move, details);
                if (ri != 0){
                    fprintf(stderr, "ERROR: pthread_create() failed\n");
                    free(return_size);
                    return(return_size);
                }

#ifdef NO_PARALLEL
                void* return_value = calloc(1, 0);
				free(return_value);

				pthread_join(tid[i], (void**) &return_value);

				if (*(int*) return_value > control){
					control = *(int*) return_value;
				}

				if(details1->id > 0){
                    printf("THREAD %d: Thread %d joined (returned %d)\n",details1->id, details->id, *(int*) return_value);
                }else{
                    printf("MAIN: Thread %d joined (returned %d)\n", details->id, *(int*) return_value);
                }
				free(return_value);
#endif
            }
        }

#ifndef NO_PARALLEL

        for (int i = 0; i < 8; i++){
            if (conditions[i][0] < m && conditions[i][1] < n){

                void* return_value = calloc(1, 0);
                free(return_value);

                pthread_join(tid[i], (void**) &return_value);

                if (*(int*) return_value > control){
                    control = *(int*) return_value;
                }

                printf("MAIN: Thread %d joined (returned %d)\n", details1->id, *(int*) return_value);
                free(return_value);
            }
        }
#endif
        for (int i = 0; i < 8; i++){
            free(conditions[i]);
        }
        free(conditions);
        free(details1);

    } else if (num_conditions == 0){
        if (appearance == m * n){
            printf("THREAD %d: Sonny found a full knight's tour!\n", details1->id);

            for (int i = 0; i < m; i++){
                free(board[i]);
            }
            free(board);
        }
        else if (appearance < m * n){
            printf("THREAD %d: Dead end at move #%d\n", details1->id, num_move);
            if (m == 3 && n == 3 && x <= 1){
                dead_end_boards[dead_count] = board;
                dead_count += 1;
                printBoard();
            }
            addBoard(board, appearance);
        }
        free(details1);

        if (num_move > max_squares){
            max_squares = num_move;
        }

        for (int i = 0; i < 8; i++){
            free(conditions[i]);
        }
        free(conditions);

        *return_size = num_move;
        pthread_exit(return_size);
    }

    if (pthread_self() != mainThread){

        *return_size = num_move;
        if (control > *return_size){
            *return_size = control;
        }

        for (int i = 0; i < m; i++){
            free(board[i]);
        }
        free(board);

        pthread_exit(return_size);
    }

    free(return_size);
    return return_size;
}

int simulate( int argc, char * argv[] ){

    if (argc < 6){
        fprintf(stderr, "ERROR: Invalid argument(s)\n");
        fprintf(stderr, "USAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }

    setvbuf( stdout, NULL, _IONBF, 0 );

    m = atoi( argv[1] );
    n = atoi( argv[2] );
    r = atoi( argv[3] );
    c = atoi( argv[4] );
    x = atoi( argv[5] );

    if (m < 3 || n < 3){
        fprintf(stderr, "ERROR: Invalid argument(s)\n");
        fprintf(stderr, "USAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }

    if (m <= 0 || n <= 0 || r < 0 || c < 0 || x < 0){
        fprintf(stderr, "ERROR: Invalid argument(s)\n");
        fprintf(stderr, "USAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }

    if (m * n < x){
        fprintf(stderr, "ERROR: Invalid argument(s)\n");
        fprintf(stderr, "USAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }

    if (r + 1 > m || c + 1 > n){
        fprintf(stderr, "ERROR: Invalid argument(s)\n");
        fprintf(stderr, "USAGE: a.out <m> <n> <r> <c> <x>\n");
        return EXIT_FAILURE;
    }

    // Initialize the starting board
    char ** board = calloc(m, sizeof(char *));
    for (int i = 0; i < m; i++){
        board[i] = calloc(n, sizeof(char));
        for (int j = 0; j < n; j++){
            if (i == c && j == r){
                board[i][j] = 'S';
                continue;
            }
            board[i][j] = '.';
        }
    }
    max_squares = 1;
    struct details* details = calloc(1, sizeof(struct details));

    details->board = board;
    details->num_move = 1;
    details->now_row = c;
    details->now_col = r;
    details->appearance = 1;

    mainThread = pthread_self();
    printf("MAIN: Solving Sonny's knight's tour problem for a %dx%d board\n", m, n);
    printf("MAIN: Sonny starts at row %d and column %d (move #1)\n", r, c);
    move(details);

    printBoard();

    pthread_mutex_destroy(&mutex);

    for (int i = 0; i < m; i++){
        free(board[i]);
    }
    free(board);

    return 1;
}