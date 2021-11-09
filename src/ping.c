#include "../includes/ft_ping.h"
int loop = 1;

void handler(){
	loop = 0;
	return ;
}

unsigned short checksum(void *b, int len)
{    unsigned short *buf = b;
    unsigned int sum=0;
    unsigned short result;
  
    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void fill_icmp_hdr(t_icmphdr *icmp, t_env *env, u_int16_t pid, time_t timestamp)
{
    bzero(icmp, sizeof(t_icmphdr));
	icmp->icmp_hdr.type = ICMP_ECHO;
	icmp->icmp_hdr.un.echo.id = ((pid << 8) & 0xff00) | ((pid >> 8) & 0x00ff);
	icmp->icmp_hdr.un.echo.sequence = env->i++;
    icmp->icmp_hdr.un.echo.sequence = ((icmp->icmp_hdr.un.echo.sequence << 8) & 0xff00) | ((icmp->icmp_hdr.un.echo.sequence >> 8) & 0x00ff);
	icmp->timestamp = timestamp;
    icmp->icmp_hdr.checksum = checksum(icmp, sizeof(t_icmphdr));
}

void send_ping(int sock, t_env *env, struct sockaddr_in *ping_addr)
{
	(void)env;
	struct timeval 	tv_out;
	t_icmphdr 		icmp;
	struct timeval 	tv_seq_start;
    u_int16_t       pid;

    pid = getpid();
    tv_out.tv_sec = 1;
    tv_out.tv_usec = 0;
	signal(SIGINT, handler);
    setsockopt(sock, SOL_IP, IP_TTL, &env->ttl, sizeof(env->ttl));                  // set TTL 
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out); // set timeout
    while (loop)
    {
        if (gettimeofday(&tv_seq_start, NULL) == -1)
		exit(42); // gerer erreur normalement
        fill_icmp_hdr(&icmp, env, pid, tv_seq_start.tv_sec);
        if (sendto(sock, &icmp, sizeof(icmp), 0, (struct sockaddr*)ping_addr, sizeof(*ping_addr)) <= 0)
        	printf("\nPacket Sending Failed!\n");
        printf("ICMP packet has been sent.");
        break;
    }
	return ;
}
