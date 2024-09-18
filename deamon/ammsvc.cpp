

#include "argparse.hpp"


struct MyArgs : public argparse::Args {
	std::string &scriptfile = kwarg("f", "A keyworded integer value");
	bool &verbose = flag("v,verbose", "A flag to toggle verbose");
};

// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 10

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

int main()
{
	
	
	
    key_t key;
    int msgid;

    // ftok to generate unique key
    key = ftok("ammsvc", 65);

    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 1;

    printf("Write Data : ");
    fgets(message.mesg_text,MAX,stdin);

    // msgsnd to send message
    msgsnd(msgid, &message, sizeof(message), 0);

    // display the message
    printf("Data send is : %s \n", message.mesg_text);

    return 0;
}