#include "../includes/ft_ping.h"

void free_rcv(t_rcvhdr rcv_hdr){
	if (rcv_hdr.buf)
		free(rcv_hdr.buf);

	return ;
}

void free_env(t_env env){
	if (env.dest)
		free(env.dest);
	if (env.addrstr)
		free(env.addrstr);
	if (env.addrstr6)
		free(env.addrstr6);
	if (env.fqdn)
		free(env.fqdn);
	if (env.list)
		free(env.list);
	return ;
}

void fill_dest(char *s, t_env *env){
	if (env->dest){
		free(env->dest);
		env->dest = NULL;
	}
	env->dest = ft_strdup(s);
	return ;
}

void	print_error(char c, int i){
	if (!i)
		printf("ping: invalid option -- '%c'\n", c);
	printf("\nUsage\n  ping [options] "
	"<destination>\n\nOptions:\n  <destination>      "
	"dns name or ip address\n  -c <count>         stop after <count> replies\n  -h                 print help and exit\n  -i <interval>"
	"      seconds between sending each packets\n  -n                 no dns name resolution\n  -q                 quiet output\n  -t <ttl>           define time to live\n  -v                 "
	"verbose output\n\nFor "
	"more details see ping(8)\n");
	return ;
}

void init_env(t_env *env){

	env->v = 0;
	env->h = 0;
	env->c = 0;
	env->s = 56;
	env->i = 1;
	env->q = 0;
	env->n = 0;
	env->interval = 1;
	env->pckt_loss = 0;
	env->pckt_recv = 0;
	env->rev_dns = NULL;
	env->ttl = 64;
	env->err = '\0';
	env->dest = NULL;
	env->addrstr = NULL;
	env->addrstr6 = NULL;
	env->fqdn = NULL;
	env->list = NULL;
	env->min = 0;
	env->max = 0;
	env->avg = 0;
	env->mdev = 0;
	gettimeofday(&env->begin, NULL);
}

int  fill_env(t_env *env, char **av){
	int i;
	int y;

	init_env(env);
	for (i = 1; av[i] != NULL; i++){
		if (av[i][0] == '-'){
			for (y = 1; av[i][y] != '\0'; y++){
				if (av[i][y] == 'v')
					env->v = 1;
				else if (av[i][y] == 'h'){
					env->h = 1;
					return (1);
				}
				else if (av[i][y] == 'q' || av[i][y] == 'n'){
					av[i][y] == 'q' ? (env->q = 1) : (env->n = 1);
					break ;
				}
				else if (av[i][y] == 'c' || av[i][y] == 'i'){
					i++;
					if (ft_strisdigit(av[i])){
						av[i - 1][y] == 'c' ? (env->c = ft_atoi(av[i])) :
						(env->interval = ft_atoi(av[i]));
						break ;
					}
					else{
					printf("ping: invalid argument: '%s'\n", av[i]);
					return (2);
					}
				}
				else if (av[i][y] == 't'){
					i++;
					if (ft_strisdigit(av[i])){
						env->ttl = ft_atoi(av[i]);
						break ;
					}
					else{
					printf("ping: invalid argument: '%s'\n", av[i]);
					return (2);
					}
				}
				else{
					env->err = av[i][y];
					return (1);
				}
			}
		}
		else{
			fill_dest(av[i], env);
		}
	}
	return (0);
}

int		main(int ac, char **av){
	int fill;
	t_env env;
	// VERIFIER DURANT QUEL PHASE DU PING IL Y A UNE ERREUR ...
	
	if (getuid() != 0){
		printf("This program sends raw socket, you must be root or sudoers to"
		"use it\n");
		return EXIT_FAILURE;
	}
	if ((fill = fill_env(&env, av)) != 0){
		if (fill == 1)
			print_error(env.err, env.h);
		free_env(env);
		return (EXIT_FAILURE);
	}
	else{
		printf("env->c - %d\n", env.c);
		if (env.dest == NULL || ac == 1){
			printf("ping: usage error: Destination address required\n");
			free_env(env);
			return EXIT_FAILURE;
		}
		else if ((lookup_dest(&env)) || reverse_lookup(&env)){
			free_env(env);
			return EXIT_FAILURE;
		}
		else if (init_ping(&env)){
				printf("A problem occured during the initialization of the socket\n");
				free_env(env);
				return EXIT_FAILURE;
		}
	}
	free_env(env);
	return (0);
}