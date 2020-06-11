/*
    C ECHO client example using sockets
*/
#include <stdio.h>      //printf
#include <string.h>     //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>  //inet_addr
#include <netinet/tcp.h>
#include <sys/time.h>
#include <unistd.h>

union val {
    int i_val;
    long l_val;
    struct linger linger_val;
    struct timeval timeval_val;
} val;
static char *sock_str_flag(union val *, int);
static char *sock_str_int(union val *, int);
static char *sock_str_linger(union val *, int);
static char *sock_str_timeval(union val *, int);
struct sock_opts
{
    const char *opt_str;
    int opt_level;
    int opt_name;
    char *(*opt_val_str)(union val *, int);
} sock_opts[] = {
    {"SO_BROADCAST", SOL_SOCKET, SO_SNDBUF, sock_str_flag},
    {"SO_DEBUG", SOL_SOCKET, SO_DEBUG, sock_str_flag},
    {"SO_DONTROUTE", SOL_SOCKET, SO_DONTROUTE, sock_str_flag},
    {"SO_ERROR", SOL_SOCKET, SO_ERROR, sock_str_int},
    {"SO_KEEPALIVE", SOL_SOCKET, SO_KEEPALIVE, sock_str_flag},
    {"SO_OOBINLINE", SOL_SOCKET, SO_OOBINLINE, sock_str_flag},
    {"SO_SNDBUF", SOL_SOCKET, SO_SNDBUF, sock_str_int},
    {"SO_RCVLOWAT", SOL_SOCKET, SO_RCVLOWAT, sock_str_int},
    {"SO_SNDLOWAT", SOL_SOCKET, SO_SNDLOWAT, sock_str_int},
    {"SO_SNDTIMEO", SOL_SOCKET, SO_SNDTIMEO, sock_str_timeval},
    {"SO_REUSEADDR", SOL_SOCKET, SO_REUSEADDR, sock_str_flag},
#ifdef SO_REUSEADDR
    {"SO_REUSEPORT", SOL_SOCKET, SO_REUSEPORT, sock_str_flag},
#else
    {"SO_REUSEPORT", 0, 0, NULL},
#endif
    {"SO_TYPE", SOL_SOCKET, SO_TYPE, sock_str_int},
    {"SO_LINGER", SOL_SOCKET, SO_LINGER, sock_str_linger},
    {"SO_RCVTIMEO", SOL_SOCKET, SO_RCVTIMEO, sock_str_timeval},
    {"SO_RCVBUF", SOL_SOCKET, SO_RCVBUF, sock_str_int},
    {"SO_ERROR", SOL_SOCKET, SO_ERROR, sock_str_int},
    {NULL, 0, 0, NULL}};

int main(int argc, char *argv[])
{
    int sock, sock_option;
    struct sockaddr_in server;
    size_t j;
    socklen_t len;
    struct sock_opts *ptr;
    char message_key[500], server_reply[2000],message_value[500],message[1000];
    int fd;

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8880);

    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    for (ptr = sock_opts; ptr->opt_str != NULL; ptr++)
    {
        if (ptr->opt_val_str == NULL)
            printf("(undefined) \n");
        else
        {
            switch (ptr->opt_level)
            {
            case SOL_SOCKET:
            case IPPROTO_IP:
            case IPPROTO_TCP:
                fd = socket(AF_INET, SOCK_STREAM, 0);
                break;
#ifdef IPV6
            case IPPROTO_IPV6:
                fd = socket(AF_INET6, SOCK_STREAM, 0);
                break;
#endif
#ifdef IPPROTO_SCTP
            case IPPROTO_SCTP:
                fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
                break;
#endif

            default:
                perror("cannot create fd");
            }
            len = sizeof(val);
            if (getsockopt(fd, ptr->opt_level, ptr->opt_name, &val, &len) == -1)
            {
                perror("getsockopt Error");
            }
            else
            {
                // printf("default = %s\n", (*ptr->opt_val_str)(&val, len));
                
                strcpy(message_key, ptr->opt_str);
                strcpy(message_value, (*ptr->opt_val_str)(&val, len));
                snprintf(message,sizeof(message),"%s=%s",message_key,message_value);
                // puts(message_key);
                puts(message);
                puts("--------------");
                write(sock, message, strlen(message));
                // if (send(sock, "Sravani", strlen(message), 0) < 0)
                // {
                //     puts("Send failed");
                //     return 1;
                // }
                // sleep(3);
            }
        }
    }
    close(sock);
    return 0;
}

static char strres[128];
static char *
sock_str_flag(union val *ptr, int len)
{
    if (len != sizeof(int))
        snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
    else
    {
        snprintf(strres, sizeof(strres), "%s", (ptr->i_val == 0) ? "off" : "on");
    }
    return (strres);
}

static char *
sock_str_int(union val *ptr, int len)
{
    if (len != sizeof(int))
        snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
    else
    {
        snprintf(strres, sizeof(strres), "%d", ptr->i_val);
    }
    return (strres);
}

static char *
sock_str_linger(union val *ptr, int len)
{
    // if (len != sizeof(int))
    //     snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
    // else
    // {
    snprintf(strres, sizeof(strres), " %d %d", ptr->linger_val.l_linger, ptr->linger_val.l_onoff);
    //  snprintf(strres, sizeof(strres)," %d",ptr->linger_val.l_linger );
    // snprintf(strres, sizeof(strres)," %d",ptr->linger_val.l_onoff );
    // }
    return (strres);
}

static char *
sock_str_timeval(union val *ptr, int len)
{
    // if (len != sizeof(int))
    //     snprintf(strres, sizeof(strres), "size (%d) not sizeof(int)", len);
    // else
    // {
    snprintf(strres, sizeof(strres), "%ld  %ld", ptr->timeval_val.tv_sec, ptr->timeval_val.tv_usec);
    // printf("%ld.%06ld\n", ptr->timeval_val.tv_sec, ptr->timeval_val.tv_usec);

    // }
    return (strres);
}