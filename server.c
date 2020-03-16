#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "backup.h"

void server()
{
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
		backup();
		printf("Recieved: Manual Backup Complete");
		terminate = 1;
  	    }
    	}while(!terminate);

	mq_close(mq);
    	mq_unlink("/dt228_queue");
}
