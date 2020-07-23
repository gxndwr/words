#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

//#define DEBUG
//#define MANUAL_CHECK
#define CHOICE_QUESTION

#if defined(DEBUG)
	#define dbg printf
#else
void dbg(char *format, ...) {}
#endif

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
} exam;

void disable_io_buffer(void)
{
    struct termios now;

	tcgetattr(0, &now);
	now.c_lflag &= ~ICANON;
	now.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &now);
}

void enable_io_buffer(void)
{
	//tcsetattr(0, TCSANOW, &old);
    struct termios now;

	tcgetattr(0, &now);
	now.c_lflag |= ICANON;
	now.c_lflag |= ECHO;
	tcsetattr(0, TCSANOW, &now);
}
int main(void)
{
    int i;
    FILE *fp;
    char answer;
    char input[14];
    char file_name[20];
    time_t timep;
    int word_seed;
    int exam_num;
    int correct_num;
    char words[1000];
    char result[MAX_WORDS_NUM];
    int char_index = 0;
    int checked_num = 0;
    char *date;
    float score;
    struct timespec *tp;

    // collect input about exam number
	printf("Please input exam number:");
    scanf("%d", &exam_num);
    printf("exam number: %d\n", exam_num);

    sprintf(input, "%d", exam_num);
    sprintf(file_name, "%s.exam", input);
    printf("exam file name: %s\n\n", file_name);
    printf("(pess 'y' if remember, otherwise press other key)\n");

    // collect launch time as random seed for word index producing
    clock_gettime(CLOCK_REALTIME, tp);
    word_seed = tp->tv_sec * 1000 + tp->tv_nsec;
	srand(word_seed);

    // import data from first line of exam file
    fp = fopen(file_name, "a+");
    fseek(fp, 0L, SEEK_SET);
    fgets(words, 1000, fp);
    //dbg("exam words: %s\n", words);
    getchar();

    i = 0;
    exam.num = 0;
    while(words[i] != '\n') {
        if (exam.num > MAX_WORDS_NUM) {
            printf("Words number more than %d\n", MAX_WORDS_NUM);
            return -1;
        }
        //dbg("words[%d]: %c\n", i, words[i]);
        if (words[i] == ' ') {
            if (char_index != 0) {
                exam.word[exam.num].flag |= VALID;
                char_index = 0;
                exam.num++;
            }
            i++;
            continue;
        }

        if (words[i] == ':') {
            exam.word[exam.num].flag |= ANSWER;
            char_index = 0;
            i++;
            continue;
        }

        if(exam.word[exam.num].flag & ANSWER) {
            exam.word[exam.num].answer[char_index++] = words[i];
            exam.word[exam.num].answer[char_index] = 0;
            //dbg("exam.word[%d].answer[%d]: %c\n", exam.num, char_index-1, words[i]);
        } else {
            exam.word[exam.num].question[char_index++] = words[i];
            exam.word[exam.num].question[char_index] = 0;
            //dbg("exam.word[%d].question[%d]: %c\n", exam.num, char_index-1, words[i]);
        }
        i++;
    }
    exam.word[exam.num++].flag |= VALID; // for the last word

    // display word one by one
    correct_num = 0;
#ifndef   CHOICE_QUESTION
    disable_io_buffer();
#endif
    for (i = 0; checked_num < exam.num; i++) {
        int index = rand()%exam.num;
        if (!(exam.word[index].flag & CHECKED)) {
#ifndef CHOICE_QUESTION
                printf("%s: ",exam.word[index].question);
                answer = getchar();
#ifndef MANUAL_CHECK
                if (answer == 'y')
                    correct_num++;

                printf("%s\n",exam.word[index].answer);
#endif
#else
                int answer_pos = rand()%4;
                int opt_index;
                dbg("answer_pos: %d\n", answer_pos);
                // print question
                printf("\n%s:\n\n", exam.word[index].question);

                int used_candidate_index_1;
                int used_candidate_index_2;
                int used_candidate_index_3;
                // print 1st option
                if (0 == answer_pos) {
                    printf("A) %s    ", exam.word[index].answer);
                } else {
                    opt_index = rand()%exam.num;

                    while(1) {
                        if (opt_index != index)
                            break;
                        opt_index = rand()%exam.num;
                    }

                    printf("A) %s    ", exam.word[opt_index].answer);
                    used_candidate_index_1 = opt_index;
                }
                // print 2nd option
                if (1 == answer_pos) {
                    printf("B) %s    ", exam.word[index].answer);
                } else {
                    opt_index = rand()%exam.num;

                    while(1) {
                        if (opt_index != index)
                            if (opt_index != used_candidate_index_1)
                                break;
                        opt_index = rand()%exam.num;
                    }

                    printf("B) %s    ", exam.word[opt_index].answer);
                    used_candidate_index_2 = opt_index;
                }
                // print 3rd option
                if (2 == answer_pos) {
                    printf("C) %s    ", exam.word[index].answer);
                } else {
                    opt_index = rand()%exam.num;

                    while(1) {
                        if (opt_index != index)
                            if (opt_index != used_candidate_index_1)
                                if (opt_index != used_candidate_index_2)
                                    break;
                        opt_index = rand()%exam.num;
                    }

                    printf("C) %s    ", exam.word[opt_index].answer);
                    used_candidate_index_3 = opt_index;
                }
                // print 4th option
                if (3 == answer_pos) {
                    printf("D) %s\n", exam.word[index].answer);
                } else {
                    opt_index = rand()%exam.num;

                    while(1) {
                        if (opt_index != index)
                            if (opt_index != used_candidate_index_1)
                                if (opt_index != used_candidate_index_2)
                                    if (opt_index != used_candidate_index_3)
                                    break;
                        opt_index = rand()%exam.num;
                    }

                    printf("D) %s\n", exam.word[opt_index].answer);
                }

                int a;
                a = getchar() - 'a';
                if (a == answer_pos) {
                    printf("Correct!\n");
                    correct_num++;
                } else {
                    printf("Wrong... The answer is: %s\n", exam.word[index].answer);
                    int original_sec, current_sec;
                    clock_gettime(CLOCK_REALTIME, tp);
                    original_sec = tp->tv_sec;
                    while(1){
                    clock_gettime(CLOCK_REALTIME, tp);
                    current_sec = tp->tv_sec;
                        if (current_sec > original_sec + 4)
                            break;
                    }
                }

                //dbg("%s\n",exam.word[index].answer);
                while(getchar() != '\n'){};
#endif
                exam.word[index].flag |= CHECKED;
                checked_num++;
                //dbg("exam.word[%02d]: %s \t checked_num: %d\n", index, exam.word[index].question, checked_num);
        }
    }
#ifndef   CHOICE_QUESTION
    enable_io_buffer();
#endif

    printf("--------------\n");
    printf("Total: %d\n", exam.num);
#ifdef MANUAL_CHECK
    // collect correct num and calculated score
	printf("Correct number:");
    scanf("%d", &correct_num);
#else
	printf("Correct: %d\n", correct_num);
#endif
    score = ((float)correct_num/(float)exam.num) * 100;
	printf("Score: %.1f\n", score);

    // append result to the end of file
    time (&timep);
    fseek(fp, 0L, SEEK_END);
    date = ctime(&timep);
    date[strlen(date)-1] = 0;
    sprintf(result, "%s score: %.1f\n", date, score);
    //dbg("\nresult: %s\n", result);
    fputs(result, fp);

    fclose(fp);

	return 0;
}
