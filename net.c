#include <arpa/inet.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef unsigned _BitInt(128) uint128_t;

int main(void) {
  struct ifaddrs *ifaddr;
  int family, nameinfo_fd, netmaskinfo_fd, prefixlen;
  struct in_addr host_n, netmask_n, broadcast_n;
  struct in6_addr host6_n, netmask6_n, broadcast6_n;
  char host[NI_MAXHOST], netmask[NI_MAXHOST], broadcast[NI_MAXHOST];

  if (getifaddrs(&ifaddr) == -1) {
    printf("Error getting interface addresses\n");
    return -1;
  }

  for (struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }

    family = ifa->ifa_addr->sa_family;

    printf("%-8s %s (%d)\n", ifa->ifa_name,
           (family == AF_PACKET)  ? "AF_PACKET"
           : (family == AF_INET)  ? "AF_INET"
           : (family == AF_INET6) ? "AF_INET6"
                                  : "???",
           family);

    if (family == AF_INET || family == AF_INET6) {
      nameinfo_fd =
          getnameinfo(ifa->ifa_addr,
                      (family == AF_INET) ? sizeof(struct sockaddr_in)
                                          : sizeof(struct sockaddr_in6),
                      host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
      netmaskinfo_fd =
          getnameinfo(ifa->ifa_netmask,
                      (family == AF_INET) ? sizeof(struct sockaddr_in)
                                          : sizeof(struct sockaddr_in6),
                      netmask, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

      inet_pton((family == AF_INET) ? AF_INET : AF_INET6, host,
                (family == AF_INET) ? &host_n : (struct in_addr *)&host6_n);
      inet_pton((family == AF_INET) ? AF_INET : AF_INET6, netmask,
                (family == AF_INET) ? &netmask_n
                                    : (struct in_addr *)&netmask6_n);

      uint128_t mask;
      if (family == AF_INET) {
        mask = ntohl(netmask_n.s_addr);
      } else {
        mask = 0;
        for (int i = 0; i < 16; i++) {
          mask <<= 8;
          mask |= netmask6_n.s6_addr[i];
        }
      }

      prefixlen = 0;
      while (mask != 0) {
        prefixlen += mask & 1;
        mask >>= 1;
      }

      if (family == AF_INET) {
        broadcast_n.s_addr = host_n.s_addr | ~netmask_n.s_addr;
      } else {
        for (int i = 0; i < 16; i++) {
          broadcast6_n.s6_addr[i] = host6_n.s6_addr[i] | ~netmask6_n.s6_addr[i];
        }
      }

      inet_ntop((family == AF_INET) ? AF_INET : AF_INET6,
                (family == AF_INET) ? &broadcast_n
                                    : (struct in_addr *)&broadcast6_n,
                broadcast, NI_MAXHOST);

      if (nameinfo_fd != 0) {
        printf("Error getting name info\n");
        return -1;
      }

      printf("\t\tinet%s: <%s/%d>\n\t\tnetmask: <%s>\n\t\tbroadcast: <%s>\n",
             (family == AF_INET) ? "" : "6", host, prefixlen, netmask,
             broadcast);
    } else if (family == AF_PACKET && ifa->ifa_data != NULL) {
      struct rtnl_link_stats *stats = ifa->ifa_data;

      printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
             "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
             stats->tx_packets, stats->rx_packets, stats->tx_bytes,
             stats->rx_bytes);
    }
  }
  return 0;
}
