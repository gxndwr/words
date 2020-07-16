#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

#define DEBUG
//#define MANUAL_CHECK

#if defined(DEBUG)
	#define dbg printf
#else
void dbg(char *format, ...)
{

}
#endif

#if 0
static struct timeb timeSeed;
char get_input()
{
	ftime(&timeSeed);
    enqueue_seed->seed = timeSeed.time * 1000 + timeSeed.millitm;
    enqueue_seed = enqueue_seed->next;
    return getchar();
}
#endif
#define MAX_WORDS_NUM 100
#define VALID   (1 << 0)
#define CHECKED (1 << 1)
struct word_struct {
    char word[20];
    int flag;
};

struct words_struct {
    struct word_struct word[MAX_WORDS_NUM];
    int num;
} w;

static struct timeb timeSeed;

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

    // collect input about exam number
	printf("Please input exam number:");
    scanf("%d", &exam_num);
    printf("exam number: %d\n", exam_num);

    sprintf(file_name, "%d", exam_num);
    sprintf(file_name, "%s.exam", file_name);
    printf("exam file name: %s\n\n", file_name);

    // collect launch time as random seed for word index producing
	ftime(&timeSeed);
    word_seed = timeSeed.time * 1000 + timeSeed.millitm;
	srand(word_seed);

    // import data from first line of exam file
    fp = fopen(file_name, "a+");
    fseek(fp, 0L, SEEK_SET);
    fgets(words, 1000, fp);
    //dbg("exam words: %s\n", words);
    getchar();

    i = 0;
    w.num = 0;
    while(words[i] != '\n') {
        if (w.num > MAX_WORDS_NUM) {
            printf("Words number more than %d\n", MAX_WORDS_NUM);
            return -1;
        }
        //dbg("i: %d\n", i);
        if (words[i] == 32){
            w.word[w.num].word[char_index + 1] = 0;
            w.word[w.num].flag |= VALID;
            char_index = 0;
            w.num++;
            i++;
            continue;
        }
        w.word[w.num].word[char_index++] = words[i];
        i++;
    }
    w.word[w.num++].flag |= VALID; // for the last word

    // display word one by one
    correct_num = 0;
    disable_io_buffer();
    for (i = 0; checked_num < w.num; i++) {
        int index = rand()%w.num;
        if (!(w.word[index].flag & CHECKED)) {
                w.word[index].flag |= CHECKED;
                checked_num++;
                //dbg("w.word[%02d]: %s \t checked_num: %d\n", index, w.word[index].word, checked_num);
                printf("%s\n", w.word[index].word);
                answer = getchar();
#ifndef MANUAL_CHECK
                if (answer == 'y')
                    correct_num++;
#endif

        }
    }
    enable_io_buffer();

    printf("Total: %d\n", w.num);
#ifdef MANUAL_CHECK
    // Collect correct num and calculated score
	printf("Correct number:");
    scanf("%d", &correct_num);
#endif
    score = ((float)correct_num/(float)w.num) * 100;
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
