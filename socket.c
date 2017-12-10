SYSCALL_DEFINE5(send_repeat, int, fd, void __user *, buff, size_t, len,
    unsigned int, flags, unsigned int, repeat)
{
return sys_sendto_repeat(fd, buff, len, flags, NULL, repeat);
}

SYSCALL_DEFINE6(sendto_repeat, int, fd, void __user *, buff, size_t, len,
    unsigned int, flags, struct sockaddr __user *, addr,
    unsigned int, repeat)
{
int addr_len = 0;
struct socket *sock;
struct sockaddr_storage address;
int err;
struct msghdr msg;
struct iovec iov;
int fput_needed;

err = import_single_range(WRITE, buff, len, &iov, &msg.msg_iter);
if (unlikely(err))
    return err;
sock = sockfd_lookup_light(fd, &err, &fput_needed);
if (!sock)
    goto out;

msg.msg_name = NULL;
msg.msg_control = NULL;
msg.msg_controllen = 0;
msg.msg_namelen = 0;
if (addr) {
    err = move_addr_to_kernel(addr, addr_len, &address);
    if (err < 0)
        goto out_put;
    msg.msg_name = (struct sockaddr *)&address;
    msg.msg_namelen = addr_len;
}
if (sock->file->f_flags & O_NONBLOCK)
    flags |= MSG_DONTWAIT;
msg.msg_flags = flags;
char val[] = {(char) 1, (char) repeat};
kernel_setsockopt(sock, IPPROTO_TCP, TCPOPT_REPEAT, val, TCPOLEN_REPEAT);

err = sock_sendmsg(sock, &msg);

out_put:
fput_light(sock->file, fput_needed);
out:
return err;
}