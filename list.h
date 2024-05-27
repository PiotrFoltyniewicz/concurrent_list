#include <semaphore.h>

struct TList {
	int maxSize;
  int currentSize;
  void** items;
  int headIndex;
  int tailIndex; 
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
