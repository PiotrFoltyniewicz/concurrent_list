#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <unistd.h>

struct Args{
    TList* list;
    int num;
};
typedef struct Args Args;

void* putter(void* arg){
    Args* args = (Args*) arg;
    TList* list = args->lst;
    int num = args->num;
    putItem(list, &num);
    putItem(list, &num);
    putItem(list, &num);
}
void* getter(void* arg){
    TList* list = (TList *)arg;
    getItem(list);
    getItem(list);
    getItem(list);
}

int main(){
    TList* list = createList(6);
    Args* args1 = malloc(sizeof(Args));
    Args* args2 = malloc(sizeof(Args));
    Args* args3 = malloc(sizeof(Args));
    args1->list = list;
    args2->list = list;
    args3->list = list;
    args1->num = 1;
    args2->num = 2;
    args3->num = 3;
    pthread_t th1, th2, th3;
    pthread_create(&th1, NULL, putter, args1);
    pthread_create(&th2, NULL, putter, args2);
    pthread_create(&th3, NULL, putter, args3);
    showList(list);
    getItem(list);
    getItem(list);
    getItem(list);
    showList(list);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    showList(list);
    printf("Count: %d", getCount(list));
    free(args1);
    free(args2);
    free(args3);
    return 0;
}