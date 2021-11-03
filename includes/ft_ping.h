#ifndef FT_PING_H
# define FT_PING_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/time.h>
# include <arpa/inet.h>
# include <signal.h>
# include "libft.h"


typedef struct	s_env{
	int v;
	int h;
	int s;
	int i;
	int ttl;
	char err;
	char *dest;
	char *addrstr;
	char *addrstr6;
	char *fqdn;
}				t_env;


int		lookup_dest(t_env *env);
void	send_ping(int sock, t_env *env);
int		init_ping(t_env *env);
int		reverse_lookup(t_env *env);
# endif
