#pragma once

#include "belnet_context.h"

#ifdef _WIN32
extern "C"
{
  struct iovec
  {
    void* iov_base;
    size_t iov_len;
  };
}
#else
#include <sys/uio.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  /// information about a udp flow
  struct belnet_udp_flow
  {
    /// the socket id for this flow used for i/o purposes and closing this socket
    int socket_id;
    /// remote endpoint's .bdx or .mnode address
    char remote_addr[256];
    /// local endpoint's ip address
    char local_addr[64];
    /// remote endpont's port
    int remote_port;
    /// local endpoint's port
    int local_port;
  };

  /// establish an outbound udp flow
  /// remoteHost is the remote .bdx or .mnode address conneting to
  /// remotePort is either a string integer or an srv record name to lookup, e.g. thingservice in
  /// which we do a srv lookup for _udp.thingservice.remotehost.tld and use the "best" port provided
  /// localAddr is the local ip:port to bind our socket to, if localAddr is NULL then
  /// belnet_udp_sendmmsg MUST be used to send packets return 0 on success return nonzero on fail,
  /// containing an errno value
  int EXPORT
  belnet_udp_establish(
      char* remoteHost,
      char* remotePort,
      char* localAddr,
      struct belnet_udp_flow* flow,
      struct belnet_context* ctx);

  /// a result from a belnet_udp_bind call
  struct belnet_udp_bind_result
  {
    /// a socket id used to close a belnet udp socket
    int socket_id;
  };

  /// inbound listen udp socket
  /// expose udp port exposePort to the void
  /// if srv is not NULL add an srv record for this port, the format being "thingservice" in which
  /// will add a srv record "_udp.thingservice.ouraddress.tld" that advertises this port provide
  /// localAddr to forward inbound udp packets to "ip:port" if localAddr is NULL then the resulting
  /// socket MUST be drained by belnet_udp_recvmmsg
  ///
  /// returns 0 on success
  /// returns nonzero on error in which it is an errno value
  int EXPORT
  belnet_udp_bind(
      int exposedPort,
      char* srv,
      char* localAddr,
      struct belnet_udp_listen_result* result,
      struct belnet_context* ctx);

  /// poll many udp sockets for activity
  /// returns 0 on sucess
  ///
  /// returns non zero errno on error
  int EXPORT
  belnet_udp_poll(
      const int* socket_ids,
      size_t numsockets,
      const struct timespec* timeout,
      struct belnet_context* ctx);

  struct belnet_udp_pkt
  {
    char remote_addr[256];
    int remote_port;
    struct iovec pkt;
  };

  /// analog to recvmmsg
  ssize_t EXPORT
  belnet_udp_recvmmsg(
      int socket_id,
      struct belnet_udp_pkt* events,
      size_t max_events,
      struct beldexent_context* ctx);

#ifdef __cplusplus
}
#endif
