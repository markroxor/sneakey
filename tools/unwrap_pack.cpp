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

int unwrap_network(const u_char *hdr)
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
    printf("Source port:%d Destination port:%d \n", tcp_hdr->th_sport, tcp_hdr->th_dport);
    printf("Sequence number:%d Acknowledgment number:%d \n", tcp_hdr->th_seq, tcp_hdr->th_ack);
    printf("FIN flag:%d\n", (tcp_hdr->th_flags & TH_FIN) & 1);
    printf("SYN Flag:%d\n", (tcp_hdr->th_flags & TH_SYN)%2 == 0);
    printf("RST Flag:%d\n", (tcp_hdr->th_flags & TH_RST)%2 == 0);
    printf("PUSH Flag:%d\n", (tcp_hdr->th_flags & TH_PUSH)%2 == 0);
    printf("ACK Flag:%d\n", (tcp_hdr->th_flags & TH_ACK)%2 == 0);
    printf("URG Flag:%d\n", (tcp_hdr->th_flags & TH_URG)%2 == 0);
    printf("Window:%d\n", tcp_hdr->th_win);
    printf("Sum:%d\n", tcp_hdr->th_sum);
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
    printf("Packet header length is %d\n", header->len);

    unwrap_data_link(packet + processed_hdr_offset);
    processed_hdr_offset += ETHER_HDR_LEN;

    int protocol = unwrap_network(packet + processed_hdr_offset);
    processed_hdr_offset += sizeof(struct iphdr_);
    
    switch(protocol)
    {
        case 6:
            processed_hdr_offset += decode_tcp(packet + processed_hdr_offset);
            break;
        case 17:
            processed_hdr_offset += decode_udp(packet + processed_hdr_offset);
            break;
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
    char * device;
    pcap_t *p_handler;
    pcap_if_t *interfaces, *temp;

    if (pcap_findalldevs(&interfaces, err_buff) == -1)
    {
        printf("Cant find interfaces exiting...");
        return 1;
    }

    printf("Select one from available interfaces - \n");
    char i;
    while(interfaces)
    {
        printf("%s : %s \n", interfaces->name, interfaces->description);
        printf("Select this interface? (y/n)\n");
        scanf("%s", &i);
        if (i=='y')
            break;
        interfaces = interfaces->next;
    }
    if (interfaces)
        printf("You chose %s\n", interfaces->name);
    else
    {
        printf("Your interface not found.. exiting...\n");
        return 1;
    }

    p_handler = pcap_open_live(interfaces->name, BUFSIZ, PROMISCUOUS, 1000, err_buff);
    if(p_handler == NULL)
    {
        printf("Could not open socket on %s the error is : %s, exiting...\n", interfaces->name, err_buff);
        return 1;
    }

    int cnt = 1;
    printf("Waiting for %d packet(s) on interface %s\n", cnt, interfaces->name);
    pcap_loop(p_handler, -1, callback, NULL);

    return 0;
}
