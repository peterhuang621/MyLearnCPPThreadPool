#include <cassert>
#include <fcntl.h>
#include <iostream>
#include <liburing.h>
#include <unistd.h>
using namespace std;

char buf[1024] = {0};

int main(int argc, char const *argv[])
{
    int fd = open("./1.txt", O_RDONLY, 0);
    io_uring ring;
    io_uring_queue_init(32, &ring, 0);
    io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, fd, buf, sizeof(buf), 0);
    io_uring_submit(&ring);
    io_uring_cqe *res;
    io_uring_wait_cqe(&ring, &res);
    assert(res);
    cout << "read bytes: " << res->res << '\n';
    cout << buf << '\n';
    io_uring_cqe_seen(&ring, res);
    io_uring_queue_exit(&ring);
    return 0;
}
