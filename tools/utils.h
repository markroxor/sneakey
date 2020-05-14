/* Header structures are copied from official source and modified as needed. */
// #include<stdio.h>
// #include<pcap.h>
// //#include<arpa/inet.h>
// #include<netinet/if_ether.h> 
// #include<netinet/ip.h>	
// #include<netinet/udp.h>
// #include<netinet/tcp.h>
#define ETH_ALEN 6
#define ETH_HLEN 14
#define IP_ADDR_LEN 4
struct ethhdr_ { 
    unsigned char h_dest[ETH_ALEN];
    unsigned char h_source[ETH_ALEN];
    unsigned short h_proto;
};

struct iphdr_ {
#if __BYTE_ORDER == __LITTLE_ENDIAN 
    unsigned int ihl:4; 
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN 
    unsigned int version:4; 
    unsigned int ihl:4;
#else 
# error "Please fix <bits/endian.h>" 
#endif 
    u_int8_t tos; 
    u_int16_t tot_len; 
    u_int16_t id;
    u_int16_t frag_off; 
    u_int8_t ttl; 
    u_int8_t protocol; 
    u_int16_t check; 
    struct in_addr saddr; 
    struct in_addr daddr; 
};

void print_mac(const unsigned char ether_addr[ETH_ALEN])
{
    for(int i=0; i< ETHER_ADDR_LEN; i++)
    {
        printf("%02x", ether_addr[i]);
        if (i!=ETHER_ADDR_LEN-1)
            printf(":");
    }
    printf("\n");
}
void print_ip_addr(const unsigned char ip_addr[IP_ADDR_LEN])
{
    for(int i=0; i< IP_ADDR_LEN; i++)
    {
        printf("%d", ip_addr[i]);
        if (i!=IP_ADDR_LEN-1)
            printf(".");
    }
    printf("\n");
}

void banner_print()
{
    printf("\n");
    for(int i=0;i<100;i++) printf("#");
    printf("\n");
}

pcap_t * get_default_pcap_handler(int promiscuous, char *device)
{
    char err_buff[PCAP_ERRBUF_SIZE];

    if(device==NULL)
    {
        pcap_if_t *interfaces;

        if (pcap_findalldevs(&interfaces, err_buff) == -1)
        {
            fprintf(stderr, "Cant find interfaces exiting...");
            return NULL;
        }

        printf("Select one from available interfaces - \n");
        char i;
        while(interfaces)
        {
            printf("%s : %s \n", interfaces->name, interfaces->description);
            printf("Select this interface? (y/n)\n");
            // scanf("%s", &i);
            break;
            interfaces = interfaces->next;
        }
        if (interfaces)
            printf("You chose %s\n", interfaces->name);
        else
        {
            fprintf(stderr, "Your interface not found.. exiting...\n");
            return NULL;
        }
        device = interfaces->name;
    }

    int timeout_ms = 1000;
    return pcap_open_live(device, BUFSIZ, promiscuous, timeout_ms, err_buff);
}