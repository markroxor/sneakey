#include<net/if_arp.h> 
#include<libnet.h>
#include<libnet/libnet-functions.h>
#include<string.h>
#include"utils.h"
static libnet_ethernet_hdr eth_hdr;
int main(int argc, char **argv)
{
    const char *device = "enp3s0";
    device = "en0";
    struct in_addr s_addr, d_addr;
    libnet_t *l;
    int len;
    eth_hdr.ether_type = ETHERTYPE_ARP; 

    char s_ip_addr_char[16] = "192.168.0.41";
    char s_eth_addr_char[18] = "30:9c:23:8d:de:94";
    char d_ip_addr_char[16] = "192.168.0.41";
    char d_eth_addr_char[18] = "30:9c:23:8d:de:94";

    char *err_buff;
    l = libnet_init(LIBNET_LINK_ADV, device, err_buff);
    u_int32_t ip_addr_32 = libnet_name2addr4(l, s_ip_addr_char, LIBNET_DONT_RESOLVE);
    u_int8_t *ip_addr_8;

    // if (inet_aton(s_ip_addr_char, &s_addr) != 0)
    //     printf("Error");
    // if (inet_aton(d_ip_addr_char, &d_addr) != 0)
    //     printf("Error");
    ip_addr_8 = (u_int8_t *) &ip_addr_32;
    printf("%d.%d.%d.%d\n", ip_addr_8[0], ip_addr_8[1], ip_addr_8[2], ip_addr_8[3]);
    printf("%s\n", libnet_addr2name4(ip_addr_32, LIBNET_DONT_RESOLVE));
    print_mac(libnet_hex_aton(s_eth_addr_char, &len));
    ip_addr_32 = libnet_get_ipaddr4(l);
    printf("%s\n", libnet_addr2name4(ip_addr_32, LIBNET_DONT_RESOLVE));
libnet_build_icmpv4_echo()
    // ip_addr_8 = (u_int8_t *) &ip_addr_32;
    // printf("%d.%d.%d.%d\n", ip_addr_8[0], ip_addr_8[1], ip_addr_8[2], ip_addr_8[3]);

    return 0;

    // sscanf(s_ip_addr_char, "%02X:%02X:%02X:%02X:%02X:%02X", &eth_hdr.ether_shost[0],
    //         &eth_hdr.ether_shost[1], &eth_hdr.ether_shost[2], &eth_hdr.ether_shost[3],
    //         &eth_hdr.ether_shost[4], &eth_hdr.ether_shost[5]);
    // sscanf(d_ip_addr_char, "%02X:%02X:%02X:%02X:%02X:%02X", &eth_hdr.ether_dhost[0],
    //         &eth_hdr.ether_dhost[1], &eth_hdr.ether_dhost[2], &eth_hdr.ether_dhost[3],
    //         &eth_hdr.ether_dhost[4], &eth_hdr.ether_dhost[5]);
    // libnet_build_arp(ARPHRD_ETHER,
    //         ETHERTYPE_IP,
    //         6,
    //         4,
    //         ARPOP_REQUEST,
    //         eth_hdr.ether_shost,
    //         (uint8_t *)s_addr.s_addr,
    //         eth_hdr.ether_dhost,
    //         (uint8_t *)d_addr.s_addr,
    //         NULL,
    //         0,
    //         l,
    //         0
    //         );
 
    // libnet_build_ethernet(eth_hdr.ether_dhost, eth_hdr.ether_shost, eth_hdr.ether_type, NULL, 0, l, 0);
}