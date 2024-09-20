
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
	  msgkey = ftok("ammsvc", 65);
	  // msgget creates a message queue
      // and returns identifier
	  msgid = msgget(msgkey, 0666 | IPC_CREAT);
	  
	  //create a new lua state
	  L = luaL_newstate();
	  
	  luaL_openlibs(L);
	  lua_RegisterMemoryFunctions(L);
	  luaopen_luamylib(L);

      dlog::info("luamapd::on_start(): luamapd version: " + cfg.get("version") + " started successfully!");
    }

    void on_update() override {
      /// Called every DURATION set in set_update_duration()...
      /// Update your code here...
	  
	  enum msgtype messageType = luastring;
	  
	  // msgrcv to receive message
	  if(msgrcv(msgid, &message, sizeof(message), messageType, IPC_NOWAIT) != -1)
	  {
		  // display the message
		  std::string s1 ("Data Received is : ");
		  dlog::info( s1 + message.mesg_text);
		  
		  if(luaL_dostring(L,message.mesg_text))
		  {
			  std::string s2 ("lua error: %s\n");
			  dlog::info( s2 + lua_tostring(L,-1));
		  }
		  
	  }
	  
	  messageType = dofile;
	  
	  if(msgrcv(msgid, &message, sizeof(message), messageType, IPC_NOWAIT) != -1)
	  {
		int r = luaL_dofile(L, message.mesg_text);
		std::string s1 ("lua error: %s\n");
		if(r != LUA_OK) dlog::info( s1 + lua_tostring(L,-1));
	  }

      //dlog::info("luamapd::on_update()");
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
	  
	  // to destroy the message queue
      msgctl(msgid, IPC_RMID, NULL);
	  
	  // ftok to generate unique key
	  msgkey = ftok("ammsvc", 65);
	  // msgget creates a message queue
      // and returns identifier
	  msgid = msgget(msgkey, 0666 | IPC_CREAT);

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