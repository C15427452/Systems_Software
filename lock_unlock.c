#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <syslog.h>

void lock(char *path){
    int status;
    struct stat buf;
    struct stat st;

    mode_t mode;
    stat(path, &st);
	
    openlog("Locking", LOG_PID|LOG_CONS, LOG_USER);

    mode = st.st_mode & 0700;
    mode &= ~(S_IRUSR);
    mode |= S_IXUSR;

    if(chmod(path, mode) < 0)
    {
	syslog(LOG_INFO, "Failed to lock");
    }
    else
    {
	syslog(LOG_INFO, "File Locked");
    }
    status = stat(path, &buf);
    closelog();
}

void unlock(char *path){
    
    char mode [] = "0777";
    int i;
    i = strtol(mode, 0, 8);

    if(chmod(path, i) < 0)
    {
	syslog(LOG_INFO, "Failed to unlock");
    }
    else
    {
	syslog(LOG_INFO, "File Unlocked");
    }
    //status = stat(path, &buf);
    closelog();
}
