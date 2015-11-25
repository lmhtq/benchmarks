#include <stdio.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <stdlib.h>
#include <assert.h>

unsigned long loop_time = 100;
unsigned long item_num = 10000;
const int item_size = 1024;

long sum = 0;

void show_usage(char *cmd)
{
    printf("%s item_num, loop_time\n", cmd);
}

void array_test()
{
    struct item
    {
        char data[item_size];
    };

    struct item items[item_num];

    timeval st, end;
    gettimeofday(&st, NULL);
    sum = 0;

    for (int i = 0; i < loop_time; i++) {
        struct item tmp = items[i];
        for (int j = 0; j < item_size; j++) {
            tmp.data[j]++;
            sum++;
        }
    }


    gettimeofday(&end, NULL);
    double span = (end.tv_sec - st.tv_sec)*1000.0 + (end.tv_usec - st.tv_usec)/1000.0;
    printf("sum: %d, array_test: %lfms\n", sum, span);    
}

void list_test()
{
    struct item
    {
        char data[item_size];
        TAILQ_ENTRY(item) item_link;
    };

    struct item items_pool[item_num];

    TAILQ_HEAD(, item) item_list;
    TAILQ_INIT(&item_list);

    struct item *tmp;

    for (int i = 0; i < item_num; i++) {
        tmp = (struct item*)calloc(1, sizeof(struct item));
        assert(tmp);
        TAILQ_INSERT_TAIL(&item_list, tmp, item_link);
    }

    timeval st, end;
    gettimeofday(&st, NULL);

    for (int i = 0; i < loop_time; i++) {
        TAILQ_FOREACH(tmp, &item_list, item_link) {
            for (int j = 0; j < item_size; j++)
                tmp->data[j]++;
        }

    }

    gettimeofday(&end, NULL);
    double span = (end.tv_sec - st.tv_sec)*1000.0 + (end.tv_usec - st.tv_usec)/1000.0;
    printf("list_test: %lfms\n", span);    
}

void list_withpool_test()
{
    struct item
    {
        char data[item_size];
        TAILQ_ENTRY(item) item_link;
    };

    struct item items_pool[item_num];

    TAILQ_HEAD(, item) item_list;
    TAILQ_INIT(&item_list);

    struct item *tmp;

    for (int i = 0; i < item_num; i++) {
        tmp = items_pool + i;
        assert(tmp);
        TAILQ_INSERT_TAIL(&item_list, tmp, item_link);
    }

    timeval st, end;
    gettimeofday(&st, NULL);

    for (int i = 0; i < loop_time; i++) {
        TAILQ_FOREACH(tmp, &item_list, item_link) {
            for (int j = 0; j < item_size; j++)
                tmp->data[j]++;
        }

    }

    gettimeofday(&end, NULL);
    double span = (end.tv_sec - st.tv_sec)*1000.0 + (end.tv_usec - st.tv_usec)/1000.0;
    printf("list_withpool_test: %lfms\n", span);    
}

void list_withprefetch_test()
{
    struct item
    {
        char data[item_size];
        TAILQ_ENTRY(item) item_link;
    };

    struct item items_pool[item_num];

    TAILQ_HEAD(, item) item_list;
    TAILQ_INIT(&item_list);

    struct item *tmp;

    for (int i = 0; i < item_num; i++) {
        tmp = items_pool + i;
        assert(tmp);
        TAILQ_INSERT_TAIL(&item_list, tmp, item_link);
    }

    timeval st, end;
    gettimeofday(&st, NULL);

    for (int i = 0; i < loop_time; i++) {
        TAILQ_FOREACH(tmp, &item_list, item_link) {
            for (int j = 0; j < item_size; j++) {
                tmp->data[j]++;
                __builtin_prefetch(&tmp->data[j+10], 0, 1);
            }
        }

    }

    gettimeofday(&end, NULL);
    double span = (end.tv_sec - st.tv_sec)*1000.0 + (end.tv_usec - st.tv_usec)/1000.0;
    printf("list_withprefetch_test: %lfms\n", span);    
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        show_usage(argv[0]);
    else {
        sscanf(argv[1], "%lu", &item_num);
        sscanf(argv[2], "%lu", &loop_time);
    }
    show_usage(argv[0]);

    array_test();
    list_test();
    list_withpool_test();
    list_withprefetch_test();
    
    return 0;
}