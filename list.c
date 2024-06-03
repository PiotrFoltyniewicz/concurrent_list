#include <pthread.h>
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
TList* createList(int s) {
  TList* list = malloc(sizeof(TList));
  if(!list) {
    perror("Failed to allocate memory for a new list");
    return NULL;
  }

  list->items = malloc(s * sizeof(char *));
  if(!list->items) {
    perror("Failed to allocate memory for list items");
    free(list);
    return NULL;
  }

  list->maxSize = s;
  list->currentSize = 0;

  if(pthread_mutex_init(&list->mutex, NULL) != 0) {
    perror("Failed to initialize mutex");
    free(list->items);
    free(list);
    return NULL;
  }

  if(pthread_cond_init(&list->listNotFull, NULL) != 0) {
    perror("Failed to initialize condition variable listNotFull");
    pthread_mutex_destroy(&list->mutex);
    free(list->items);
    free(list);
    return NULL;
  }

  if(pthread_cond_init(&list->listNotEmpty, NULL) != 0) {
    perror("Failed to initialize condition variable listNotEmpty");
    pthread_cond_destroy(&list->listNotFull);
    pthread_mutex_destroy(&list->mutex);
    free(list->items);
    free(list);
    return NULL;
  }

  return list;
}

void destroyList(TList *lst) {
    if(lst == NULL) {
        // debug message
        printf("Cannot destroy the list, because list doesn't exist\n");
        return;
    }

    pthread_mutex_lock(&lst->mutex);
    free(lst->items);
    pthread_mutex_unlock(&lst->mutex);

    if(pthread_cond_destroy(&lst->listNotFull) != 0){
      perror("Failed to destroy conditional variable listNotFull");
    }

    if(pthread_cond_destroy(&lst->listNotEmpty) != 0){
      perror("Failed to destroy conditional variable listNotEmpty");
    }

    if(pthread_mutex_destroy(&lst->mutex) != 0){
      perror("Failed to destroy mutex");
    }

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
    // debug message
    printf("Cannot put item, because list is full\n");
    pthread_cond_wait(&lst->listNotFull, &lst->mutex);
  }

  lst->items[lst->currentSize] = itm;
  lst->currentSize++;

  pthread_cond_signal(&lst->listNotEmpty);
  pthread_mutex_unlock(&lst->mutex);
}

void* getItem(TList *lst){
  if(!lst){
    // debug message
    printf("Cannot get item, because list doesn't exist\n");
    return NULL;
  }

  pthread_mutex_lock(&lst->mutex);

  while(lst->currentSize == 0){
    printf("Cannot get item, because list is empty\n");
    pthread_cond_wait(&lst->listNotEmpty, &lst->mutex);
  }

  void* foundItem = lst->items[0];
  for(int i = 1; i < lst->currentSize; i++){
    lst->items[i-1] = lst->items[i];
  }

  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;

  if(lst->currentSize < lst->maxSize){
    pthread_cond_signal(&lst->listNotFull);
  }

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

  while(lst->currentSize == 0){
    printf("Cannot pop item, because list is empty\n");
    pthread_cond_wait(&lst->listNotEmpty, &lst->mutex);
  }

  void* foundItem = lst->items[lst->currentSize - 1];
  lst->items[lst->currentSize - 1] = NULL;
  lst->currentSize--;

  if(lst->currentSize < lst->maxSize){
    pthread_cond_signal(&lst->listNotFull);
  }

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
  while(lst->currentSize == 0){
    printf("Cannot remove item, because list is empty\n");
    pthread_cond_wait(&lst->listNotEmpty, &lst->mutex);
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
    if(lst->currentSize < lst->maxSize){
      pthread_cond_signal(&lst->listNotFull);
    }
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
  void** temp = realloc(lst->items, sizeof(void *) * s);
  if(!temp){
    perror("Failed to reallocate memory for items");
  }else{
    lst->items = temp;
    lst->maxSize = s;
  }

  pthread_mutex_unlock(&lst->mutex);
}

void appendItems(TList *lst, TList *lst2){
  if(!lst || !lst2){
    // debug message
    printf("Cannot append items, because one of the lists (or both) doesn't exist\n");
    return;
  }

  pthread_mutex_lock(&lst->mutex);
  int newSize = lst->currentSize + lst2->currentSize;

  if(newSize > lst->maxSize){
    setMaxSize(lst, newSize);
  }

  for(int i = 0; i < lst2->currentSize; i++){
    lst->items[lst->currentSize] = lst->items[i];
    lst->currentSize++;
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