#define SIGNAL_MESSAGE ((Signal) 1)
#define SIGNAL_REPLY ((Signal) 2)
#define SIGNAL_INTERRUPT ((Signal) 4)

struct message {
    Node node;
    Task *source;
};

