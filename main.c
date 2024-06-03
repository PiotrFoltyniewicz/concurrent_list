#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <unistd.h>

struct RemoverArgs{
    TList* list;
    int* nums;
};
typedef struct RemoverArgs RemoverArgs;

void* putter(void* arg){
    TList* list = (TList*) arg;
    int nums[10] = {1,2,3,4,5,6,7,8,9,10};
    for(int i = 0; i < 10; i++){
        putItem(list, &nums[i]);
        printf("Added item\n");
        sleep(1);
    }
    printf("Putter finished\n");
    return NULL;
}

void* getter(void* arg){
    TList* list = (TList *)arg;
    for(int i = 0; i < 5; i++){
        getItem(list);
        printf("Got item\n");
        sleep(1);
    }
    printf("Getter finished\n");
    return NULL;
}

void* popper(void* arg){
    TList* list = (TList *)arg;
    for(int i = 0; i < 5; i++){
        popItem(list);
        printf("Popped item\n");
        sleep(1);
    }
    printf("Popper finished\n");
    return NULL;
}

void* remover(void* arg){
    RemoverArgs* args = (RemoverArgs*) arg;
    TList* list = args->list;
    for(int i = 0; i < 5; i++){
        removeItem(list, &args->nums[i]);
        printf("Removed item\n");
        sleep(1);
    }
    printf("Remover finished\n");
    return NULL;
}

void* counter(void* arg){
    TList* list = (TList *)arg;
    for(int i = 0; i < 10; i++){
        printf("Current count is: %d\n", getCount(list));
        sleep(1);
    }
    printf("Counter finished\n");
    return NULL;
}

void* presenter(void* arg){
    TList* list = (TList *)arg;
    for(int i = 0; i < 5; i++){
        showList(list);
        sleep(2);
    }
    printf("Presenter finished\n");
    return NULL;
}

int main(){

    TList* list = createList(6);
    TList* list2 = createList(10);
    pthread_t th1, th2, th3, th4, th5, th6, th7, th8;
    pthread_create(&th1, NULL, putter, list);
    pthread_create(&th2, NULL, popper, list);
    pthread_create(&th3, NULL, putter, list2);
    pthread_create(&th4, NULL, presenter, list);
    pthread_create(&th5, NULL, counter, list);
    pthread_create(&th6, NULL, getter, list);
    pthread_create(&th7, NULL, putter, list);
    sleep(7);
    setMaxSize(list, 20);
    printf("Max size set\n");
    int* nums = malloc(sizeof(int) * 5);
    RemoverArgs* args = malloc(sizeof(RemoverArgs));
    for(int i = 0; i < 5; i++){
        nums[i] = i + 1;
    }
    args->list = list;
    args->nums = nums;
    pthread_create(&th8, NULL, remover, args);
    appendItems(list, list2);
    printf("Appended items\n");
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);
    pthread_join(th5, NULL);
    pthread_join(th6, NULL);
    pthread_join(th7, NULL);
    pthread_join(th8, NULL);
    printf("Finished\n");
    showList(list);
    printf("Count: %d\n", getCount(list));
    destroyList(list);
    destroyList(list2);
    free(nums);
    free(args);
    return 0;
}