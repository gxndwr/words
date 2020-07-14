#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#include <string.h>

#if 1
#define DEBUG
#endif

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

int main(void)
{
    FILE *fp;
    char *date;
    float score;
    int exam_num;
    time_t timep;
    int correct_num;
    char result[100];
    char words[1000];
    int words_num = 0;
    char file_name[20];

	printf("Please input exam number:");
    scanf("%d", &exam_num);
    printf("exam number: %d\n", exam_num);

    sprintf(file_name, "%d", exam_num);
    sprintf(file_name, "%s.exam", file_name);
    printf("exam file name: %s\n\n", file_name);

    fp = fopen(file_name, "a+");
    fseek(fp, 0L, SEEK_SET);
    fgets(words, 1000, fp);
    //dbg("exam words: %s\n", words);
    getchar();

    for (int i = 0; i<1000; i++) {
        //dbg("i: %d\n", i);
        if (words[i] == '\n') {
            printf("%c", words[i]);
            words_num++;
            break;
        }

        if (words[i] == 32){
            //dbg("\nwords[%d]: %d\n", i, words[i]);
            words_num++;
            getchar();
            continue;
        }

        printf("%c", words[i]);
    }
    printf("Total: %d\n", words_num);

	printf("Correct number:");
    scanf("%d", &correct_num);
    score = ((float)correct_num/(float)words_num) * 100;
	printf("Score: %.1f\n", score);

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
