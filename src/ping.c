#include "../includes/ft_ping.h"

int loop = 1;

void handler()
{
	loop = 0;
	return ;
}

double  calcul_mdev(t_env *env){
    double type;
    double square;
    int i;
    type = 0;

    for (i = 0; i < env->pckt_recv; i++){
        square = (env->list[i] - env->avg);
        square = square * square;
        type += square;
    }
    type = type / i;
    return type;
}

// Resolves the reverse lookup of the hostname
void reverse_dns_lookup(t_env *env)
{
    struct sockaddr_in  temp_addr;    
    socklen_t           len;
    char                buf[NI_MAXHOST];
  
    temp_addr.sin_family = AF_INET;
    temp_addr.sin_addr.s_addr = inet_addr(env->addrstr);
    len = sizeof(struct sockaddr_in);
  
    if (getnameinfo((struct sockaddr *)&temp_addr, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD)) 
        return ;
    env->rev_dns = ft_strdup(buf);
}

unsigned short checksum(void *b, int len)
{    
    unsigned short  *buf;
    unsigned int    sum;
    unsigned short  result;
  
    buf = b;
    sum = 0;
    result = 0;
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
    // REPRENDRE POUR L'OPTION S OU REMETTRE LE PADDING EN PADDING[] DNAS LE .H LE PADDING FAIT TOUJOURS 8 EN SIZE
	icmp->icmp_hdr.type = ICMP_ECHO;
	icmp->icmp_hdr.un.echo.id = ((pid << 8) & 0xff00) | ((pid >> 8) & 0x00ff);
	icmp->icmp_hdr.un.echo.sequence = env->i++;
    icmp->icmp_hdr.un.echo.sequence = ((icmp->icmp_hdr.un.echo.sequence << 8) & 0xff00) | ((icmp->icmp_hdr.un.echo.sequence >> 8) & 0x00ff);
	icmp->timestamp = timestamp;
    icmp->icmp_hdr.checksum = checksum(icmp, sizeof(t_icmphdr));
}

void fill_recv_msg(t_rcvhdr *rcv_hdr)
{
    rcv_hdr->buf = malloc(sizeof(unsigned char) * PACKET_SIZE); // pas oublier de free
    rcv_hdr->msg.msg_name = NULL;
    rcv_hdr->msg.msg_iov = rcv_hdr->iov;
    rcv_hdr->msg.msg_iovlen = 1;
    rcv_hdr->msg.msg_flags = 0;
    rcv_hdr->iov[0].iov_base = rcv_hdr->buf;
    rcv_hdr->iov[0].iov_len = PACKET_SIZE;
}

void recv_ping(int sock, t_icmphdr *icmp_sent, struct timeval tv_seq_start, t_env *env)
{
    t_rcvhdr        rcv_hdr;
    int             len;
    int             flags;
    t_rcvmem        *rcv_mem;
    u_int16_t       rcv_id;
    struct timeval 	tv_seq_end;
    double          tv_seq_diff;

    flags = 0;
    bzero(&rcv_hdr, sizeof(t_rcvhdr));
    fill_recv_msg(&rcv_hdr);
    len = recvmsg(sock, &rcv_hdr.msg, flags);
    if (!len)
    {
        free_rcv(rcv_hdr);
        return ;
    }
    rcv_mem = (t_rcvmem*)(rcv_hdr.iov[0].iov_base); // caster le reply dans une structure ip header + icmp header. plus facile pour acceder aux elements
    rcv_id = rcv_mem->icmp_hdr.un.echo.id;
    if (len && rcv_id == icmp_sent->icmp_hdr.un.echo.id) // on check que l'id du icmp reply est le meme que celui du icmp request
    {
        env->pckt_recv += 1;
        gettimeofday(&tv_seq_end, NULL);
        tv_seq_diff = (double)(tv_seq_end.tv_sec - tv_seq_start.tv_sec) * 1000.0 + (double)(tv_seq_end.tv_usec - tv_seq_start.tv_usec) / 1000.0;
        // gere le rtt a la fin du ping (mdev a finir) --> realloc de double *list a chaque fois pour integrer les valeurs de tv_seq_diff
        if (!(env->list))
            env->list = malloc(sizeof(double));
        else
            env->list = realloc(env->list, (sizeof(double) * env->pckt_recv));
        env->list[env->pckt_recv - 1] = tv_seq_diff;
        env->min = (env->min > tv_seq_diff || env->min == 0) ? tv_seq_diff : env->min;
        env->max = (env->max < tv_seq_diff || env->max == 0) ? tv_seq_diff : env->max;
        env->avg += tv_seq_diff;
        if (!env->q)
            (env->rev_dns && !env->n) ? printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.3lf ms\n", env->s + 8, env->rev_dns, env->addrstr, env->i - 1, env->ttl, tv_seq_diff) : 
            printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3lf ms\n", env->s + 8, env->addrstr, env->i - 1, env->ttl, tv_seq_diff); // env->s + 8 (icmp header)
    }
    else
        env->pckt_loss += 1;
    rcv_mem = (t_rcvmem*)(rcv_hdr.iov[0].iov_base);
    free_rcv(rcv_hdr);
}

void    wait_interval(int interval) // sleep() alternative
{
	struct timeval tv_current;
	struct timeval tv_next;

    signal(SIGINT, handler);
	if (gettimeofday(&tv_current, NULL) < 0)
		return ; // gerer erreur exit(42)
	tv_next = tv_current;
	tv_next.tv_sec += interval;
	while ((tv_current.tv_sec < tv_next.tv_sec || tv_current.tv_usec < tv_next.tv_usec) && loop)
	{
		if (gettimeofday(&tv_current, NULL) < 0)
			return ; // gerer erreur exit(42)
	}
}

void print_stats(t_env *env)
{
    int loss_total;
    struct timeval tv;
    double spent_time;

    loss_total = env->pckt_loss;
    if (loss_total > 0)
        loss_total = ((env->i - 1) / env->pckt_loss) * 100;
    printf("--- %s ping statistics ---\n", env->dest);
    gettimeofday(&tv, NULL);
    spent_time = ((double)(tv.tv_usec - env->begin.tv_usec) / 1000000 +(double)(tv.tv_sec - env->begin.tv_sec)) * 1000;
    if (env->nb_errors)
        printf("%d packets transmitted, %d received, +%d errors, %d%% packet loss, time %.fms\n", env->i - 1, env->pckt_recv, env->nb_errors, loss_total, spent_time);
    else
        printf("%d packets transmitted, %d received, %d%% packet loss, time %.fms\n", env->i - 1, env->pckt_recv, loss_total, spent_time);
    env->avg = env->avg / (env->pckt_recv - env->pckt_loss);
    env->mdev = calcul_mdev(env);
    if (!env->nb_errors)
        printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", env->min, env->avg, env->max, env->mdev);
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
    while (loop){
        if (gettimeofday(&tv_seq_start, NULL) == -1){ // on sort de la fonction ? Pas de probleme avec un return ; tout est free apres
            return ; // gerer erreur normalement
        }
        fill_icmp_hdr(&icmp, env, pid, tv_seq_start.tv_sec);
        if (sendto(sock, &icmp, sizeof(icmp), 0, (struct sockaddr*)ping_addr, sizeof(*ping_addr)) <= 0)
        	printf("\nPacket Sending Failed!\n");
        if (errno == 11)
        {
            printf("From %s icmp_seq=%d Destination Host Unreachable\n", env->addrstr, env->i);
            env->nb_errors++;
        }
        recv_ping(sock, &icmp, tv_seq_start, env);
        wait_interval(env->interval);
    }
    print_stats(env);
	return ;
}