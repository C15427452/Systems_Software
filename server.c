#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "lock_unlock.h"

#define PATH_I "var/www/html/intranet"

int main(void)
{
    char intra_p[100];
    strcpy(intra_p, PATH_I);

	mqd_t mq;
	struct mq_attr queue_attributes;
        char buffer[1024 +1];
        int terminate = 0;

        queue_attributes.mq_flags = 0;
        queue_attributes.mq_maxmsg = 10;
        queue_attributes.mq_msgsize = 1024;
        queue_attributes.mq_curmsgs = 0;

        mq = mq_open("/dt228_queue", O_CREAT | O_RDONLY, 0644, &queue_attributes);
    
	openlog("Server started", LOG_PID|LOG_CONS, LOG_USER);

        do
        {
	    ssize_t bytes_read;

	    bytes_read = mq_receive(mq, buffer, 1024, NULL);
	
	    buffer[bytes_read] = '\0';
	    if(!strncmp(buffer, "exit", strlen("exit")))
	    {
	    	terminate = 1;
	    }
	    else if(!strncmp(buffer, "backup", strlen("backup")))
	    {
		lock(intra_p);
		sleep(1);
		char command[100];

		strcpy(command, "cp -r ./var/www/html/live/. ./backup");
		system(command);
		 
	        if(system(command) < 0)
	        {
	     	   openlog("Error Back", LOG_PID|LOG_CONS, LOG_USER);
		   syslog(LOG_INFO, "Manual Backup Failure");
		   closelog();
		} 
		else{
		   syslog(LOG_INFO, "Manual Backup Complete");
		   printf("\nRecieved: Manual Backup Complete\n");
		   sleep(1);
		   unlock(intra_p);
		}
		closelog();
		sleep(1);
  	    }
	    else if(!strncmp(buffer, "update", strlen("update")))
	    {
		lock(intra_p);
		sleep(1);
		char command[100];

		strcpy(command, "cp -r ./var/www/html/intranet/. ./var/www/html/live");
		system(command);
		 
	        if(system(command) < 0)
	        {
	     	   openlog("Error Back", LOG_PID|LOG_CONS, LOG_USER);
		   syslog(LOG_INFO, "Manual Update Failure");
		   closelog();
		} 
		else{
		   syslog(LOG_INFO, "Manual Update Complete");
		   printf("\nRecieved: Manual Update Complete\n");
		   sleep(1);
		   unlock(intra_p);
		}
		closelog();
		sleep(1);
  	    }
    	}while(!terminate);

	mq_close(mq);
    	mq_unlink("/dt228_queue");
	return 0;
}
