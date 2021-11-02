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

// Automatic port number
#define PORT_NO 0
// Gives the timeout delay for receiving packets (in seconds)
#define RECV_TIMEOUT 1

struct ping_variables
{
	int ttl_val;
	int msg_count; 
	int i;
	int addr_len;
	int flag;
	int msg_received_count;
};

void init_ping_variables(struct ping_variables *ping_vars)
{
	bzero(ping_vars, sizeof(struct ping_variables));
	ping_vars->ttl_val = 64;
	ping_vars->msg_count = 0;
	ping_vars->i = 0;
	ping_vars->addr_len = 0;
	ping_vars->flag = 1;
	ping_vars->msg_received_count = 0;
}

// Calculating the Check Sum (fonction pas checkée)
unsigned short checksum(void *b, int len)
{ 
	unsigned short *buf = b;
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

// Performs a DNS lookup (fonction pas checkée)
char *dns_lookup(char *addr_host, struct sockaddr_in *addr_con)
{
	struct hostent *host_entity;
	char *ip=(char*)malloc(NI_MAXHOST*sizeof(char));
	int i;

	if ((host_entity = gethostbyname(addr_host)) == NULL)
	{
		// No ip found for hostname
		return NULL;
	}
	
	//filling up address structure
	strcpy(ip, inet_ntoa(*(struct in_addr *)
						host_entity->h_addr));

	(*addr_con).sin_family = host_entity->h_addrtype;
	(*addr_con).sin_port = htons (PORT_NO);
	(*addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr;

	return ip;
	
}

// Resolves the reverse lookup of the hostname (fonction pas checkée)
char* reverse_dns_lookup(char *ip_addr)
{
	struct sockaddr_in temp_addr;	
	socklen_t len;
	char buf[NI_MAXHOST], *ret_buf;

	temp_addr.sin_family = AF_INET;
	temp_addr.sin_addr.s_addr = inet_addr(ip_addr);
	len = sizeof(struct sockaddr_in);

	if (getnameinfo((struct sockaddr *) &temp_addr, len, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
	{
		printf("Could not resolve reverse lookup of hostname\n");
		return NULL;
	}
	ret_buf = (char*)malloc((strlen(buf) +1)*sizeof(char) );
	strcpy(ret_buf, buf); // copie non controlée...
	return ret_buf;
}

// make a ping request
void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr, char *ping_ip, char *rev_host)
{
	struct ping_variables ping_vars;
	struct sockaddr_in r_addr;
	struct timespec time_start, time_end, tfs, tfe;
	struct icmphdr icmp_hdr;
	struct timeval tv_out;

	tv_out.tv_sec = RECV_TIMEOUT;
	tv_out.tv_usec = 0;
	init_ping_variables(&ping_vars);
	// set socket options at ip to TTL and value to 64,
	// change to what you want by setting ttl_val
	setsockopt(ping_sockfd, SOL_IP, IP_TTL, &ping_vars.ttl_val, sizeof(ping_vars.ttl_val));
	// setting timeout of recv setting
	setsockopt(ping_sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv_out, sizeof tv_out);
	bzero(&icmp_hdr, sizeof(icmp_hdr));
	icmp_hdr.type = ICMP_ECHO;
	icmp_hdr.un.echo.id = getpid();
	icmp_hdr.un.echo.sequence = ping_vars.msg_count++;
	icmp_hdr.checksum = checksum(&icmp_hdr, sizeof(icmp_hdr));
	//send packet
	if (sendto(ping_sockfd, &icmp_hdr, sizeof(icmp_hdr), 0, (struct sockaddr*)ping_addr, sizeof(*ping_addr)) <= 0)
	{
		printf("\nPacket Sending Failed!\n");
		ping_vars.flag=0;
	}
	printf("ICMP packet has been sent.");
}

// Driver Code
int main(int argc, char *argv[])
{
	int sockfd;
	char *ip_addr, *reverse_hostname;
	struct sockaddr_in addr_con;

	ip_addr = dns_lookup(argv[1], &addr_con);
	//socket()
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	//send pings continuously
	send_ping(sockfd, &addr_con, ip_addr, argv[1]);
	close(sockfd);
	return 0;
}

