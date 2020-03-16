#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void monitor()
{
    char audit[500];

    strcpy(audit, "ausearch -f /var/www/html/intranet/ > accesslogs.txt");
    system(audit);
 
    if(system(audit) < 0)
    {
	openlog("Error Aud", LOG_PID|LOG_CONS, LOG_USER);
        syslog(LOG_INFO, "Audit Failure");
        closelog();
    }
    else
    {
	syslog(LOG_INFO, "Audit Success");
    }   
}
