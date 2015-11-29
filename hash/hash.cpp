#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

//#define M 249997
#define M 131072
int string_len = 12;
int string_num = 10000;
typedef unsigned int uint32_t;
uint32_t hash[M];
char **strings;


void hash_init()
{
    uint32_t i, j;
    srand((unsigned)time(NULL));
    strings = (char **)calloc(string_num, sizeof(char*));
    for (i = 0; i < string_num; ++i)
    {
        strings[i] = (char *)calloc(string_len, sizeof(char));
    }
    for (i = 0; i < string_num; ++i)
    {
        for (j = 0; j < string_len; j++) {
            strings[i][j] = (char)(rand() % 256);
        }
        //printf("%s\n", strings[i]);
    }
}

void hash_free()
{
    int i, j;
    for (i = 0; i < string_num; i++)
        free(strings[i]);
    free(strings);
}

uint32_t hash_count()
{
    uint32_t sum = 0;
    uint32_t i = 0;

    for (i = 0; i < M; i++) 
        sum += hash[i];
    for (i = 0; i < M; i++) {
        if (hash[i] != 0) {
            //printf("hash[%d]=%d\n", i, hash[i]);
            sum--;
        }
    } 

    return sum;
}

int BKDR_hash(char *str)
{
    int seed = 131;/* 32, 131, 1313, 13131, 131313, ... */
    int hash = 0;
    int i;
    for (i = 0; i < string_len; i++) {
        hash = (hash * seed) + str[i];
    }
    hash &= 0x7fffffff;
    return (hash % M);
}

void BKDR_test()
{
    int i, ind;
    memset(hash, 0, M * sizeof(uint32_t));
    timeval st, end;
    gettimeofday(&st, NULL);
    for (i = 0; i < string_num; i++) {
        ind = BKDR_hash(strings[i]);
        //printf("%d %d\n", i, ind);
        hash[ind]++;
    }
    gettimeofday(&end, NULL);
    
    double span = (end.tv_sec - st.tv_sec) * 1000.0 + 
                (end.tv_usec - st.tv_usec) / 1000.0;
    printf("BKDR_hash: %u, %lfms\n", hash_count(), span);
}


int onebyone_hash(char *str)
{
    int hash = 0; 
    int i;
    int M_MASK = 0x8765fed1;
    for (i = 0; i < string_len; i++) {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash & (M - 1);

    //hash &= 0x7fffffff;
    //return hash % M;
}

void onebyone_test()
{
    int i, ind;
    memset(hash, 0, M * sizeof(uint32_t));
    timeval st, end;
    gettimeofday(&st, NULL);
    for (i = 0; i < string_num; i++) {
        ind = onebyone_hash(strings[i]);
        //printf("%d %d\n", i, ind);
        hash[ind]++;
    }
    gettimeofday(&end, NULL);
    double span = (end.tv_sec - st.tv_sec) * 1000.0 + 
                (end.tv_usec - st.tv_usec) / 1000.0;
    printf("onebyone_hash: %u, %lfms\n", hash_count(), span);
}

void show_usage(char *cmd)
{
    printf("%s string_len string_num\n", cmd);
}

int main(int argc, char *argv[])
{
    if (argc != 3) 
        show_usage(argv[0]);
    else {
        sscanf(argv[1], "%d", &string_len);
        sscanf(argv[2], "%d", &string_num);
    }

    hash_init();

    BKDR_test();
    onebyone_test();

    hash_free();

    return 0;
} 