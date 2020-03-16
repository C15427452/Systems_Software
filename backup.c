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

void backup()
{
    openlog("Backup", LOG_PID|LOG_CONS, LOG_USER);

    char command[100];

    strcpy(command, "cp -r ./home/yasminanunez/Desktop/SS_Assign/var/www/html/live/. ./home/yasminanunez/Desktop/SS_Assign/backup");
    system(command);
 
    if(system(command) < 0)
    {
	openlog("Error Back", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, "Backup Failure");
        closelog();
    } 
    else{
	syslog(LOG_INFO, "Backup Complete");
    }
    closelog();
}


