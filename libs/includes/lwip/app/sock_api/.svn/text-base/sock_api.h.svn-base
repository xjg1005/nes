#ifndef __SOCK_API_H__
#define __SOCK_API_H__

enum sock_api_msg_type
{
    SOCK_SEND_TO,
    SOCK_RECV_TO,
    SOCK_CONNECT_SUCC,
    SOCK_CONNECT_FAIL,
    SOCK_UNREG,
};

int sock_get_socket(void *hdl);
void *sock_reg(int domain, int type, int protocol, int (*cb_func)(enum sock_api_msg_type type, void* priv), void *priv);
void sock_unreg(void *sock_hdl);
int sock_set_recv_timeout(void *sock_hdl, u32 millsec);
int sock_get_recv_timeout(void *sock_hdl);
int sock_recv_timeout(void *sock_hdl);
int sock_set_send_timeout(void *sock_hdl, u32 millsec);
int sock_get_send_timeout(void *sock_hdl);
int sock_send_timeout(void *sock_hdl);
int sock_recv(void *sock_hdl, u8 *buf, u32 len, int flag);
int sock_send(void *sock_hdl, u8 *buf, u32 len, int flag);
void *sock_accept(void *sock_hdl, struct sockaddr *addr, socklen_t *addrlen, int (*cb_func)(enum sock_api_msg_type type, void* priv), void *priv);
int sock_bind(void *sock_hdl, const struct sockaddr *name, socklen_t namelen);
int sock_listen(void *sock_hdl, int backlog);
void sock_set_connect_to(void *sock_hdl, int sec);
int sock_connect(void *sock_hdl, const struct sockaddr *name, socklen_t namelen);
int sock_select_rdset(void *sock_hdl);
int socket_set_keepalive (void *sock_hdl, int keep_idle, int keep_intv, int keep_cnt);

int sock_recvfrom(void *sock_hdl, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
int sock_sendto(void *sock_hdl, const void *data, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);

#endif

