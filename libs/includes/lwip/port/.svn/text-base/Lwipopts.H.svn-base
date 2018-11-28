
/*********************************************************************************************************
**-------------------------------Socket参数配置：Socket options----------------------------
*********************************************************************************************************/
#include <errno.h>
//#define LWIP_PROVIDE_ERRNO

#include <sys/time.h>
#define LWIP_TIMEVAL_PRIVATE 0

#define LWIP_COMPAT_MUTEX               1
#define LWIP_SOCKET                     1
#define LWIP_COMPAT_SOCKETS             1
#define LWIP_NETCONN                    1
#define LWIP_SO_RCVTIMEO                1
#define LWIP_SO_SNDTIMEO                1
#define SO_REUSE                        1
#define LWIP_SO_SNDRCVTIMEO_NONSTANDARD 1
/*
   -----------------------------------------------
   ---------- Platform specific locking ----------
   -----------------------------------------------
*/

#define SYS_LIGHTWEIGHT_PROT            1
#define NO_SYS                          0	 //0:使用操作系统,1:不使用操作系统
#define NO_SYS_NO_TIMERS                1

/*---------- ARP options ----------*/
#define ARP_TABLE_SIZE          16

/*
   --------------------------------
   ---------- IP options ----------
   --------------------------------
*/
#define IP_FRAG_MAX_MTU                 1500


/*---------- TCP options ----------*/
#define TCP_QUEUE_OOSEQ         1
#define TCP_MSS                 (IP_FRAG_MAX_MTU - 40)	/* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */
#define TCP_WND                 (1*TCP_MSS)
#define TCP_SND_BUF             (4*TCP_MSS)
#define TCP_SND_QUEUELEN        128
#define TCP_LISTEN_BACKLOG       1

/*---------- RAW options ----------*/
#define LWIP_RAW                1


/* ---------- Memory options ---------- */
//内存管理被挪到外部使用

#define MEMCPY(dst,src,len)             memcpy((void *)dst,(void *)src,len)
#define SMEMCPY(dst,src,len)            memcpy((void *)dst,(void *)src,len)

#define MEMP_NUM_PBUF           4
#define MEMP_NUM_UDP_PCB        64
#define MEMP_NUM_TCP_PCB        64
#define MEMP_NUM_TCP_PCB_LISTEN 64
#define MEMP_NUM_TCP_SEG        128
#define MEMP_NUM_SYS_TIMEOUT    16

/*---------- Internal Memory Pool Sizes ----------*/
#define MEMP_NUM_RAW_PCB                4
#define MEMP_NUM_REASSDATA              32
#define IP_REASS_MAX_PBUFS              64
#define MEMP_NUM_FRAG_PBUF              32
#define MEMP_NUM_ARP_QUEUE              4
#define MEMP_NUM_IGMP_GROUP             4

/* ---------- Pbuf options ---------- */
#define PBUF_POOL_SIZE          32
#define PBUF_POOL_BUFSIZE       LWIP_MEM_ALIGN_SIZE(TCP_MSS+40+PBUF_LINK_HLEN)

#define MEMP_NUM_NETCONN                (MEMP_NUM_TCP_PCB+MEMP_NUM_TCP_PCB_LISTEN+MEMP_NUM_UDP_PCB+MEMP_NUM_RAW_PCB)
#define MEMP_NUM_TCPIP_MSG_API          4
#define MEMP_NUM_NETBUF                 512
#define MEMP_NUM_TCPIP_MSG_INPKT        128


#define MEMP_SEPARATE_POOLS             0

#if MEMP_SEPARATE_POOLS == 0
extern unsigned char __attribute__((aligned (4)))  memp_memory[];
#else
extern unsigned char __attribute__((aligned (4)))  memp_memory_PBUF_POOL_base[];

extern unsigned char __attribute__((aligned (4)))  memp_memory_NETBUF_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_NETCONN_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_PBUF_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_NETDB_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_REASSDATA_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_TCP_PCB_LISTEN_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_SYS_TIMEOUT_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_UDP_PCB_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_TCPIP_MSG_INPKT_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_LOCALHOSTLIST_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_FRAG_PBUF_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_TCP_PCB_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_IGMP_GROUP_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_TCPIP_MSG_API_base[];
extern unsigned char __attribute__((aligned (4)))  memp_memory_TCP_SEG_base[];
#endif


#define MEMP_OVERFLOW_CHECK             0//2
#define MEMP_SANITY_CHECK               0//1


/* ---------- TCP options ---------- */
#define LWIP_TCP                1
#define TCP_TTL                 255

#define LWIP_TCP_KEEPALIVE              1

/*Don't change this unless you know what you're doing */

/* The maximum segment lifetime in milliseconds */
#define  TCP_MSL 0//60000UL
#define  TCP_KEEPIDLE_DEFAULT     7200000UL /* Default KEEPALIVE timer in milliseconds */
#define  TCP_KEEPINTVL_DEFAULT    75000UL   /* Default Time between KEEPALIVE probes in milliseconds */
#define  TCP_KEEPCNT_DEFAULT      9U        /* Default Counter for KEEPALIVE probes */

/* ---------- UDP options ---------- */
#define LWIP_UDP                1
#define LWIP_UDPLITE            1
#define UDP_TTL                 255
#define CHECKSUM_GEN_UDP                0
#define CHECKSUM_CHECK_UDP               0

/* ---------- ICMP options ---------- */
#define LWIP_ICMP                 1
#define ICMP_TTL                255


/*  ---------- IGMP options ----------*/
#define LWIP_IGMP                  1
#define LWIP_RAND()             rand()

/*---------- DHCP options ----------*/
#define LWIP_DHCP                       1
/*---------- AUTOIP options ----------*/
#define LWIP_AUTOIP                     1
#define LWIP_DHCP_AUTOIP_COOP           1
#define LWIP_DHCP_AUTOIP_COOP_TRIES     20
/*---------- DNS options -----------*/
#define MEMP_NUM_NETDB                  16
#define LWIP_DNS                        1
#define DNS_TABLE_SIZE                  64
#define DNS_MAX_NAME_LENGTH             256
#define DNS_MAX_SERVERS                 2
#define DNS_DOES_NAME_CHECK             1
#define DNS_MSG_SIZE                    512
#define DNS_LOCAL_HOSTLIST              1
#define DNS_LOCAL_HOSTLIST_INIT {{"lwip_jl", 0x123},}
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC   1

/*---------- Statistics options ----------*/
#define LWIP_STATS                      1

#if LWIP_STATS

#define LWIP_STATS_DISPLAY              1
#define LINK_STATS                      0
#define ETHARP_STATS                    0//(LWIP_ARP)
#define IP_STATS                        0
#define IPFRAG_STATS                    0//(IP_REASSEMBLY || IP_FRAG)
#define ICMP_STATS                      0
#define IGMP_STATS                      0//(LWIP_IGMP)
#define UDP_STATS                       0//(LWIP_UDP)
#define TCP_STATS                       0//(LWIP_TCP)
#define MEM_STATS                       0//((MEM_LIBC_MALLOC == 0) && (MEM_USE_POOLS == 0)) 内存管理被挪到外部使用?
#define MEMP_STATS                      (MEMP_MEM_MALLOC == 0)
#define SYS_STATS                       0//(NO_SYS == 0)

#else

#define LINK_STATS                      0
#define IP_STATS                        0
#define IPFRAG_STATS                    0
#define ICMP_STATS                      0
#define IGMP_STATS                      0
#define UDP_STATS                       0
#define TCP_STATS                       0
#define MEM_STATS                       0
#define MEMP_STATS                      0
#define SYS_STATS                       0
#define LWIP_STATS_DISPLAY              0

#endif /* LWIP_STATS */

/*---------- Checksum options ----------*/
#define LWIP_CHKSUM                     ethernet_checksum    //使用硬件算checksum

/*
   ---------------------------------------
   ---------- Debugging options ----------
   ---------------------------------------
*/
//#define LWIP_NOASSERT

#define LWIP_DEBUG  1

#ifdef LWIP_DEBUG

#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_WARNING//LWIP_DBG_LEVEL_ALL
#define ETHARP_DEBUG                    LWIP_DBG_OFF
#define NETIF_DEBUG                     LWIP_DBG_OFF
#define PBUF_DEBUG                      LWIP_DBG_OFF
#define API_LIB_DEBUG                   LWIP_DBG_OFF
#define API_MSG_DEBUG                   LWIP_DBG_OFF
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#define ICMP_DEBUG                      LWIP_DBG_OFF
#define IGMP_DEBUG                      LWIP_DBG_OFF
#define INET_DEBUG                      LWIP_DBG_OFF
#define IP_DEBUG                        LWIP_DBG_OFF
#define IP_REASS_DEBUG                  LWIP_DBG_OFF
#define RAW_DEBUG                       LWIP_DBG_OFF
#define MEM_DEBUG                       (0xffU & ~(LWIP_DBG_HALT))
#define MEMP_DEBUG                      (0xffU & ~(LWIP_DBG_HALT))
#define SYS_DEBUG                       LWIP_DBG_OFF
#define TIMERS_DEBUG                    LWIP_DBG_OFF
#define TCP_DEBUG                       LWIP_DBG_OFF
#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
#define TCP_FR_DEBUG                    LWIP_DBG_OFF
#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
#define TCP_WND_DEBUG                   LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
#define TCP_RST_DEBUG                   LWIP_DBG_OFF
#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_OFF
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#define PPP_DEBUG                       LWIP_DBG_OFF
#define SLIP_DEBUG                      LWIP_DBG_OFF
#define DHCP_DEBUG                      LWIP_DBG_OFF
#define AUTOIP_DEBUG                    LWIP_DBG_OFF
#define SNMP_MSG_DEBUG                  LWIP_DBG_OFF
#define SNMP_MIB_DEBUG                  LWIP_DBG_OFF
#define DNS_DEBUG                       LWIP_DBG_OFF
#define LWIP_PERF                       LWIP_DBG_OFF

#define SMTP_DEBUG                      (0xffU & ~(LWIP_DBG_HALT))
#define HTTPD_DEBUG                     (0xffU & ~(LWIP_DBG_HALT))
#define HTTPD_DEBUG_TIMING		        (0xffU & ~(LWIP_DBG_HALT))
#define SNTP_DEBUG                      (0xffU & ~(LWIP_DBG_HALT))
#define RTP_DEBUG                       (0xffU & ~(LWIP_DBG_HALT))
#define PING_DEBUG                      (0xffU & ~(LWIP_DBG_HALT))


#endif
