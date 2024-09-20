
#include "daemon.hpp"
using namespace daemonpp;
using namespace std::chrono_literals;

// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#include "LuaMemoryMapperLib.h"
extern "C"{
#include "printoverride.h"
};

#ifdef DBG_MESSAGES
#ifdef LINUX_BUILD
std::string BuildInfo = "hello world";
#else
#include "revinfo.h"
#endif // LINUX_BUILD
#endif

#include "ammsvc.h"

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[DAEMON_MSG_MAX_LEN+1];
} message;

key_t msgkey;
int msgid;
lua_State* L;

class luamapd : public daemon
{
public:
    void on_start(const dconfig& cfg) override {
      /// Called once after daemon starts automatically with system startup or when you manually call `$ systemctl start luamapd`
      /// Initialize your code here...
	  // ftok to generate unique key
	  msgkey = ftok("/usr/bin/ammsvc", 65);
	  
	  //create a new lua state
	  L = luaL_newstate();
	  
	  luaL_openlibs(L);
	  lua_RegisterMemoryFunctions(L);
	  luaopen_luamylib(L);

      dlog::info("luamapd::on_start(): luamapd version: " + cfg.get("version") + " started successfully!");
    }

	void on_update() override
	{
		// msgget creates a message queue
		// and returns identifier
		msgid = msgget(msgkey, 0666 | IPC_CREAT);
		if (msgid == -1)
		{
			//dlog::info("msgget failed");
			printf("Error could not create message queue. \n");
			dlog::info("Error could not create message queue.");
			exit(EXIT_FAILURE);
		}
		
		enum msgtype messageType = luastring;

		// Try to receive a message of type luastring
		if (msgrcv(msgid, &message, sizeof(message), messageType, IPC_NOWAIT) != -1) {
			// Process the message
			std::string receivedData("Data Received is: ");
			dlog::info(receivedData + message.mesg_text);
			
			// Execute the Lua string
			if (luaL_dostring(L, message.mesg_text)) {
				std::string luaError("Lua error: ");
				dlog::info(luaError + lua_tostring(L, -1));
			}
		} else {
			// Handle case where no message is received or an error occurred
			if (errno != ENOMSG) {
				//dlog::info("msgrcv failed");
			}
		}

		// Check for dofile messages similarly
		messageType = dofile;
		if (msgrcv(msgid, &message, sizeof(message), messageType, IPC_NOWAIT) != -1) {
			// Process the dofile message
			int r = luaL_dofile(L, message.mesg_text);
			if (r != LUA_OK) {
				std::string luaError("Lua error: ");
				dlog::info(luaError + lua_tostring(L, -1));
			}
		} else {
			if (errno != ENOMSG) {
				//dlog::info("msgrcv file failed");
			}
		}
		// to destroy the message queue
		msgctl(msgid, IPC_RMID, NULL);
	}


    void on_stop() override {
      /// Called once before daemon is about to exit with system shutdown or when you manually call `$ systemctl stop luamapd`
      /// Cleanup your code here...
	  
	  lua_close(L);
	  
	  // to destroy the message queue
      msgctl(msgid, IPC_RMID, NULL);

      dlog::info("luamapd::on_stop()");
    }

    void on_reload(const dconfig& cfg) override {
      /// Called once after your daemon's config fil is updated then reloaded with `$ systemctl reload luamapd`
      /// Handle your config updates here...
	  
	  //restart the lua instance
	  lua_close(L);
	  L = luaL_newstate();
	  
	  luaL_openlibs(L);
	  lua_RegisterMemoryFunctions(L);
	  luaopen_luamylib(L);

      dlog::info("luamapd::on_reload(): new daemon version from updated config: " + cfg.get("version"));
    }
};

int main(int argc, const char* argv[]) {
  luamapd dmn;                         // create a daemon instance
  dmn.set_name("luamapd"); // set daemon name to identify logs in syslog
  dmn.set_update_duration(1s);   // set duration to sleep before triggering the on_update callback 1 seconds
  dmn.set_cwd("/");      // set daemon's current working directory to root /
  dmn.run(argc, argv);                   // run your daemon
  return EXIT_SUCCESS;
}