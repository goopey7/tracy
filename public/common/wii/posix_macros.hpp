#pragma once

#include <network.h>

#define socket(d, t, p)       net_socket(d, t, p)
#define bind(s, a, l)         net_bind(s, a, l)
#define listen(s, b)          net_listen(s, b)
#define accept(s, a, l)       net_accept(s, a, l)
#define connect(s, a, l)      net_connect(s, a, l)
#define send(s, d, l, f)      net_send(s, d, l, f)
#define recv(s, d, l, f)      net_recv(s, d, l, f)
#define close(s)              net_close(s)
#define setsockopt(s, l, o, v, len) net_setsockopt(s, l, o, v, len)
#define getsockopt(s, l, o, v, len) net_getsockopt(s, l, o, v, len)
#define select(n, r, w, e, t) net_select(n, r, w, e, t)
#define fcntl(s, c, f)        net_fcntl(s, c, f)
#define poll(s, n, t)         net_poll(s, n, t)
#define shutdown(s, h)        net_shutdown(s, h)
#define recvfrom(s, mem, len, flags, from, fromlen) net_recvfrom(s, mem, len, flags, from, fromlen)
#define sendto(s, data, len, flags, to, tolen) net_sendto(s, data, len, flags, to, tolen)
#ifndef POLLIN
#define POLLIN   0x0001
#define POLLOUT  0x0008
#define POLLERR  0x0020
#define POLLHUP  0x0040
#define POLLNVAL 0x0080
#endif
