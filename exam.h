#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#define MAX_WORDS_NUM 100
#define MAX_WORD_LEN 30
#define VALID   (1 << 0)
#define CHECKED (1 << 1)
#define ANSWER  (1 << 2)


struct word_struct {
    char question[MAX_WORD_LEN ];
    char answer[MAX_WORD_LEN ];
    int flag;
};

struct exam_struct {
    struct word_struct word[MAX_WORDS_NUM];
    int num;
    int correct_num;
    struct timespec tp;
};
