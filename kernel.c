#include "lists.h"
#include "kernel.h"

struct message {
    Node node;
    Task *source;
    Message_type type;
};

struct task {
    Node node;
    List messages;
};

