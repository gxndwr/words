//#define DEBUG
//#define MANUAL_CHECK
#define CHOICE_QUESTION

#include "exam.h"
#include "choice_question.h"

#if defined(DEBUG)
	#define dbg printf
#else
void dbg(char *format, ...) {}
#endif

struct exam_struct exam;

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
    long word_seed;
    int exam_num;
    char words[1000];
    char result[MAX_WORDS_NUM];
    int char_index = 0;
    int checked_num = 0;
    char *date;
    float score;

    // collect input about exam number
	printf("Please input exam number:");
    scanf("%d", &exam_num);
    printf("exam number: %d\n", exam_num);

    sprintf(input, "%d", exam_num);
    sprintf(file_name, "%s.exam", input);
    printf("exam file name: %s\n\n", file_name);
#ifndef   CHOICE_QUESTION
    printf("(pess 'y' if remember, otherwise press other key)\n");
#endif

    // collect launch time as random seed for word index producing
    clock_gettime(CLOCK_REALTIME, &exam.tp);
    word_seed = exam.tp.tv_sec * 1000 + exam.tp.tv_nsec;
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
    exam.correct_num = 0;
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
                    exam.correct_num++;

                printf("%s\n",exam.word[index].answer);
#endif
#else
                choice_question(&exam, index);
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
    scanf("%d", &exam.correct_num);
#else
	printf("Correct: %d\n", exam.correct_num);
#endif
    score = ((float)exam.correct_num/(float)exam.num) * 100;
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
