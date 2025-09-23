#include <cassert>
#include <fcntl.h>
#include <iostream>
#include <liburing.h>
#include <netinet/in.h>
#include <unistd.h>
using namespace std;

constexpr int BUFSIZE = 1024;

struct request
{
    enum STATE
    {
        ACCEPT,
        READ,
        WRITE
    };
    int fd;
    STATE state;
    union {
        struct
        {
            sockaddr_in ipv4_addr;
            socklen_t lens;
        } addr;
        char buf[BUFSIZE];
    };
};

class IOuring
{
    io_uring ring;

  public:
    IOuring(const int &queue_size)
    {
        io_uring_queue_init(queue_size, &ring, 0);
    }
    ~IOuring()
    {
        io_uring_queue_exit(&ring);
    }
    void seen(io_uring_cqe *const cqe)
    {
        io_uring_cqe_seen(&ring, cqe);
    }
    int wait(io_uring_cqe **const cqe)
    {
        return io_uring_wait_cqe(&ring, cqe);
    }
    int submit()
    {
        return io_uring_submit(&ring);
    }
    void accept_async(const int &sock_fd, request *body)
    {
        io_uring_sqe *sqe = io_uring_get_sqe(&ring);
        body->state = request::ACCEPT;
        body->fd = sock_fd;
        body->addr.lens = sizeof(sockaddr_in);
        io_uring_prep_accept(sqe, sock_fd, (sockaddr *)&(body->addr.ipv4_addr), &(body->addr.lens), 0);
        io_uring_sqe_set_data(sqe, body);
    }

    void read_async(const int &client_fd, request *body)
    {
        io_uring_sqe *sqe = io_uring_get_sqe(&ring);
        body->state = request::READ;
        body->fd = client_fd;
        io_uring_prep_read(sqe, client_fd, body->buf, sizeof(body->buf), -1);
        io_uring_sqe_set_data(sqe, body);
    }

    void write_asyn(const int &client_fd, request *body)
    {
        io_uring_sqe *sqe = io_uring_get_sqe(&ring);
        body->state = request::WRITE;
        body->fd = client_fd;
        io_uring_prep_write(sqe, client_fd, body->buf, sizeof(body->buf), -1);
        io_uring_sqe_set_data(sqe, body);
    }
};

int main(int argc, char const *argv[])
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sock_addr;
    sock_addr.sin_port = htons(8000);
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(sock_fd, (sockaddr *)&sock_addr, sizeof(sock_addr));
    perror("");
    listen(sock_fd, 10);

    cout << "listen begin ...\n";

    IOuring ring(1024);
    ring.accept_async(sock_fd, new request);
    ring.submit();

    while (1)
    {
        io_uring_cqe *cqe;
        ring.wait(&cqe);
        request *res = (request *)cqe->user_data;
        switch (res->state)
        {
        case request::ACCEPT:
            if (cqe->res > 0)
            {
                int client_fd = cqe->res;
                ring.accept_async(sock_fd, res);
                ring.read_async(client_fd, new request);
                ring.submit();
            }
            cout << cqe->res << endl;
            break;
        case request::READ:
            if (cqe->res > 0)
            {
                cout << res->buf << endl;
            }
            ring.write_asyn(res->fd, res);
            ring.submit();
            break;
        case request::WRITE:
            if (cqe->res > 0)
            {
                close(res->fd);
                delete res;
            }
            break;

        default:
            cout << "error\n";
            break;
        }
        ring.seen(cqe);
    }

    return 0;
}
