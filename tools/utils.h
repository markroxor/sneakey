/* Header structures are copied from official source and modified as needed. */

#define ETH_ALEN 6
#define ETH_HLEN 14
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

void banner_print()
{
    printf("\n");
    for(int i=0;i<100;i++) printf("#");
    printf("\n");
}
