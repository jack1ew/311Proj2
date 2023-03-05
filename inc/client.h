// Copyright 2023 Jackie Wang
#ifndef PROJ2_INC_CLIENT_H_
#define PROJ2_INC_CLIENT_H_

#include <proj2/inc/unix_domain_socket.h>
///
/// Domain Socket Client C++ Interface Class
///
class DomainSocketClient : public UnixDomainSocket {
 public:

  using UnixDomainSocket::UnixDomainSocket;
  std::string operationFinder(int argc, char **argv);
  std::string seCombiner(std::string op, int argc, char **argv);
  void bufferWriter(int sp, int ep, std::string str, char ch[]);
  void RunClient(int argc, char **argv) {
    // (1) open nameless Unix socket
    std::string op = operationFinder(argc, argv);
    std::string se = seCombiner(op, argc, argv);
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }
    if(op == "MIXED") {
      std::cerr << "Mixed boolean operations not presently supported" << std::endl;
      exit(2);
    }

    // (2) connect to an existing socket
    int success = connect(socket_fd,
                          // sockaddr_un is a Unix sockaddr
                          reinterpret_cast<const sockaddr*>(&sock_addr_),
                          sizeof(sock_addr_));
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }
    std::clog << "SERVER CONNECTION ACCEPTED" << std::endl;

    // (3) write to socket
    const size_t kRead_buffer_size = 32;  // read 4 byte increaments
    char read_buffer[kRead_buffer_size];
    int bytes_read = 0;
    int bytes_wrote = 0;
    const ssize_t kWrite_buffer_size = 64;
    char write_buffer[kWrite_buffer_size];
    int ep = kWrite_buffer_size;
    int sp = 0;
    int seSize = se.size();
    int t = 0;
    while (true) {
      if(ep < seSize) {
        bufferWriter(sp, ep , se, write_buffer);
        ep += kWrite_buffer_size;
        sp += kWrite_buffer_size;
        t = write(socket_fd, write_buffer, kWrite_buffer_size);
        bytes_wrote += t;
      } else {
        bufferWriter(sp, seSize, se, write_buffer);
        t = write(socket_fd, write_buffer, kWrite_buffer_size);
        bytes_wrote += t;
      }
      while (t > 0) {
        if (t < 0) {
          std::cerr << strerror(errno) << std::endl;

          exit(-1);
        }

        if (t == 0) {
          std::clog << "Server dropped connection!" << std::endl;
          exit(-2);
        }
        if(ep < seSize) {
          bufferWriter(sp, ep , se, write_buffer);
          ep += kWrite_buffer_size;
          sp += kWrite_buffer_size;
          t = write(socket_fd, write_buffer, kWrite_buffer_size);
          bytes_wrote += t;
        } else {
          bufferWriter(sp, seSize, se, write_buffer);
          t = write(socket_fd, write_buffer, kWrite_buffer_size);
          bytes_wrote += t;
        }
      }
      t = read(socket_fd, read_buffer, kRead_buffer_size);

      const char kKill_msg[] = "quit";
      while (t > 0) {
        if (strcmp(read_buffer, kKill_msg) == 0) {
          std::cout << "Server shutting down..." << std::endl;

          bytes_read = 0;  // message handled, disconnect client
          exit(0);
        }

        std::cout.write(read_buffer, bytes_read);
        t = read(socket_fd, read_buffer, kRead_buffer_size);
        bytes_read += t;
      }
      std::clog << "BYTES READ" << bytes_read;
      if (t == 0) {
        std::cout << "Server disconnected" << std::endl;

        close(socket_fd);
      } else if (t < 0) {
        std::cerr << strerror(errno) << std::endl;

        exit(-1);
      }
    }
  }
};

#endif  // PROJ2_INC_CLIENT_H_

