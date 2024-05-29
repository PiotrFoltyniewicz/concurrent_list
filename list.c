#include <pthread.h>
#include "list.h"

TList* createList(int s){
  TList* list = malloc(sizeof(TList));
  list->items = malloc(s * sizeof(char * ));
  list->maxSize = s;
  list->currentSize = 0;
  return list;
}
void destroyList(TList *lst){
  for(int i = 0; i < lst->currentSize; i++){
    free(lst->items[i]);
  }
  free(lst->items);
  free(lst);
}
void putItem(TList *lst, void *itm){
  // if lst is null then error
  if(lst->currentSize >= lst->maxSize) return;
  lst->items[lst->currentSize] = itm;
  lst->currentSize++;
}
void* getItem(TList *lst){
  // block if list is empty
  void* foundItem = lst->items[0];
  for(int i = 1; i < lst->currentSize; i++){
    lst->items[i-1] = lst->items[i];
  }
  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;
  return foundItem;
}
void* popItem(TList *lst){
  // block if list is empty
  void* foundItem = lst->items[lst->currentSize - 1];
  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;
}
int removeItem(TList *lst, void *itm){
  int removeFlag = 0;
  for(int i = 0; i < lst->currentSize; i++){
    if(itm == lst->items[i] && !removeFlag){
      removeFlag = 1;
      continue;
    }
    else if(removeFlag){
      lst->items[i-1] = lst->items[i];
    }
  }
  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;
  return removeFlag
}
int getCount(TList *lst){
  return lst->currentSize;
}
void setMaxSize(TList *lst, int s){
  lst->maxSize = s;
}
void appendItems(TList *lst, TList *lst2){
  // appending items until lst reaches max size
  for(int i = 0; i < lst2->currentSize; i++){
    if(lst->currentSize == lst->maxSize){
      printf("Exceeded maximum list capacity when adding two lists");
      break;
    }
    putItem(lst, lst2->items[i]);
  }
}
void showList(TList *lst){
  for(int i = 0; i < lst->currentSize; i++){
  }
}