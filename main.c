#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"


int main(){

    TList* list = createList(5);
    int item1 = 1;
    int item2 = 1;
    int item3 = 1;
    putItem(list, &item1);
    putItem(list, &item2);
    putItem(list, &item3);
    putItem(list, &item1);
    putItem(list, &item2);
    putItem(list, &item3);
    showList(list);
    popItem(list);
    getItem(list);
    showList(list);
    destroyList(list);
    putItem(list, &item3);
    return 0;
}