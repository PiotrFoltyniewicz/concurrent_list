#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

void testCreateAndDestroyList() {
    TList *list = createList(10);
    if (list && list->maxSize == 10 && list->currentSize == 0) {
        printf("testCreateAndDestroyList: PASSED\n");
    } else {
        printf("testCreateAndDestroyList: FAILED\n");
    }
    destroyList(list);
    // Set list to NULL after destroying to avoid accidental usage
    list = NULL;
}

void testPutAndGetItem() {
    TList *list = createList(3);
    char *item1 = strdup("item1"); // Using strdup to allocate memory for items
    char *item2 = strdup("item2");
    char *item3 = strdup("item3");

    putItem(list, item1);
    putItem(list, item2);
    putItem(list, item3);

    if (list->currentSize == 3) {
        printf("testPutItem: PASSED\n");
    } else {
        printf("testPutItem: FAILED\n");
    }

    char *gotItem = (char *)getItem(list);
    if (gotItem && strcmp(gotItem, "item1") == 0 && list->currentSize == 2) {
        printf("testGetItem: PASSED\n");
    } else {
        printf("testGetItem: FAILED\n");
    }
    free(gotItem); // Free the item obtained from the list

    destroyList(list);
}

void testPopItem() {
    TList *list = createList(3);
    char *item1 = strdup("item1");
    char *item2 = strdup("item2");
    char *item3 = strdup("item3");

    putItem(list, item1);
    putItem(list, item2);
    putItem(list, item3);

    char *poppedItem = (char *)popItem(list);
    if (poppedItem && strcmp(poppedItem, "item3") == 0 && list->currentSize == 2) {
        printf("testPopItem: PASSED\n");
    } else {
        printf("testPopItem: FAILED\n");
    }
    free(poppedItem); // Free the popped item

    destroyList(list);
}

void testRemoveItem() {
    TList *list = createList(3);
    char *item1 = strdup("item1");
    char *item2 = strdup("item2");
    char *item3 = strdup("item3");

    putItem(list, item1);
    putItem(list, item2);
    putItem(list, item3);

    int removed = removeItem(list, item2);
    if (removed && list->currentSize == 2 && strcmp((char *)list->items[0], "item1") == 0 && strcmp((char *)list->items[1], "item3") == 0) {
        printf("testRemoveItem: PASSED\n");
    } else {
        printf("testRemoveItem: FAILED\n");
    }

    destroyList(list);
}

void testGetCount() {
    TList *list = createList(3);
    char *item1 = strdup("item1");
    char *item2 = strdup("item2");
    char *item3 = strdup("item3");

    putItem(list, item1);
    putItem(list, item2);
    putItem(list, item3);

    int count = getCount(list);
    if (count == 3) {
        printf("testGetCount: PASSED\n");
    } else {
        printf("testGetCount: FAILED\n");
    }

    destroyList(list);
}

void testSetMaxSize() {
    TList *list = createList(3);
    setMaxSize(list, 5);
    if (list->maxSize == 5) {
        printf("testSetMaxSize: PASSED\n");
    } else {
        printf("testSetMaxSize: FAILED\n");
    }
    destroyList(list);
}

void testAppendItems() {
    TList *list1 = createList(5);
    TList *list2 = createList(3);

    char *item1 = strdup("item1");
    char *item2 = strdup("item2");
    char *item3 = strdup("item3");
    char *item4 = strdup("item4");

    putItem(list1, item1);
    putItem(list1, item2);

    putItem(list2, item3);
    putItem(list2, item4);

    appendItems(list1, list2);
    showList(list1);
    destroyList(list1);
    showList(list2);

    if (list1->currentSize == 4 && strcmp((char *)list1->items[2], "item3") == 0 && strcmp((char *)list1->items[3], "item4") == 0) {
        printf("testAppendItems: PASSED\n");
    } else {
        printf("testAppendItems: FAILED\n");
    }
    destroyList(list1);
    destroyList(list2);
}

void testShowList() {
    TList *list = createList(3);
    char *item1 = strdup("item1");
    char *item2 = strdup("item2");
    char *item3 = strdup("item3");

    putItem(list, item1);
    putItem(list, item2);
    putItem(list, item3);

    printf("Expected list: item1 item2 item3\n");
    printf("Actual list: ");
    showList(list);

    destroyList(list);
}

int main() {
    testCreateAndDestroyList();
    testPutAndGetItem();
    testPopItem();
    testRemoveItem();
    testGetCount();
    testSetMaxSize();
    testAppendItems();
    testShowList();

    return 0;
}