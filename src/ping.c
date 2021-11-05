#include "../includes/ft_ping.h"
int loop = 1;

void handler(){
	loop = 0;
	return ;
}

void send_ping(int sock, t_env *env){
	(void)env;
	struct timeval tv_out;
	tv_out.tv_sec = 1;
    tv_out.tv_usec = 0;

	signal(SIGINT, handler);
	if (setsockopt(sock, SOL_IP, IP_TTL, &env->ttl, sizeof(env->ttl)) != 0){
		printf("Setting option to TTL failed\n");
		return ;
	}
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);
	while (loop){
		// REPRENDRE ICI POUR GERER L'ENVOI ET LA RECEPTION DES REQUETES ICMP
		printf("sending ping \n");
	}
	return ;
}
