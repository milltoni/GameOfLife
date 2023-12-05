//    Conway's Game of Life
// q - quit game;
// u - speed boster
// d - speed reducer

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define input_file "testboard.txt"
#define M 25
#define N 80

int input_start_position(int **buffer);
void print_graphic(int **buffer);
int neighbors_counter(int **buffer, int i, int j);
void create_new_gen(int **past, int **new);
void key_reader();
int key_kontrol(char const *key, int *tick);

int main() {
    char key;
    fd_set rfds;
    struct timeval tv;
    int tickrate = 100000;

    int **past_gen = calloc(M, sizeof(int *));
    int **next_gen = calloc(M, sizeof(int *));
    for (int i = 0; i < M; ++i) {
        past_gen[i] = calloc(N, sizeof(int));
        next_gen[i] = calloc(N, sizeof(int));
    }

    if (input_start_position(past_gen) == 1) {
        printf("No such file or directory!");
    } else {
        int flag = 0;
        key_reader();
        while (flag != 1) {
            FD_ZERO(&rfds);
            FD_SET(0, &rfds);
            tv.tv_sec = 0;
            tv.tv_usec = 0;
            int retval = select(2, &rfds, NULL, NULL, &tv);
            system("clear");
            print_graphic(past_gen);
            create_new_gen(past_gen, next_gen);
            int **gen = past_gen;
            past_gen = next_gen;
            next_gen = gen;
            if (retval) {
                key = getc(stdin);
            } else {
                key = ' ';
            }
            flag = key_kontrol(&key, &tickrate);
            usleep(tickrate);
        }
    }

    for (int i = 0; i < M; ++i) {
        free(past_gen[i]);
        free(next_gen[i]);
    }
    free(past_gen);
    free(next_gen);
}

int input_start_position(int **buffer) {
    int flag = 0;
    FILE *inp = fopen(input_file, "r");
    if (inp == NULL) {
        flag = 1;
    } else {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N + 1; ++j) {
                int num = (int)(fgetc(inp) - '0');
                if (num == 1 || num == 0) {
                    buffer[i][j] = num;
                }
            }
        }
        fclose(inp);
    }
    return flag;
}

void print_graphic(int **buffer) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (buffer[i][j] == 1) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int neighbors_counter(int **buffer, int i, int j) {
    int sum = 0;
    for (int k = -1; k < 2; ++k) {
        for (int l = -1; l < 2; ++l) {
            sum += buffer[(i + k + M) % M][(j + l + N) % N];
        }
    }
    return sum - buffer[i][j];
}

void create_new_gen(int **past, int **new) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            int neighbors = neighbors_counter(past, i, j);
            if (past[i][j] == 1 && (neighbors < 2 || neighbors > 3)) {
                new[i][j] = 0;
            } else if (past[i][j] == 0 && neighbors == 3) {
                new[i][j] = 1;
            } else {
                new[i][j] = past[i][j];
            }
        }
    }
}

void key_reader() {
    static struct termios oldt, newt;
    tcgetattr(0, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    newt.c_lflag &= (~ECHO);
    newt.c_cc[VTIME] = 0;
    newt.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &newt);
}

int key_kontrol(char const *key, int *tick) {
    int flag = 0;
    if (*key == 'q' || *key == 'Q') {
        flag = 1;
    } else if ((*key == 'u' || *key == 'U') && *tick > 20000) {
        *tick -= 20000;
    } else if ((*key == 'd' || *key == 'D') && *tick < 1500000) {
        *tick += 20000;
    }
    return flag;
}

