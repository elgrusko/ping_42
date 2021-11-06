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

void send_ping(int sock, t_env *env, struct sockaddr_in *addr_con){
	(void)env;
	struct timeval tv_out;
	struct 	icmphdr pckt;
	tv_out.tv_sec = 1;
    tv_out.tv_usec = 0;
	signal(SIGINT, handler);
	bzero(&pckt, sizeof(pckt));
		
	pckt.type = ICMP_ECHO;
	pckt.un.echo.id = getpid();
	pckt.un.echo.sequence = env->s;
	pckt.checksum = checksum(&pckt, sizeof(pckt));
	if (setsockopt(sock, SOL_IP, IP_TTL, &env->ttl, sizeof(env->ttl)) != 0){
		printf("Setting option to TTL failed\n");
		return ;
	}
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);
	while (loop){
		// REPRENDRE ICI POUR GERER L'ENVOI ET LA RECEPTION DES REQUETES ICMP
		int res = sendto(sock, &pckt, sizeof(pckt), 0, (struct sockaddr*)addr_con, sizeof(*addr_con)); // variable res juste pour checker le nombre de bytes envoyes
		printf("res %d\n", res);
		exit(42); // temporaire
	}
	return ;
}
