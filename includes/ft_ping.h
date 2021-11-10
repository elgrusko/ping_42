#ifndef FT_PING_H
# define FT_PING_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
# include "libft.h"

#define PACKET_SIZE 84 // ip header (20 bytes) + icmp header (8 bytes) + data

typedef struct	s_env{
	int v;
	int h;
	int s;
	int i;
	int ttl;
	int interval;
	char err;
	char *dest;
	char *addrstr;
	char *addrstr6;
	char *fqdn;
}				t_env;

typedef struct 	s_icmhdr{
	struct icmphdr 	icmp_hdr;
	time_t			timestamp;
	unsigned char 	padding[48];
}			   	t_icmphdr;

typedef struct 	s_rcvhdr{
	struct msghdr 	msg;
	struct iovec 	iov[1];
	unsigned char 	*buf;
}			   	t_rcvhdr;

typedef struct 	s_rcvmem{
	struct ip 		ip_hdr;
	struct icmphdr 	icmp_hdr;
}				t_rcvmem;

int		lookup_dest(t_env *env);
void 	send_ping(int sock, t_env *env, struct sockaddr_in *addr_con);
int		init_ping(t_env *env);
int		reverse_lookup(t_env *env);
void 	print_bytes(void *ptr, int size);
# endif
