#include "exam.h"


void fill_in_question(struct exam_struct *e, int index )
{
    int i = 0;
    char input[MAX_WORD_LEN];

    printf("\n  %s: ", e->word[index].question);

    // collect input and make judgement
    fgets(input, MAX_WORD_LEN, stdin);

    while(input[i] != 10) {
        //printf("iput[%d]: %d, %c\n", i, input[i], input[i]);
        if (input[i] == ' '){
            //printf("found space\n");
            input[i] = '_';
        }
        i++;
    }
    input[i] = 0; // remove '/n'

    if (!strcmp(input, e->word[index].answer)) {
        printf("Correct!\n");
        e->correct_num++;
    } else {
        printf("Wrong... The right answer is: %s\n", e->word[index].answer);

        int original_sec, current_sec;
        clock_gettime(CLOCK_REALTIME, &e->tp);
        original_sec = e->tp.tv_sec;
        while(1){
            clock_gettime(CLOCK_REALTIME, &e->tp);
            current_sec = e->tp.tv_sec;
            if (current_sec > original_sec + 4)
                break;
        }
    }
}
