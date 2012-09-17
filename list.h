/* Linked list library. See "Amiga Rom Kernel Reference Manual:
Libraries". */

#include <stdint.h>

#define PRIORITY_IDLE (INT8_MIN)
#define PRIORITY_LOW (INT8_MIN / 2)
#define PRIORITY_NORMAL (0)
#define PRIORITY_HIGH (INT8_MAX / 2)
#define PRIORITY_MAX (INT8_MAX)

typedef int8_t Priority;
typedef struct node Node;
typedef struct list List;

/* A list node.*/
struct node {
    /* The successor node. */
    Node *ln_succ;
    /* The predecessor node. */
    Node *ln_pred;
    /* The priority of the node. */
    Priority ln_pri;
};

/* A linked list. A list header contains the head and tail
nodes for the list. These nodes do not contain any data. The
lh_head and lh_tail fields of struct list act like ln_succ
and lh_pred fields of struct node. */
struct list {
    /* The successor node of the head node. */
    Node *lh_head;
    /* The predecessor of the head AND the successor node of
    the tail node. */
    Node *lh_tail;
    /* The predessessor node of the tail node. */
    Node *lh_tailpred;
};


/* list_init: Prepare list for use. */
void list_init(List *list);

/* list_add_end: Add node to end of list. */
void list_add_end(List *list, Node *node);

/* list_enqueue: Enqueue node in list. The node is inserted
in the list before the first node with a priority lower than
node. */
void list_enqueue(List *list, Node *node);

/* list_remove_node: Remove node from a list. The list is
implicit as a node can be in at most one list at a time. */
void list_remove_node(Node *node);

/* list_remove_head: Remove and return the head node of list. */
Node *list_remove_head(List *list);

/* list_head: Return the head node. The list is not modified. */
Node *list_head(List *list);

