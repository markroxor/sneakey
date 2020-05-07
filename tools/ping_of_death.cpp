#include<libnet.h>
#include"utils.h"

static libnet_ethernet_hdr eth_hdr;
int main(int argc, char **argv)
{
    const char *device = NULL;
    char targ_ip_addr_char[16] = "192.168.0.1";
    char *err_buff;
    libnet_t *lc;  // libnet context

    lc = libnet_init(LIBNET_RAW4, device, err_buff);  // notice the prot
    if (lc == NULL)
    {
        fprintf(stderr, "%s\n", err_buff);
        return 1;
    }

    uint32_t targ_ip_addr = libnet_name2addr4(lc, targ_ip_addr_char, LIBNET_DONT_RESOLVE);

    if (targ_ip_addr == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
        return 1;
    }

    // setting size of payload alone cause PoD.
    char payload[16] = "abc123";

    libnet_ptag_t icmp_ptag = libnet_build_icmpv4_echo(ICMP_ECHO, 0, 0, 0, 0, \
                            (uint8_t *)payload, sizeof(payload), lc, 0);

    if(icmp_ptag == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
        return 1;
    }

    // The ICMP packet is encapsulated in an IPv4 packet.
    libnet_ptag_t ip_ptag = libnet_autobuild_ipv4(LIBNET_IPV4_H+LIBNET_ICMPV4_ECHO_H+sizeof(payload),
    IPPROTO_ICMP, targ_ip_addr, lc);

    if(ip_ptag == -1)
    {
        fprintf(stderr, "%s\n", libnet_geterror(lc));
        return 1;
    }

    printf("%d\n",libnet_write(lc));

    libnet_destroy(lc);
    return 0;

}