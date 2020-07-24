#include "exam.h"


void choice_question(struct exam_struct *e, int index )
{
    int answer_pos;
    int opt_index;
    int used_candidate_index_1;
    int used_candidate_index_2;
    int used_candidate_index_3;

    answer_pos = rand()%4;
    //dbg("answer_pos: %d\n", answer_pos);
    // print question
    printf("\n  %s:\n\n", e->word[index].question);

    // print 1st option
    if (0 == answer_pos) {
        printf(" A) %s    ", e->word[index].answer);
    } else {
        opt_index = rand()%e->num;

        while(1) {
            if (opt_index != index)
                break;
            opt_index = rand()%e->num;
        }

        printf("A) %s    ", e->word[opt_index].answer);
        used_candidate_index_1 = opt_index;
    }
    // print 2nd option
    if (1 == answer_pos) {
        printf("B) %s    ", e->word[index].answer);
    } else {
        opt_index = rand()%e->num;

        while(1) {
            if (opt_index != index)
                if (opt_index != used_candidate_index_1)
                    break;
            opt_index = rand()%e->num;
        }

        printf("B) %s    ", e->word[opt_index].answer);
        used_candidate_index_2 = opt_index;
    }
    // print 3rd option
    if (2 == answer_pos) {
        printf("C) %s    ", e->word[index].answer);
    } else {
        opt_index = rand()%e->num;

        while(1) {
            if (opt_index != index)
                if (opt_index != used_candidate_index_1)
                    if (opt_index != used_candidate_index_2)
                        break;
            opt_index = rand()%e->num;
        }

        printf("C) %s    ", e->word[opt_index].answer);
        used_candidate_index_3 = opt_index;
    }
    // print 4th option
    if (3 == answer_pos) {
        printf("D) %s\n", e->word[index].answer);
    } else {
        opt_index = rand()%e->num;

        while(1) {
            if (opt_index != index)
                if (opt_index != used_candidate_index_1)
                    if (opt_index != used_candidate_index_2)
                        if (opt_index != used_candidate_index_3)
                            break;
            opt_index = rand()%e->num;
        }

        printf("D) %s\n", e->word[opt_index].answer);
    }

    int a;
    a = getchar() - 'a';
    if (a == answer_pos) {
        printf("Correct!\n");
        e->correct_num++;
    } else {
        printf("Wrong... The right answer is: %c) %s\n", 'A' + answer_pos, e->word[index].answer);
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

    //dbg("%s\n",e->word[index].answer);
    while(getchar() != '\n'){};
}
