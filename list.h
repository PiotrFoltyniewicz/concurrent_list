#include <pthread.h>

struct TList {
  int debug;
	int maxSize;
  int currentSize;
  void** items;
  int head;
  int tail;
  pthread_mutex_t mutex;
  pthread_cond_t listNotFull;
  pthread_cond_t listNotEmpty;
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
// value: 0 = false, 1 = true
void debugMessages(TList *lst, int value);
