
#include "ammsvc.h"
#include "argparse.hpp"


struct MyArgs : public argparse::Args {
	std::string &scriptfile = kwarg("f", "A lua filename to run").set_default("*");;
	std::string &scriptstring = kwarg("s", "A lua string to run","MemoryMap.lua").set_default("*");
	bool &verbose = flag("v,verbose", "A flag to toggle verbose");
};

// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[DAEMON_MSG_MAX_LEN+1];
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
	
	if (args.scriptfile != "*")
	{
		message.mesg_type = messageType;
		if(args.scriptfile.length() <= DAEMON_MSG_MAX_LEN)
		{
			for(int i = 0; i <= args.scriptfile.length(); i++)
			{
				message.mesg_text[i] = args.scriptfile.c_str()[i];
			}
		}
		else
		{
			printf("Error filename too long. \n");
		}
	}
	else if (args.scriptstring != "*")
	{
		messageType = luastring;
		message.mesg_type = messageType;
		if(args.scriptstring.length() <= DAEMON_MSG_MAX_LEN)
		{
			for(int i = 0; i <= args.scriptstring.length(); i++)
			{
				message.mesg_text[i] = args.scriptstring.c_str()[i];
			}
		}
		else
		{
			printf("Error script string too long. \n");
		}
	}
	else
	{
	}

    // msgsnd to send message
    msgsnd(msgid, &message, sizeof(message), 0);

	printf("Sent \n");

    return 0;
}