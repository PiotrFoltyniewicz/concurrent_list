#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "list.h"

TList* createList(int s){
  TList* list = malloc(sizeof(TList));
  list->items = malloc(s * sizeof(char * ));
  list->headIndex = 0;
  list->tailIndex = 0;
  list->maxSize = s;
  list->currentSize = 0;
}
void destroyList(TList *lst){

}
void putItem(TList *lst, void *itm){

}
void* getItem(TList *lst){

}
void* popItem(TList *lst){

}
int removeItem(TList *lst, void *itm){

}
int getCount(TList *lst){

}
void setMaxSize(TList *lst, int s){

}
void appendItems(TList *lst, TList *lst2){

}
void showList(TList *lst){

}