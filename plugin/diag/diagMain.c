#include "diag.h"
#include <wdef.h>
#include <unistd.h>

#include <linux/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <signal.h>

#include <system/thread.h>
#include <system/lx_lock.h>


typedef struct DiagEvent_t
{
	int event;
	char*value;
	int isLoop;
	WThread thread;
	Wlock muter;
	Diag *p_diag;
}DiagEvent;

#define DIAG_EVENT_LOCALIP 0x1
#define DIAG_EVENT_NETIP 0x2
#define DIAG_EVENT_SHOW 0x4
#define DIAG_EVENT_TIME 0x8
#define DIAG_EVENT_SHUTDOWN 0x10


static int getlocalip(char* buffer){
	int i = 0;
	int sockfd;
	struct ifconf ifconf;
	char buf[512];
	struct ifreq *ifreq;
	char* ip;
	//初始化ifconf
	ifconf.ifc_len = 512;
	ifconf.ifc_buf = buf;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		return -1;
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf); //获取所有接口信息
	close(sockfd);
	//接下来一个一个的获取IP地址
	ifreq = (struct ifreq*)buf;
	for (i = (ifconf.ifc_len / sizeof(struct ifreq)); i > 0; i--)
	{
		ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
		if (strcmp(ip, "127.0.0.1") == 0) //排除127.0.0.1，继续下一个
		{
			ifreq++; continue;
		}
		strcpy(buffer, ip);
		return 0;
	}
	return -1;
}
DiagEvent* diag =NULL;

void sigEvent(int sig)
{
	switch (sig)
	{
	case SIGPWR://收到关机消息
	{
		printf("shutdown \n");
		lx_lock(diag->muter);
		diag->event = DIAG_EVENT_SHUTDOWN;
		SAFE_FREE(diag->value);
		diag->value = strdup("0FF");
		lx_unlock(diag->muter);
	}break;
	case SIGKILL:{
		
		diag->isLoop = 0;

		
		}break;
	default:
		break;
	}
}


static const char* wgetshorttime(char *buffer)
{
	time_t now;
	time(&now);
	
	
	strftime(buffer, 20, "%H-%M-%S", localtime(&now));
	return buffer;

}

static void showtext(DiagEvent* pEvent,const char* info)
{
	char text[5] = { 0 };
	
	const char* p=info;
	while(pEvent->isLoop)
	{
		int len =strlen(p);
		if(len==0) break;
		if(len>4)
			strncpy(text, p, 4);
		else
			strcpy(text,p);
		for (int i = 0; i < 60 && pEvent->isLoop;++i)
		{
			diag_shownumStr(pEvent->p_diag, text);
			delay(2);
		}
		++p;
	}
}

static void showip(DiagEvent* pEvent,int isLocal,const char* ip)
{

	char tmp[20]={0};
	
	if (isLocal)
			strcpy(tmp, "LIP-");
		else
			strcpy(tmp, "NIP-");
	strcat(tmp,ip);
	showtext(pEvent, tmp);
	
}

static void showtime(DiagEvent* pEvent)
{
	char timebuffer[30]={0};

	showtext(pEvent,wgetshorttime(timebuffer));
}

static ThreadResult WorkingThread(void* lpParam)
{
	DiagEvent* pEvent = NULL;
	INTOFUNC();
	pEvent = (DiagEvent*)lpParam;
	if (!pEvent){
		perror("input param is null");
		goto error_lab;
	}
	while (pEvent->isLoop){
		lx_lock(pEvent->muter);
		switch (pEvent->event)
		{
		case DIAG_EVENT_LOCALIP:
		{
			char tmp[20]={0};
			int ret = getlocalip(tmp);
			if(ret)
				strcpy(tmp,"0.0.0.0");
			showip(pEvent, 1, tmp);
		}break;
		case DIAG_EVENT_TIME:
		{
			showtime(pEvent);
		}break;
		case DIAG_EVENT_SHOW:
			showtext(pEvent,pEvent->value);
			break;
		case DIAG_EVENT_SHUTDOWN:
		{
			showtext(pEvent, pEvent->value);
		}break;
		default:
			break;
		}
		lx_unlock(pEvent->muter);
	}
	diag_clean(pEvent->p_diag);
error_lab:
	EXITFUNC();
	return NULL;
}

extern char *optarg;

int main(int argc, char *argv[])
{
	
	const char *optstring = "lns:t";
	DiagEvent _event = { 0 };
	int ret = 0;
	char buffer[20] = { 0 };
	int opt=0;
	int state=0;
	while ((opt=getopt(argc, argv, optstring)) != -1)
	{
		switch (opt)
		{
		case 'l':
			_event.event = DIAG_EVENT_LOCALIP;
			
			break;
		case 'n':
			_event.event = DIAG_EVENT_NETIP;
			break;
		case 's':
			_event.event = DIAG_EVENT_SHOW;
			if (_event.value)
				free(_event.value);
			_event.value = strdup(optarg);
			break;
		case 't':
			_event.event = DIAG_EVENT_TIME;
			break;
		default:
			return  -1;
		}
	}
	if (_event.event == 0)
	{
		fprintf(stderr, "param is null.");
		return -1;
	}
	diag=&_event;
	signal(SIGPWR, sigEvent);
	signal(SIGKILL, sigEvent);
	_event.muter = lx_lock_init();
	_event.isLoop = 1;

	_event.p_diag = diag_create(7, 9, 8);
	printf("start thread...\n");
	_event.thread = WThread_Create(WorkingThread, &_event, 0);

	WThread_Wait(_event.thread, INFINITE, &state);
	WThread_Close(_event.thread);
	_event.thread = NULL;
	lx_lock_free(_event.muter);
	_event.muter = NULL;
	diag_destroy(_event.p_diag);
	_event.p_diag = NULL;
	SAFE_FREE(_event.value);

	return 1;
}