#include <lua.h>//Lua 5.1.5
#include <lauxlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>  
 



#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <wiringPi.h>


static int wificar_setup(lua_State* l)
{

	wiringPiSetup();
	return 0;
}

static int wificar_pinMode(lua_State*l)
{
	if(lua_gettop(l)!=2)
		return 0;
	int gpio=lua_tointeger(l,1);
	int isOut=lua_tointeger(l,2);
	pinMode(gpio,isOut!=0?OUTPUT:INPUT);
	lua_pushinteger(l,0);
	return 1;
}

static int wificar_write(lua_State*l)
{
	if(lua_gettop(l)!=2)
		return 0;
	int gpio=lua_tointeger(l,1);
	int isLow=lua_tointeger(l,2);
	digitalWrite(gpio,isLow!=0?LOW:HIGH);
	lua_pushinteger(l,0);
	return 1;
}

static int wificar_read(lua_State* l)
{
	if(lua_gettop(l)!=1)
		return 0;
	int gpio=lua_tointeger(l,1);
	int ret=digitalRead(gpio);
	lua_pushinteger(l,ret);
	return 1;
}



static const struct luaL_Reg car_lib[] = {
	{ "setup", wificar_setup },
	{ "pinMode", wificar_pinMode },
	{ "write", wificar_write },
	{ "read", wificar_read },
	{ NULL, NULL }
};


#define LUA_CARLIBNAME "car"

LUALIB_API int luaopen_car(lua_State *L)
{
	luaL_register(L, LUA_CARLIBNAME, car_lib);

	return 1;
}