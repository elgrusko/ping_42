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
#include <errno.h>
#include <time.h>
#include <sys/time.h>
# include "libft.h"

#define PACKET_SIZE 84 // ip header (20 bytes) + icmp header (8 bytes) + data
// PACKET_SIZE varie en fonction de l'option -s Donc pas sur qu'on s'en serve vraiment

typedef struct	s_env{
	int v;
	int h;
	int c;
	int s;
	int i;
	int q;
	int n;
	int ttl;
	int interval;
	int pckt_loss;
	int pckt_recv;
	int nb_errors;
	char err;
	char *dest;
	char *rev_dns;
	char *addrstr;
	char *addrstr6;
	char *fqdn;
	double min;
	double max;
	double avg;
	double *list;
	double mdev;
	struct timeval  begin;

}				t_env;

typedef struct 	s_icmphdr{
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
void 	reverse_dns_lookup(t_env *env);
void 	free_rcv(t_rcvhdr rcv_hdr);

# endif
