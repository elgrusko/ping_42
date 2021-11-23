#include "../includes/ft_ping.h"

void print_bytes(void *ptr, int size) // pour afficher le contenu d'une zone memoire (genre la trame)
{
    unsigned char *p = ptr;
    int i;
    for (i=0; i<size; i++) {
        if (i > 1 && i % 16 == 0)
            printf("\n");
        printf("%02hhX ", p[i]);
    }
    printf("\n");
}

int resolve_ip(char *addr_host, struct sockaddr_in *addr_con) // IP string vers IP decimale (utilisable), et hostname vers IP decimale
{
	struct hostent 	*host_entity;
	
	if ((host_entity = gethostbyname(addr_host)) == NULL)
		return (-1);
	(*addr_con).sin_family = host_entity->h_addrtype;
	(*addr_con).sin_port = htons(0);
	(*addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr;
	return (0);
}

int init_ping(t_env *env)
{
	int 	sock;
	struct 	sockaddr_in addr_con;

	if (resolve_ip(env->addrstr, &addr_con) != 0)
	{
		printf("ft_ping: %s: unknown IP or hostname\n", env->addrstr);
		exit(1);
	}
	printf("PING %s (%s) %d(%d) bytes of data.\n", env->dest, env->addrstr, env->s, env->s + 28);
	reverse_dns_lookup(env);
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock < 0)
		return (1);
	send_ping(sock, env, &addr_con);
	// Ici la struct rcv_hdr sera free dans tous les cas et la struct
	// sera free dans le main au retour de la fonction init_ping
	return (0);
}

int		reverse_lookup(t_env *env){
	(void)env;
	return (0);
}

int		lookup_dest(t_env *env){
	int i;
	char addrstr[100];
	struct addrinfo hints, *res, *result;
	void *ptr;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;
	i = getaddrinfo(env->dest, NULL, &hints, &result);
	if (i != 0){
		printf("ping: %s: Name or service not known\n", env->dest);
		return (2);
	}
	for (res = result; res != NULL; res = res->ai_next){
		inet_ntop(res->ai_family, res->ai_addr->sa_data, addrstr, 100);
		switch (res->ai_family){
			case AF_INET:
				ptr = &((struct sockaddr_in *) res->ai_addr)->sin_addr;
				break ;
			case AF_INET6:
				ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
				break ;
        }
		env->fqdn = res->ai_canonname != NULL ? ft_strdup(res->ai_canonname) : env->fqdn;
		inet_ntop (res->ai_family, ptr, addrstr, 100);
		if (res->ai_family == PF_INET)
			env->addrstr = ft_strdup(addrstr);
		else if (res->ai_family == PF_INET6)
			env->addrstr6 = ft_strdup(addrstr);
	}
	freeaddrinfo(result);
	return (0);
}
