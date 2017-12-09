SYSCALL_DEFINE4(send_repeat, int, fd, void __user *, buff, size_t, len,
    unsigned int, flags)
{
return sys_sendto(fd, buff, len, flags, NULL, 0);
}