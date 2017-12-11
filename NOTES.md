# What needs to be done
I think:
    - Add options to include/linux/tcp.h tcp_sock struct
        - just vars on it? one for repeat, default attached as one
    - Add getsockopt/setsockopt for these options?
    - To be honest I dunno

# Files and Relevance
    - /include/linux/tcp.h (linux_tcp.h)
        - add tcp_repeat_n and tcp_repeat_i to tcp_sock struct
    - /include/uapi/linux/tcp.h (uapi_linux_tcp.h)
        - define TCP_REPEAT = 253
        - define TCP_REPEAT_RETURN = 254
    - /include/net/tcp.h (net_tcp.h)
        - define TCPOPT_TCP_REPEAT = 253
        - define TCPOPT_TCP_REPEAT_RETURN = 254
        - define TCPOLEN_TCP_REPEAT = 3
        - define TCPOLEN_TCP_REPEAT_RETURN = 1
    - /net/socket.c (socket.c)
        - define send_repeat syscall
    - /arch/x86/entry/syscalls/syscall_64.tbl (syscall_64.tbl)
        - add syscall table entry for our syscall (send_repeat, 332)
    - /net/ipv4/tcp_output.c (tcp_output.c)
        - modify set/get sockopt for TCP

# Notes on Path
Entrypoint -> send_repeat
    - cp of send, calls sendto_repeat
send_repeat -> sendto_repeat

tcp_sendmsg -> tcp_transmit_skb
In tcp_output.c
Opts is set in:
    - tcp_syn_options:543
        - computes options for SYN packets
tcp_make_synack:3035 -> tcp_options_write:445
tcp_transmit_skb:908 -> tcp_options_write:445
    - called with 'opts' var

# General notes
- modify retransmission code-- one vision
    - keep skb floating around, when timer goes off retransmission retransmits
    - use retransmit queue
    - tcp_syn_options/tcp_synack_options denote whether to emit or notesthen in options_write figure out if needs to be emitted
    - prepend a npm when adding the tcp_repeat options_write
    - use TCP_REPEAT and TCP_REPEAT_RETURN (253 and 254)
- tcp window_scale and timestamp are newer and must be negotiated (present) on both ends
- do_tcp_setsockopt tcp.c:2452

# Other ideas:
    - build from patch? --> what is the point, then?
    - tcp_write_timer_handler may be important
    - inet_sendmsg -> tcp_sendmsg -> tcp_push
    - tcp_ack <- handles acknowledgments
    - tcp_v4_rcv -> entry on receiver side
    - tcp_connect -> makes syn packet
    - SOL_TCP

#TODO:
    - add to getsockopt/setsockopt

# Links:
    - https://notes.shichao.io/unp/ch2/#tcp-options
    - https://www.ietf.org/rfc/rfc1323.txt // useful to see 'recent' (1992) implementation

# Commits:
    - not useful?
        - https://github.com/torvalds/linux/commit/60e2a7780793bae0debc275a9ccd57f7da0cf195
    - useful:
        - https://github.com/torvalds/linux/commit/8917a777be3ba566377be05117f71b93a5fd909d
        - https://github.com/torvalds/linux/commit/33ad798c924b4a1afad3593f2796d465040aadd5
