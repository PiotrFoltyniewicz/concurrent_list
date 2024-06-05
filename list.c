#include <pthread.h>
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TList* createList(int s) {
    TList* list = malloc(sizeof(TList));
    if (!list) {
        perror("Failed to allocate memory for a new list");
        return NULL;
    }

    list->items = malloc(s * sizeof(char *));
    if (!list->items) {
        perror("Failed to allocate memory for list items");
        free(list);
        return NULL;
    }

    list->maxSize = s;
    list->currentSize = 0;
    list->head = 0;
    list->tail = 0;
    list->debug = 0;

    if (pthread_mutex_init(&list->mutex, NULL) != 0) {
        perror("Failed to initialize mutex");
        free(list->items);
        free(list);
        return NULL;
    }

    if (pthread_cond_init(&list->listNotFull, NULL) != 0) {
        perror("Failed to initialize condition variable listNotFull");
        pthread_mutex_destroy(&list->mutex);
        free(list->items);
        free(list);
        return NULL;
    }

    if (pthread_cond_init(&list->listNotEmpty, NULL) != 0) {
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
    if (!lst) {
        if (lst->debug) printf("Cannot destroy the list, because list doesn't exist\n");
        return;
    }

    pthread_mutex_lock(&lst->mutex);
    for (int i = 0; i < lst->currentSize; i++) {
        free(lst->items[(lst->head + i) % lst->maxSize]);
    }
    free(lst->items);
    pthread_mutex_unlock(&lst->mutex);

    if (pthread_cond_destroy(&lst->listNotFull) != 0) {
        perror("Failed to destroy conditional variable listNotFull");
    }

    if (pthread_cond_destroy(&lst->listNotEmpty) != 0) {
        perror("Failed to destroy conditional variable listNotEmpty");
    }

    if (pthread_mutex_destroy(&lst->mutex) != 0) {
        perror("Failed to destroy mutex");
    }

    free(lst);
}

void putItem(TList *lst, void *itm) {
    if (!lst) {
        if (lst->debug) printf("Cannot put item, because list doesn't exist\n");
        return;
    }

    pthread_mutex_lock(&lst->mutex);

    while (lst->currentSize >= lst->maxSize) {
        if (lst->debug) printf("Cannot put item, because list is full\n");
        pthread_cond_wait(&lst->listNotFull, &lst->mutex);
    }

    lst->items[lst->tail] = itm;
    lst->tail = (lst->tail + 1) % lst->maxSize;
    lst->currentSize++;

    pthread_cond_signal(&lst->listNotEmpty);
    pthread_mutex_unlock(&lst->mutex);
}

void* getItem(TList *lst) {
    if (!lst) {
        if (lst->debug) printf("Cannot get item, because list doesn't exist\n");
        return NULL;
    }

    pthread_mutex_lock(&lst->mutex);

    while (lst->currentSize == 0) {
        if (lst->debug) printf("Cannot get item, because list is empty\n");
        pthread_cond_wait(&lst->listNotEmpty, &lst->mutex);
    }

    void* foundItem = lst->items[lst->head];
    lst->items[lst->head] = NULL;
    lst->head = (lst->head + 1) % lst->maxSize;
    lst->currentSize--;

    pthread_cond_signal(&lst->listNotFull);
    pthread_mutex_unlock(&lst->mutex);
    return foundItem;
}

void* popItem(TList *lst) {
    if (!lst) {
        if (lst->debug) printf("Cannot pop item, because list doesn't exist\n");
        return NULL;
    }

    pthread_mutex_lock(&lst->mutex);

    while (lst->currentSize == 0) {
        if (lst->debug) printf("Cannot pop item, because list is empty\n");
        pthread_cond_wait(&lst->listNotEmpty, &lst->mutex);
    }

    lst->tail = (lst->tail - 1 + lst->maxSize) % lst->maxSize;
    void* foundItem = lst->items[lst->tail];
    lst->items[lst->tail] = NULL;
    lst->currentSize--;

    pthread_cond_signal(&lst->listNotFull);
    pthread_mutex_unlock(&lst->mutex);
    return foundItem;
}

int removeItem(TList *lst, void *itm) {
    if (!lst) {
        if (lst->debug) printf("Cannot remove item, because list doesn't exist\n");
        return 0;
    }

    pthread_mutex_lock(&lst->mutex);
    int removeFlag = 0;

    for (int i = 0; i < lst->currentSize; i++) {
        int index = (lst->head + i) % lst->maxSize;
        if (!removeFlag && itm == lst->items[index]) {
            free(lst->items[index]);
            removeFlag = 1;
        }
        if (removeFlag) {
            int nextIndex = (index + 1) % lst->maxSize;
            lst->items[index] = lst->items[nextIndex];
        }
    }

    if (removeFlag) {
        lst->tail = (lst->tail - 1 + lst->maxSize) % lst->maxSize;
        lst->currentSize--;
        pthread_cond_signal(&lst->listNotFull);
    }

    pthread_mutex_unlock(&lst->mutex);
    return removeFlag;
}

int getCount(TList *lst) {
    if (!lst) {
        if (lst->debug) printf("Cannot get count, because list doesn't exist\n");
        return 0;
    }

    pthread_mutex_lock(&lst->mutex);
    int count = lst->currentSize;
    pthread_mutex_unlock(&lst->mutex);

    return count;
}

void setMaxSize(TList *lst, int s) {
    if (!lst) {
        if (lst->debug) printf("Cannot set max size, because list doesn't exist\n");
        return;
    }
    pthread_mutex_lock(&lst->mutex);
    void** temp = realloc(lst->items, sizeof(void *) * s);
    if (!temp) {
        perror("Failed to reallocate memory for items");
    } else {
        lst->items = temp;
        if (s < lst->currentSize) {
            lst->maxSize = lst->currentSize;
        } else {
            lst->maxSize = s;
        }
        lst->maxSize = s;
        if(lst->currentSize < lst->maxSize){
            pthread_cond_signal(&lst->listNotFull);
        }
    }
    pthread_mutex_unlock(&lst->mutex);
}

void appendItems(TList *lst, TList *lst2) {
    if (!lst || !lst2) {
        if (lst->debug) printf("Cannot append items, because one of the lists (or both) doesn't exist\n");
        return;
    }

    pthread_mutex_lock(&lst->mutex);
    while (lst->currentSize >= lst->maxSize) {
        if (lst->debug) printf("Cannot append items, because list is full\n");
        pthread_cond_wait(&lst->listNotFull, &lst->mutex);
    }

    pthread_mutex_lock(&lst2->mutex);
    int newSize = lst->currentSize + lst2->currentSize;
    int appendTail = lst->tail;
    if(lst->currentSize >= lst->maxSize){
      appendTail = lst->currentSize;
    }
    if (newSize > lst->maxSize) {
        pthread_mutex_unlock(&lst->mutex);
        setMaxSize(lst, newSize);
        pthread_mutex_lock(&lst->mutex);
    }

    int listFull = 0;
    for (int i = 0; i < lst2->currentSize; i++) {
        lst->items[appendTail] = lst2->items[(lst2->head + i) % lst2->maxSize];
        appendTail++;
        if(!listFull && appendTail == lst->head){
          listFull = 1;
          appendTail = lst->currentSize;
        }
    }
    lst->tail = (lst->tail + 1) % newSize;
    lst->currentSize = newSize;

    pthread_mutex_unlock(&lst2->mutex);
    pthread_mutex_unlock(&lst->mutex);
}

void showList(TList *lst) {
    if (!lst) {
        if (lst->debug) printf("Cannot show the list, because it doesn't exist\n");
        return;
    }

    pthread_mutex_lock(&lst->mutex);
    for (int i = 0; i < lst->currentSize; i++) {
        printf("%p ", lst->items[(lst->head + i) % lst->maxSize]);
    }

    printf("\n");
    pthread_mutex_unlock(&lst->mutex);
}

void debugMessages(TList *lst, int value) {
    if (!lst) {
        if (lst->debug) printf("Cannot change visibility of debug messages, because list doesn't exist\n");
        return;
    }
    pthread_mutex_lock(&lst->mutex);
    lst->debug = value;
    pthread_mutex_unlock(&lst->mutex);
}
