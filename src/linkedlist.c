#include "global.h"
#include "linkedlist.h"
#include "recomputils.h"

void LinkedList_initList(LinkedList *list) {
    list->start = NULL;
    list->end = NULL;
}

void LinkedList_destroyList(LinkedList *list) {
    Node *curr = list->start;

    Node *next = NULL;

    while (curr) {
        next = curr->next;
        recomp_free(curr);
        curr = next;
    }
}

Node *LinkedList_start(const LinkedList *list) {
    return list->start;
}

Node *LinkedList_end(const LinkedList *list) {
    return list->end;
}

Node *createNode() {
    Node *n = recomp_alloc(sizeof(Node));
    n->data = NULL;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

void LinkedList_addFront(LinkedList *list, void *data) {
    Node *n = createNode();
    n->data = data;
    if (!list->start) {
        list->start = n;
        list->end = n;
    }
    else {
        list->start->prev = n;
        n->next = list->start;
        list->start = n;
    }
}

void LinkedList_addBack(LinkedList *list, void *data) {
    Node *n = createNode();
    n->data = data;
    if (!list->start) {
        list->start = n;
        list->end = n;
    } else {
        list->end->next = n;
        n->prev = list->end;
        list->end = n;
    }
}

void LinkedList_removeFront(LinkedList *list) {
    if (list->start) {
        Node *oldStart = list->start;
        list->start = oldStart->next;
        recomp_free(oldStart);
    }
}

void LinkedList_removeBack(LinkedList *list) {
    if (list->end) {
        Node *oldEnd = list->end;
        list->end = oldEnd->prev;
        recomp_free(oldEnd);
    }
}

void LinkedList_removeData(LinkedList *list, void *data) {
    Node *curr = list->start;
    while(curr) {
        if (curr->data == data) {
            if (curr->prev) {
                curr->prev->next = curr->next;
            }

            if (curr->next) {
                curr->next->prev = curr->prev;
            }
        }
    }
}

void LinkedList_forEach(LinkedList *list, void (*nodeProcessingFunc)(void *)) {
    Node *curr = list->start;

    while(curr) {
        (nodeProcessingFunc(curr->data));
    }
}
