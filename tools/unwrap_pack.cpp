/*
   Code inspired from 
   Hacking: The Art of Exploitation
   Book by Jon Erickson
*/

#include<stdio.h>
#include<pcap.h>
//#include<arpa/inet.h>
#include<netinet/if_ether.h> 
#include<netinet/ip.h>	
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include <libnet.h>
#include <libnet/libnet-functions.h>
#include<net/if_arp.h>
#include"utils.h"

#define PROMISCUOUS 1

void unwrap_data_link(const u_char *hdr)
{
    const struct ethhdr_ *dll_hdr;
    dll_hdr = (const struct ethhdr_ *)hdr;

    printf("Unwrapping data link layer\n");
    printf("Source MAC ID - ");
    print_mac(dll_hdr->h_source);

    printf("Destination MAC ID - ");
    print_mac(dll_hdr->h_dest);
    printf("\n");
}

void unwrap_arp(const u_char *hdr)
{
    const struct arphdr *arp_hdr;
    arp_hdr = (const struct arphdr *) hdr;
    printf("Unwrapping arp header\n");
    printf("Source IP: ");
    // print_ip_addr(arp_hdr->__ar_sip);
    // printf("Source MAC ID - ");
    // print_mac(arp_hdr->__ar_sha);
    // printf("Destination IP: ");
    // print_ip_addr(arp_hdr->__ar_tip);
    // printf("Destination MAC ID - ");
    // print_mac(arp_hdr->__ar_tha);
    printf("\n");
    printf("\n");
}

int unwrap_ip(const u_char *hdr)
{
    const struct iphdr_ *net_hdr;
    net_hdr = (const struct iphdr_ *) hdr;
    printf("Unwrapping network layer\n");
    printf("Source IP: %s\n", inet_ntoa(net_hdr->saddr));
    printf("Destination IP: %s\n", inet_ntoa(net_hdr->daddr));
    printf("Protocol Num: %d\n", net_hdr->protocol);
    printf("\n");
    return net_hdr->protocol;
}

int decode_tcp(const u_char *hdr)
{
    const struct tcphdr *tcp_hdr;
    tcp_hdr = (const struct tcphdr *)hdr;
    printf("Protocol: TCP\n");
    printf("Source port:%hu Destination port:%hu \n", ntohs(tcp_hdr->th_sport), ntohs(tcp_hdr->th_dport));
    printf("Sequence number:%u Acknowledgment number:%u \n", ntohl(tcp_hdr->th_seq), ntohl(tcp_hdr->th_ack));
    printf("FIN flag:%d\n", (tcp_hdr->th_flags & TH_FIN));
    printf("SYN Flag:%d\n", (tcp_hdr->th_flags & TH_SYN));
    printf("RST Flag:%d\n", (tcp_hdr->th_flags & TH_RST));
    printf("PUSH Flag:%d\n", (tcp_hdr->th_flags & TH_PUSH)%2 == 0);
    printf("ACK Flag:%d\n", (tcp_hdr->th_flags & TH_ACK)%2 == 0);
    printf("URG Flag:%d\n", (tcp_hdr->th_flags & TH_URG)%2 == 0);
    printf("Window:%d\n", tcp_hdr->th_win);
    printf("Sum:%0x\n", tcp_hdr->th_sum);
    printf("Urgent pointer:%d\n", tcp_hdr->th_urp);
    printf("\n");
    return 4 * tcp_hdr->th_off;
}

int decode_udp(const u_char *hdr)
{
    const struct udphdr *udp_hdr;
    udp_hdr = (const struct udphdr *)hdr;
    printf("Protocol: UDP\n");
    printf("Source port:%d Destination port:%d \n", ntohs(udp_hdr->uh_sport), ntohs(udp_hdr->uh_dport));
    // printf("Length:%d Checksum:%d\n", ntohs(udp_hdr->uh_ulen), ntohs(udp_hdr->uh_check));
    printf("\n");
    return sizeof(struct udphdr);
}

void callback(u_char *arg, const struct pcap_pkthdr * header, const u_char * packet)
{
    int processed_hdr_offset = 0;
    banner_print();
    printf("Entire packet length is %d\n", header->len);

    unwrap_data_link(packet + processed_hdr_offset);
    processed_hdr_offset += ETHER_HDR_LEN;

    // unwrap_arp(packet + processed_hdr_offset);
    // processed_hdr_offset += sizeof(struct arphdr);

    int protocol = unwrap_ip(packet + processed_hdr_offset);
    processed_hdr_offset += LIBNET_IPV4_H;
    
    switch(protocol)
    {
        // case 1:
        //     processed_hdr_offset += decode_icmp(packet + processed_hdr_offset);
        //     break;
        case 6:
            processed_hdr_offset += decode_tcp(packet + processed_hdr_offset);
            break;
        case 17:
            printf("protocol is udp, skipping unwrap\n");
            // processed_hdr_offset += decode_udp(packet + processed_hdr_offset);
            break;
        default:
            printf("Protocol %d not implemented\n", protocol);
    }

    packet = packet + processed_hdr_offset;
    int packet_len = header->len - processed_hdr_offset;

    printf("PACKET DATA:");
    
    for (int i=0; i< packet_len; i++)
        printf("%c", packet[i]);
    
    printf("EO PACKET DATA\n\n\n");
    
}

int main(int argc, char ** argv )
{
    struct pcap_pkthdr header;
    char err_buff[PCAP_ERRBUF_SIZE];
    u_char *device = (u_char *)("lo");

    pcap_t *p_handler = get_default_pcap_handler(0,(char *) device);
    if (p_handler == NULL)
        return 1;

    int cnt = 1;
    pcap_loop(p_handler, -1, callback, NULL);
    pcap_close(p_handler);  // wont work?

    return 0;
}
