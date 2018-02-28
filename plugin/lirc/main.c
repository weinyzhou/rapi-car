#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lirc/lirc_client.h>

#include <lua.h>  
#include <lualib.h>  
#include <lauxlib.h> 

#define LIRC_CLIENT_ID "carRobot"



void PrintLuaError(lua_State* L,int sErr)  
{  
    if (sErr==0)  
    {  
        return;  
    }  
    const char* error;  
    char sErrorType[256]={0}; 
    switch(sErr)  
    {  
    case LUA_ERRSYNTAX://编译时错误  
        /*const char *buf = "mylib.myfun()2222";类似这行语句可以引起编译时错误*/  
        snprintf(sErrorType,sizeof(sErrorType),"%s","syntax error during pre-compilation");  
        break;  
    case LUA_ERRMEM://内存错误  
        snprintf(sErrorType,sizeof(sErrorType),"%s","memory allocation error");  
        break;  
    case LUA_ERRRUN://运行时错误  
        /*const char *buf = "my222lib.myfun()";类似这行语句可以引起运行时错误，my222lib实际上不存在这样的库，返回的值是nil*/  
        snprintf(sErrorType,sizeof(sErrorType),"%s","a runtime error");  
        break;  
    case LUA_YIELD://线程被挂起错误  
        snprintf(sErrorType,sizeof(sErrorType),"%s","Thread has Suspended");  
        break;  
    case LUA_ERRERR://在进行错误处理时发生错误  
        snprintf(sErrorType,sizeof(sErrorType),"%s","error while running the error handler function");  
        break;  
    default:  
        break;  
    }  
    error = lua_tostring(L, -1);//打印错误结果  
    fprintf(stderr,"%s:%s",sErrorType,error);  
    lua_pop(L, 1);   
}

int main(int argc, char *argv[])
{
         struct lirc_config *config;
 		 lua_State* lua = lua_open();
 		
        
         if(lirc_init(LIRC_CLIENT_ID,1)==-1) exit(EXIT_FAILURE);
         if(lua==NULL){
         	fprintf(stderr,"open lua failed.\n");
         	exit(EXIT_FAILURE);

         }
 		 luaopen_base(lua);
    	 luaL_openlibs(lua);

    	 int ret= luaL_dofile(lua, "/usr/local/lib/lua/resty/carRobotfunc.lua");
	     if (ret)  
	     {  
			PrintLuaError(lua,ret); 
			if(lua)lua_close(lua);
	        return 0;  
	     } 
         if(lirc_readconfig("/etc/lirc/lircrc",&config,NULL)==0)
         {
                   char *code=NULL;
                   char *c;
                   int ret;
				   
                   while(lirc_nextcode(&code)==0)
                   {
					   printf("read lirc_nextcode code is %p value=%s\n",code,code);
                            if(code==NULL) continue;

                            while((ret=lirc_code2char(config,code,&c))==0 &&
                                  c!=NULL)
                            {

                            	const char* p=NULL;
                            	printf("lirc_code2char %s.\n",c);
                            	if(strncmp(c,"car_",4)){
                            		continue;
                            	}
                            	p=c+4;
                            	switch(*p)
                            	{
                            		case 'b':
                            		{
										if(!strcmp(p,"back"))
		                                {
											lua_getglobal(lua, "carBack");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
		                                }
                            		}break;
                            		case 's':
                            		{
										if(!strcmp(p,"stop"))
		                                {
		                                	lua_getglobal(lua, "carStopCar");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
		                                }
                            		}break;
                            		case 'l':
                            		{
                            			if(!strcmp(p,"left")){
                            				lua_getglobal(lua, "carLeft");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
		                                }else if(!strcmp(p,"leftback")){
		                                	lua_getglobal(lua, "carLeftback");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
		                                }
                            		}break;
                            		case 'f':
                            		{
										if(!strcmp(p,"front")){
											lua_getglobal(lua, "carFront");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
                                		}
                            		}break;
                            		case 'r':
                            		{
                            			if(!strcmp(p,"right")){
                            				lua_getglobal(lua, "carRight");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
		                                }else if(!strcmp(p,"rightback")){
		                                	lua_getglobal(lua, "carRightback");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
		                                }else if(!strcmp(p,"reboot")){
		                                	lua_getglobal(lua, "carReboot");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
		                                }

                            		}break;
                            		case 'o':{
                            			 if(!strcmp(p,"off")){
                            			 	lua_getglobal(lua, "carOff");
											
										    ret=lua_pcall(lua, 0, 1, 0);
											if(ret!=0){
												PrintLuaError(lua,ret);
											}
											lua_pop(lua, 1);
		                                }
                            		}break;

                            	}
                                

                            }
                            printf("operation end\n",code,code);
                            free(code);
                            if(ret==-1) break;

                   }
                   lirc_freeconfig(config);
         }
 
         lirc_deinit();
         if(lua)lua_close(lua);
         exit(EXIT_SUCCESS);
}
