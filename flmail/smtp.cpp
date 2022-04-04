/*
**
**  Copyright (c) 1997 Ralf S. Engelschall, All rights reserved.
**
**  This program is free software; it may be redistributed and/or modified
**  only under the terms of either the Artistic License or the GNU General
**  Public License, which may be found in the SMTP source distribution.
**  Look at the file COPYING. 
**
**  This program is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  ======================================================================
**
**  smtpclient_main.c -- program source
**
**  Based on smtp.c as of August 11, 1995 from
**      W.Z. Venema,
**      Eindhoven University of Technology,
**      Department of Mathematics and Computer Science,
**      Den Dolech 2, P.O. Box 513, 5600 MB Eindhoven, The Netherlands.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <syslog.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include <pwd.h>
#include <unistd.h>

static char *from_addr  = NULL;
static char *mailhost   = NULL;
static int   mailport   = 25;
static int   verbose    = 0;
static int   usesyslog  = 0;

static FILE *sfp;
static FILE *rfp;

#define dprintf  if (verbose) printf
#define dvprintf if (verbose) vprintf

/* hack for Ultrix */
#ifndef LOG_DAEMON
#define LOG_DAEMON 0
#endif

static char *errlist[] = {
/*  0 - NOERROR */ "No error status currently",
/*  1 - EPERM   */ "Not super-user",
/*  2 - ENOENT  */ "No such file or directory",
/*  3 - ESRCH   */ "No such process",
/*  4 - EINTR   */ "Interrupted system call",
/*  5 - EIO     */ "I/O error",
/*  6 - ENXIO   */ "No such device or address",
/*  7 - E2BIG   */ "Arg list too long",
/*  8 - ENOEXEC */ "Exec format error",
/*  9 - EBADF   */ "Bad file number",
/* 10 - ECHILD  */ "No children",
/* 11 - EAGAIN  */ "No more processes",
/* 12 - ENOMEM  */ "Not enough core",
/* 13 - EACCES  */ "Permission denied",
/* 14 - EFAULT  */ "Bad address",
/* 15 - ENOTBLK */ "Block device required",
/* 16 - EBUSY   */ "Mount device busy",
/* 17 - EEXIST  */ "File exists",
/* 18 - EXDEV   */ "Cross-device link",
/* 19 - ENODEV  */ "No such device",
/* 20 - ENOTDIR */ "Not a directory",
/* 21 - EISDIR  */ "Is a directory",
/* 22 - EINVAL  */ "Invalid argument",
/* 23 - ENFILE  */ "File table overflow",
/* 24 - EMFILE  */ "Too many open files",
/* 25 - ENOTTY  */ "Not a typewriter",
/* 26 - ETXTBSY */ "Text file busy",
/* 27 - EFBIG   */ "File too large",
/* 28 - ENOSPC  */ "No space left on device",
/* 29 - ESPIPE  */ "Illegal seek",
/* 30 - EROFS   */ "Read only file system",
/* 31 - EMLINK  */ "Too many links",
/* 32 - EPIPE   */ "Broken pipe",
/* 33 - EDOM    */ "Math arg out of domain of func",
/* 34 - ERANGE  */ "Math result not representable",
/* 35 - ENOMSG  */ "No message of desired type",
/* 36 - EIDRM   */ "Identifier removed"
        };
static int sys_ner = 37;

static char *errorstr(int errnum) 
{
        static char buffer[50];
        if (errnum < 0 || errnum >= sys_ner)  {
            sprintf(buffer, "ERR-UNKNOWN (%d)", errnum);
            return(buffer);
        }
        return(errlist[errnum]);
}
/*
**  logging support
*/
void log(char *str, ...)
{
    va_list ap;
    char buf[1024];

    va_start(ap, str);
    vsprintf(buf, str, ap);
    if (usesyslog)
        syslog(LOG_ERR, "SMTPclient: %s", buf);
    else
        fprintf(stderr, "SMTPclient: %s\n", buf);
    va_end(ap);
    return;
}


/*
**  examine message from server 
*/
void get_response(void)
{
    char buf[BUFSIZ];

    while (fgets(buf, sizeof(buf), rfp)) {
        buf[strlen(buf)-1] = 0;
        dprintf("%s --> %s\n", mailhost, buf);
        if (!isdigit(buf[0]) || buf[0] > '3') {
            log("unexpected reply: %s", buf);
            exit(1);
        }
        if (buf[4] != '-')
            break;
    }
    return;
}

/*
**  say something to server and check the response
*/
void chat(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(sfp, fmt, ap);
    va_end(ap);
  
    va_start(ap, fmt);
    dprintf("%s <-- ", mailhost);
    dvprintf(fmt, ap);
    va_end(ap);

    fflush(sfp);
    get_response();
}


/*
**  main procedure
*/


int main(int argc, char **argv)
{
    char buf[BUFSIZ];
    char my_name[BUFSIZ];
    struct sockaddr_in sin;
    struct hostent *hp;
    int s;
    int r;
    struct passwd *pwd;
    char *cp;

    /* 
     *  Go away when something gets stuck.
     */
     alarm(60);
    /*
     *  Determine SMTP server
     */
    if (argc > 1) {
	mailhost = argv[1];
    }
    if (mailhost == NULL) {
        if ((cp = getenv("SMTPSERVER")) != NULL)
            mailhost = cp;
        else
            mailhost = "localhost";
    }

    /*
     *  Find out my own host name for HELO; 
     *  if possible, get the FQDN.
     */
    if (gethostname(my_name, sizeof(my_name) - 1) < 0) {
        log("gethostname: %s", errorstr(errno));
        exit(1);
    }
    if ((hp = gethostbyname(my_name)) == NULL) {
        log("%s: unknown host\n", my_name);
        exit(1);
    }
    strcpy(my_name, hp->h_name);

    /*
     *  Determine from address.
     */
    if (from_addr == NULL) {
        if ((pwd = getpwuid(getuid())) == 0) {
            sprintf(buf, "userid-%d@%s", getuid(), my_name);
        } else {
            sprintf(buf, "%s@%s", pwd->pw_name, my_name);
        }
        from_addr = strdup(buf);
    }

    /*
     *  Connect to smtp daemon on mailhost.
     */
    if ((hp = gethostbyname(mailhost)) == NULL) {
        log("%s: unknown host\n", mailhost);
        exit(1);
    }
    if (hp->h_addrtype != AF_INET) {
        log("unknown address family: %d", hp->h_addrtype);
        exit(1);
    }
    memset((char *)&sin, 0, sizeof(sin));
    memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
    sin.sin_family = hp->h_addrtype;
    sin.sin_port = htons(mailport);
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log("socket: %s", errorstr(errno));
        exit(1);
    }
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        log("connect: %s", errorstr(errno));
        exit(1);
    }
    if ((r = dup(s)) < 0) {
        log("dup: %s", errorstr(errno));
        exit(1);
    }
    if ((sfp = fdopen(s, "w")) == 0) {
        log("fdopen: %s", errorstr(errno));
        exit(1);
    }
    if ((rfp = fdopen(r, "r")) == 0) {
        log("fdopen: %s", errorstr(errno));
        exit(1);
    }

    /* 
     *  Give out SMTP headers.
     */
    get_response(); /* banner */
    chat("HELO %s\r\n", my_name);
/*
    chat("MAIL FROM: <%s>\r\n", from_addr);

    for (i = optind; i < argc; i++)
        chat("RCPT TO: <%s>\r\n", argv[i]);
    if (cc_addr)
        chat("RCPT TO: <%s>\r\n", cc_addr);
*/
	
   	while (fgets(buf, sizeof(buf) - 3, stdin)) {
		int l;
		if (buf[0] == '\n') break;
		l = strlen(buf)-1;
		buf[l++] = '\r';
		buf[l++] = '\n';
		buf[l++] = '\0';
		chat(buf);
	}
    	chat("DATA\r\n");

    /*
     *  Give out Message body.
     */
    while (fgets(buf, sizeof(buf), stdin)) {
       buf[strlen(buf)-1] = 0;
       if (strcmp(buf, ".") == 0) { /* quote alone dots */
            fprintf(sfp, "..\r\n");
       } else { /* pass thru mode */
            fprintf(sfp, "%s\r\n", buf);
       }
       alarm(15);
    }

    /* 
     *  Give out SMTP end.
     */
    alarm(60);
    chat(".\r\n");
    chat("QUIT\r\n");

    /* 
     *  Die gracefully ...
     */
    exit(0);
}

/*EOF*/
