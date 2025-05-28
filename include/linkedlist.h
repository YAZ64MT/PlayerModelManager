#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

typedef struct Node {
    void *data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct {
    Node *start;
    Node *end;
} LinkedList;

void LinkedList_initList(LinkedList *list);

void LinkedList_destroyList(LinkedList *list);

Node *LinkedList_start(const LinkedList *list);

Node *LinkedList_end(const LinkedList *list);

void LinkedList_addFront(LinkedList *list, void *data);

void LinkedList_addBack(LinkedList *list, void *data);

void LinkedList_removeFront(LinkedList *list);

void LinkedList_removeBack(LinkedList *list);

void LinkedList_removeData(LinkedList *list, void *data);

void LinkedList_forEach(LinkedList *list, void (*nodeProcessingFunc)(void *));

#endif