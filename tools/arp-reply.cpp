// #include<net/if_arp.h> 
#include<libnet.h>
// #include<libnet/libnet-functions.h>
#include"utils.h"
static libnet_ethernet_hdr eth_hdr;
int main(int argc, char **argv)
{
    const char *device = "wlp2s0";
    char targ_ip_addr_char[16] = "127.0.0.1";
    char src_ip_addr_char[16] = "192.168.0.149";
    char targ_hw_addr_char[18] = "ff:ff:ff:ff:ff:ff";  // broadcast
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
    uint8_t * targ_hw_addr = libnet_hex_aton(targ_hw_addr_char, &len);
    targ_ip_addr = libnet_name2addr4(lc, targ_ip_addr_char, LIBNET_DONT_RESOLVE);

    // u_int8_t* ip_addr_p = (u_int8_t*)(&src_ip_addr);
    // for (int i=0; i<4; i++)
    //     printf("%d.", ip_addr_p[i]);

    if (src_ip_addr == -1 || targ_ip_addr == -1 || targ_hw_addr == NULL || src_hw_addr == NULL)
    {
        fprintf(stderr, "err");
        return 1;
    }

    // libnet_ptag_t a_ptag = libnet_autobuild_arp(ARPOP_REPLY, src_hw_addr->ether_addr_octet, (uint8_t *)&src_ip_addr,
    //                                             targ_hw_addr, (uint8_t *)&targ_ip_addr, lc);
    libnet_ptag_t a_ptag = libnet_autobuild_arp(ARPOP_REPLY, src_hw_addr->ether_addr_octet, (uint8_t *)&src_ip_addr,
                                                src_hw_addr->ether_addr_octet, (uint8_t *)&targ_ip_addr, lc);
    if(a_ptag == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
        return 1;
    }

    libnet_ptag_t e_ptag = libnet_autobuild_ethernet(targ_hw_addr, ETHERTYPE_ARP, lc);
    if(e_ptag == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
        return 1;
    }

    uint8_t *packet;
    uint32_t packet_size;
    if (libnet_adv_cull_packet(lc, &packet, &packet_size) == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
    }
    printf("%d", packet_size);

    if (libnet_adv_write_link(lc, packet, packet_size) == -1) {
        fprintf(stderr, "%s\n",libnet_geterror(lc));
    }

    libnet_adv_free_packet(lc, packet);
    libnet_destroy(lc);
    return 0;

}