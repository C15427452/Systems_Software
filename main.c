#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <time.h>

#include "backup.h"
#include "monitor.h"
#include "update.h"
#include "lock_unlock.h"

#define PATH_I "home/yasminanunez/Desktop/SS_Assign/var/www/html/intranet"
#define PATH_L "home/yasminanunez/Desktop/SS_Assign/var/www/html/live"
#define PATH_B "home/yasminanunez/Desktop/SS_Assign/backup"

int main()
{
    char intra_p[255];
    char live_p[255];
    char back_p[255];

    strcpy(intra_p, PATH_I);
    strcpy(live_p, PATH_L);
    strcpy(back_p, PATH_B);

    int i;

    int pid = fork();
    if(pid > 0)
    {
	printf("Parent Process");
	sleep(5);
	exit(EXIT_SUCCESS);
    }
    else if(pid == 0)
    {
	openlog("Assignment", LOG_PID|LOG_CONS, LOG_USER);
        closelog();

	if(setsid() < 0)
	{
	    exit(EXIT_FAILURE);
	}

	int pid = fork();
	if(pid > 0)
	{
	    exit(EXIT_SUCCESS);
	}	
	else
	{
	    monitor();
	    umask(0);

	    if(chdir("/") < 0)
	    {
		exit(EXIT_FAILURE);
	    }

	    int x;
	    for(x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
	    {
		close(x);
   	    }

	    openlog("Assignment", LOG_PID|LOG_CONS, LOG_USER);

	    while(1)
	    {
		sleep(1);
		syslog(LOG_INFO, "Daemon started");
		time_t t = time(NULL);
		struct tm lol = *localtime(&t);
		//server();
		if(lol.tm_hour == 17 && lol.tm_min == 26 && lol.tm_sec == 0)
		{
		    lock(intra_p);
		    sleep(1);
		    backup();
		    sleep(1);
		    syslog(LOG_INFO, "Successfully Backed Up");
		    //sleep(1);
		    update();
		    //sleep(1);
		    syslog(LOG_INFO, "Successfully Updated Live");
		    sleep(1);
		    unlock(intra_p);
		}		     
	    }
	}
    }
    closelog();
    return(0);
}

