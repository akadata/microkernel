#include <stdlib.h>
#include "list.h"

void list_init(List *list)
{
    list->lh_head = (Node *) &list->lh_tail;
    list->lh_tail = NULL;
    list->lh_tailpred = (Node *) &list->lh_head;
}

void list_add_end(List *list, Node *node)
{
    node->ln_succ = (Node *) &list->lh_tail;
    node->ln_pred = list->lh_tailpred;
    list->lh_tailpred->ln_succ = node;
    list->lh_tailpred = node;
}

void list_enqueue(List *list, Node *node)
{
    Node *nextnode;

    nextnode = list->lh_head;
    while (NULL != nextnode->ln_succ) {
        if (nextnode->ln_pri < node->ln_pri) {
            break;
        }
        nextnode = nextnode->ln_succ;
    }
    node->ln_succ = nextnode;
    node->ln_pred = nextnode->ln_pred;
    nextnode->ln_pred->ln_succ = node;
    nextnode->ln_pred = node;
}

void list_remove_node(Node *node)
{
    node->ln_pred->ln_succ = node->ln_succ;
    node->ln_succ->ln_pred = node->ln_pred;
}

Node *list_remove_head(List *list)
{
    Node *node;
    node = list->lh_head;
    if (!node->ln_succ) {
        return NULL;
    }
    node->ln_succ->ln_pred = (Node *) &list->lh_head;
    list->lh_head = node->ln_succ;
    return node;
}

Node *list_head(List *list)
{
    Node *node;
    node = list->lh_head;
    if (!node->ln_succ) {
        return NULL;
    }
    return node;
}

