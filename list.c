#include <pthread.h>
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// !!! FIX SEG FAULT XD

TList* createList(int s){
  TList* list = malloc(sizeof(TList));
  if(!list){
    // debug message
    printf("Failed to allocate memory for a new list\n");
    return NULL;
  }
  list->items = malloc(s * sizeof(char * ));
  if(!list){
    // debug message
    printf("Failed to allocate memory for a new list\n");
    return NULL;
  }
  list->maxSize = s;
  list->currentSize = 0;
  return list;
}
void destroyList(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot destroy the list, because list doesn't exist\n");
    return;
  }
  // currently only array is freed, not all the pointers inside this array
  /*
  for(int i = 0; i < lst->currentSize; i++){
    if (lst->items[i]) {
      free(lst->items[i]);
    }
  }
  */
  free(lst->items);
  free(lst);
  lst = NULL;
}
void putItem(TList *lst, void *itm){
  if(!lst){
    // debug message
    printf("Cannot put item, because list doesn't exist\n");
    return;
  }
  if(lst->currentSize >= lst->maxSize){
    // debug message
    printf("Cannot put item, because list has reached maximum capacity\n");
    return;
  }
  lst->items[lst->currentSize] = itm;
  lst->currentSize++;
}
void* getItem(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot get item, because list doesn't exist\n");
    return NULL;
  }
  if(lst->currentSize == 0){
    // debug message
    printf("Cannot get item, because list is empty\n");
  }
  void* foundItem = lst->items[0];
  for(int i = 1; i < lst->currentSize; i++){
    lst->items[i-1] = lst->items[i];
  }
  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;
  return foundItem;
}
void* popItem(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot pop item, because list doesn't exist\n");
    return NULL;
  }
  if(lst->currentSize == 0){
    // debug message
    printf("Cannot pop item, because list is empty\n");
  }
  void* foundItem = lst->items[lst->currentSize - 1];
  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;
  return foundItem;
}
int removeItem(TList *lst, void *itm){
  if(!lst){
    // debug message
    printf("Cannot remove item, because list doesn't exist\n");
    return 0;
  }
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
  if(removeFlag){
    lst->items[lst->currentSize - 1] = NULL;
    lst->currentSize--;
  }
  return removeFlag;
}
int getCount(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot get count, because list doesn't exist\n");
    return 0;
  }
  return lst->currentSize;
}
void setMaxSize(TList *lst, int s){
  if(!lst){
    // debug message
    printf("Cannot set max size, because list doesn't exist\n");
    return;
  }
  lst->maxSize = s;
}
void appendItems(TList *lst, TList *lst2){
  if(!lst || !lst2){
    // debug message
    printf("Cannot append items, because one of the lists (or both) doesn't exist\n");
    return;
  }
  // appending items until lst reaches max size, lst2 remains unchanged
  for(int i = 0; i < lst2->currentSize; i++){
    if(lst->currentSize >= lst->maxSize){
      printf("List 1 has reached full capacity and not all items from list2 has been appended\n");
      break;
    }
    void *newItem = malloc(sizeof(&(lst2->items[i])));
    memcpy(newItem, lst2->items[i], sizeof(&(lst2->items[i])));
    putItem(lst, newItem);
  }
}
void showList(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot show the list, because it doesn't exist\n");
    return;
  }
  for(int i = 0; i < lst->currentSize; i++){
    printf("%s ", (char*)lst->items[i]);
  }
  printf("\n");
}