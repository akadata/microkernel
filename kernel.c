#include "lists.h"
#include "kernel.h"

struct message {
    Node node;
    Task *source;
    void *data;
};

struct task {
    Node node;
    List messages;
};

