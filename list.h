#include <pthread.h>

struct TList {
	int maxSize;
  int currentSize;
  void** items;
  pthread_mutex_t mutex;
  pthread_cond_t listFull;
  pthread_cond_t listEmpty;
};
typedef struct TList TList;

TList* createList(int s);
void destroyList(TList *lst);
void putItem(TList *lst, void *itm);
void* getItem(TList *lst);
void* popItem(TList *lst);
int removeItem(TList *lst, void *itm);
int getCount(TList *lst);
void setMaxSize(TList *lst, int s);
void appendItems(TList *lst, TList *lst2);
void showList(TList *lst);
