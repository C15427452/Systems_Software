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

void update()
{
    openlog("Update", LOG_PID|LOG_CONS, LOG_USER);

    char command[255];

    strcpy(command, "cp -r ./home/yasminanunez/Desktop/SS_Assign/var/www/html/intranet/. ./home/yasminanunez/Desktop/SS_Assign/var/www/html/live");
    system(command);
 
    if(system(command) < 0)
    {
	openlog("Error Back", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, "Update Failure");
        closelog();
    } 
    else{
	syslog(LOG_INFO, "Update complete");
    }
    closelog();
}

