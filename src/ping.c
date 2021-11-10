#include "../includes/ft_ping.h"
int loop = 1;

void handler(){
	loop = 0;
	return ;
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

void recv_ping(int sock, t_icmphdr *icmp_sent)
{
    t_rcvhdr    rcv_hdr;
    int         len;
    int         flags;
    t_rcvmem    *rcv_mem;
    u_int16_t   rcv_id;

    flags = 0;
    bzero(&rcv_hdr, sizeof(t_rcvhdr));
    fill_recv_msg(&rcv_hdr);
    len = recvmsg(sock, &rcv_hdr.msg, flags);
    if (!len)
    {
        printf("error"); // gerer normalement l'erreur
        exit(42);
    }
    rcv_mem = (t_rcvmem*)(rcv_hdr.iov[0].iov_base); // caster le reply dans une structure ip header + icmp header. plus facile pour acceder aux elements
    rcv_id = rcv_mem->icmp_hdr.un.echo.id;
    if (len && rcv_id == icmp_sent->icmp_hdr.un.echo.id){ // on check que l'id du icmp reply est le meme que celui du icmp request
        printf("\nReply received!\n");
    }
    rcv_mem = (t_rcvmem*)(rcv_hdr.iov[0].iov_base);
    print_bytes(rcv_mem, 84); // juste pour print la trame icmp reply
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
        recv_ping(sock, &icmp);
        break;
    }
	return ;
}
