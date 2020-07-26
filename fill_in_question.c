#include "exam.h"


void fill_in_question(struct exam_struct *e, int index )
{
    char input[MAX_WORD_LEN];

    printf("\n  %s: ", e->word[index].question);

    // collect input and make judgement
    scanf("%s", input);
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
