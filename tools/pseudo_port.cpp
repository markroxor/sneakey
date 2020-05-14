#include <pcap.h>
#include <stdio.h>
#include <libnet.h>
#include <libnet/libnet-functions.h>
#include <netinet/if_ether.h> 
#include <netinet/tcp.h>
#include "utils.h"
#define MAX_OPEN_PORTS

/*
sudo apt-get install iptables-persistent
sudo iptables-save >| backup.txt
# sudo iptables -P INPUT ACCEPT
# https://serverfault.com/questions/157375/reject-vs-drop-when-using-iptables
sudo iptables -A INPUT -p tcp --dport 65000 -j DROP # stops sending RST tcp packet
sudo netfilter-persistent save && sudo netfilter-persistent reload  # 
sudo iptables-restore < backup.txt

nc 127.0.0.1 65000
*/
void callback(u_char *device, const struct pcap_pkthdr * pkt_hdr, const u_char *packet)
{
    // receives a packet and sends a corresponding ACK/SYN packet.
    // libnet_t *lc = (libnet_t *) lc_char;
    struct iphdr_ *ip_hdr = (struct iphdr_ *)(packet + ETHER_HDR_LEN);
    struct tcphdr *tcp_hdr = (struct tcphdr *)(packet + ETHER_HDR_LEN + LIBNET_TCP_H);

    char *err_buf;
    libnet_t *lc = libnet_init(LIBNET_RAW4, (const char *)device, err_buf);
    if(lc == NULL)
    {
        fprintf(stderr, "%s\n", err_buf);
    }

    printf("Source port:%hu Destination port:%hu \n", ntohs(tcp_hdr->th_sport), ntohs(tcp_hdr->th_dport));
    printf("Sequence number:%u Acknowledgment number:%u \n", ntohl(tcp_hdr->th_seq), ntohl(tcp_hdr->th_ack));

    libnet_ptag_t t_ptag = libnet_build_tcp(htons(tcp_hdr->th_dport),
    htons(tcp_hdr->th_sport),
    (tcp_hdr->th_ack),
    (ntohl(tcp_hdr->th_seq) + 1),
    TH_SYN | TH_ACK,
    50, // to identify my packet
    0,
    0,
    LIBNET_IPV4_H,
    NULL,
    0,
    lc,
    0
    );

    libnet_ptag_t i_ptag = libnet_build_ipv4(LIBNET_TCP_H,
    IPTOS_LOWDELAY,
    libnet_get_prand(LIBNET_PRu16),
    0,
    libnet_get_prand(LIBNET_PR8),
    IPPROTO_TCP,
    0,
    *(uint32_t *)(&ip_hdr->daddr),
    *(uint32_t *)(&ip_hdr->saddr),
    NULL,
    0,
    lc,
    0
    );

    libnet_write(lc);

}

void set_packet_filter(pcap_t *p_handler, int ports[MAX_OPEN_PORTS], const char * target_ip_char)
{
    char filter_string[200];
    sprintf(filter_string, "dst host %s and ", target_ip_char);
    strcat(filter_string, "tcp[tcpflags] & tcp-syn !=0 and tcp[tcpflags] & tcp-ack = 0 ");

    int i = 0;
    if (ports[i] != -1)
    {
        char temp_str[20];
        sprintf(temp_str, "and not (dst port %d", ports[i]);
        strcat(filter_string, temp_str);

        while (ports[++i] != -1)
        {
            sprintf(temp_str, " or dst port %d", ports[i]);
            strcat(filter_string, temp_str);
        }
        strcat(filter_string, ")");
    }

    printf("%s\n", filter_string);

    struct bpf_program filter;
    pcap_compile(p_handler, &filter, filter_string, 0, PCAP_NETMASK_UNKNOWN);
    pcap_setfilter(p_handler, &filter);
}

int main(int argc, char ** argv)
{
    // struct in_addr *target_ip = libnet_name_resolve();// "192.168.43.179";
    const char *target_ip_char = "127.0.0.1";
    u_char *device = (u_char *)("lo");

    int ports[MAX_OPEN_PORTS] = {2, -1};

    pcap_t *p_handler = get_default_pcap_handler(0,(char *) device);
    if (p_handler == NULL)
        return 1;

    u_int32_t *target_ip = (uint32_t *) &target_ip_char;    

    set_packet_filter(p_handler, ports, target_ip_char);
    pcap_loop(p_handler, -1, callback, device);
    pcap_close(p_handler);
}