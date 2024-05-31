#include <pthread.h>
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
TList* createList(int s){
  TList* list = malloc(sizeof(TList));
  if(!list){
    // debug message
    printf("Failed to allocate memory for a new list\n");
    return NULL;
  }
  list->items = malloc(s * sizeof(char * ));
  if(!list->items){
    // debug message
    printf("Failed to allocate memory for a new list\n");
    return NULL;
  }
  list->maxSize = s;
  list->currentSize = 0;
  pthread_mutex_init(&list->mutex, NULL);
  pthread_cond_init(&list->listFull, NULL);
  pthread_cond_init(&list->listEmpty, NULL);
  return list;
}

void destroyList(TList *lst) {
    if (lst == NULL) {
        // debug message
        printf("Cannot destroy the list, because list doesn't exist\n");
        return;
    }
    pthread_mutex_lock(&lst->mutex);
    free(lst->items);
    pthread_mutex_unlock(&lst->mutex);
    pthread_cond_destroy(&lst->listFull);
    pthread_cond_destroy(&lst->listEmpty);
    pthread_mutex_destroy(&lst->mutex);
    free(lst);
    lst = NULL;
}

void putItem(TList *lst, void *itm){
  if(!lst){
    // debug message
    printf("Cannot put item, because list doesn't exist\n");
    return;
  }
  pthread_mutex_lock(&lst->mutex);
  while(lst->currentSize >= lst->maxSize)
  {
    pthread_cond_wait(&lst->listFull, &lst->mutex);
  }
  lst->items[lst->currentSize] = itm;
  lst->currentSize++;
  pthread_cond_signal(&lst->listFull);
  pthread_mutex_unlock(&lst->mutex);
}

// add condition variable

void* getItem(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot get item, because list doesn't exist\n");
    return NULL;
  }
  pthread_mutex_lock(&lst->mutex);
  if(lst->currentSize == 0){
    // debug message
    printf("Cannot get item, because list is empty\n");
    pthread_mutex_unlock(&lst->mutex);
    return NULL;
  }

  void* foundItem = lst->items[0];
  for(int i = 1; i < lst->currentSize; i++){
    lst->items[i-1] = lst->items[i];
  }
  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;
  pthread_mutex_unlock(&lst->mutex);
  return foundItem;
}

void* popItem(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot pop item, because list doesn't exist\n");
    return NULL;
  }
  pthread_mutex_lock(&lst->mutex);
  if(lst->currentSize == 0){
    // debug message
    printf("Cannot pop item, because list is empty\n");
    pthread_mutex_unlock(&lst->mutex);
    return NULL;
  }
  void* foundItem = lst->items[lst->currentSize - 1];
  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;
  pthread_mutex_unlock(&lst->mutex);
  return foundItem;
}

int removeItem(TList *lst, void *itm){
  if(!lst){
    // debug message
    printf("Cannot remove item, because list doesn't exist\n");
    return 0;
  }
  pthread_mutex_lock(&lst->mutex);
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
  pthread_mutex_unlock(&lst->mutex);
  return removeFlag;
}

int getCount(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot get count, because list doesn't exist\n");
    return 0;
  }
  pthread_mutex_lock(&lst->mutex);
  int count = lst->currentSize;
  pthread_mutex_unlock(&lst->mutex);
  return count;
}

void setMaxSize(TList *lst, int s){
  if(!lst){
    // debug message
    printf("Cannot set max size, because list doesn't exist\n");
    return;
  }
  pthread_mutex_lock(&lst->mutex);
  lst->maxSize = s;
  pthread_mutex_unlock(&lst->mutex);
}

void appendItems(TList *lst, TList *lst2){
  if(!lst || !lst2){
    // debug message
    printf("Cannot append items, because one of the lists (or both) doesn't exist\n");
    return;
  }
  pthread_mutex_lock(&lst->mutex);
  // appending items until lst reaches max size, lst2 remains unchanged
  for(int i = 0; i < lst2->currentSize; i++){
    if(lst->currentSize >= lst->maxSize){
      printf("List 1 has reached full capacity and not all items from list2 has been appended\n");
      break;
    }
    putItem(lst, lst2->items[i]);
  }
  pthread_mutex_unlock(&lst->mutex);
}

void showList(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot show the list, because it doesn't exist\n");
    return;
  }
  pthread_mutex_lock(&lst->mutex);
  for(int i = 0; i < lst->currentSize; i++){
    printf("%p ", lst->items[i]);
  }
  printf("\n");
  pthread_mutex_unlock(&lst->mutex);
}