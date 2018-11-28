#ifndef __LW_IP_H
#define __LW_IP_H
#include "lwip/netif.h"
#ifdef __cplusplus
extern "C" { /* Make sure we have C-declarations in C++ programs */
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define WIRELESS_IP_ADDR0 (192)
#define WIRELESS_IP_ADDR1 (168)
#define WIRELESS_IP_ADDR2 (1)
#define WIRELESS_IP_ADDR3 (1)

#define WIRELESS_NETMASK0 (255)
#define WIRELESS_NETMASK1 (255)
#define WIRELESS_NETMASK2 (255)
#define WIRELESS_NETMASK3 (0)

#define WIRELESS_GATEWAY0 (192)
#define WIRELESS_GATEWAY1 (168)
#define WIRELESS_GATEWAY2 (1)
#define WIRELESS_GATEWAY3 (1)

#ifndef __LW_IP_C
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern struct netif wireless_netif;
void Init_LwIP(u8_t is_wireless, u8_t dhcp);
int gethostname(char *name, int namelen);
void Get_IPAddress(u8 is_wireless, char *ipaddr);
void Display_IPAddress(void);
void LwIP_Pkt_Handle(void);

#endif /* !defined(__LW_IP_C) */


/*
#error section
-- The standard C preprocessor directive #error should be used to notify the
programmer when #define constants or macros are not present and to indicate
that a #define value is out of range. These statements are normally found in
a module's .H file. The #error directive will display the message within the
double quotes when the condition is not met.
*/


#ifdef __cplusplus
}
#endif

#define NI_NUMERICHOST (1<<0)
#define NI_NUMERICSERV (1<<1)

#endif /* #ifndef __LW_IP_H */
/*-- File end --*/

