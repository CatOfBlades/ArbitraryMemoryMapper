
#include "ammsvc.h"
#include "argparse.hpp"


struct MyArgs : public argparse::Args {
	std::string &scriptfile = kwarg("f", "A lua filename to run");
	std::string &scriptstring = kwarg("s", "A lua string to run","MemoryMap.lua");
	bool &verbose = flag("v,verbose", "A flag to toggle verbose");
};

// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[512];
} message;

int main(int argc, char* argv[])
{
	auto args = argparse::parse<MyArgs>(argc, argv);
	
    key_t key;
    int msgid;

    // ftok to generate unique key
    key = ftok("ammsvc", 65);

    // msgget creates a message queue
    // and returns identifier
    msgid = msgget(key, 0666 | IPC_CREAT);
	
	enum msgtype messageType = dofile;
	
	if (args.scriptfile.is_valid)
	{
		message.mesg_type = messageType;
		message.mesg_text = args.scriptfile;
	}
	else if (args.scriptstring.is_valid)
	{
		messageType = luastring;
		message.mesg_type = messageType;
		message.mesg_text = args.scriptstring;
	}
	else
	{
	}

    // msgsnd to send message
    msgsnd(msgid, &message, sizeof(message), 0);

	printf("Sent \n");

    return 0;
}