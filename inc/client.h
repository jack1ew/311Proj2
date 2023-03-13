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

  // README.md for function description
  std::string operationFinder(int argc, std::vector<std::string> argv);
  std::string seCombiner(std::string op, int argc, std::vector<std::string> args);
  void bufferWriter(int sp, int ep, std::string str, char ch[]);
  std::vector<std::string> splitString(std::string str, int chunkSize);
  void strcopy(char ch[], std::string str);
  void RunClient(int argc, char **argv) {
    const char kEoT = static_cast<char>(3);
    // (1) open nameless Unix socket
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++) {
      args.push_back(argv[i]);
    }
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    std::string op;
    if (socket_fd < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }
    if (argv[1] == NULL) {
      std::cerr << "No host given" << std::endl;
      exit(-1);
    }
    if(argv[2] == NULL) {
      std::cerr << "Nothing to search for" << std::endl;
      exit(2);
    }
    op = operationFinder(argc, args);
    if(op == "MIXED") {
      std::cerr << "Mixed boolean operations not presently supported" << std::endl;
      exit(2);
    }
    std::string se = seCombiner(op, argc, args);
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
    const size_t kRead_buffer_size = 1;  // read 4 byte increaments
    char read_buffer[kRead_buffer_size];
    int bytes_read = 0;
    int bytes_wrote = 0;
    const ssize_t kWrite_buffer_size = 1;
    char write_buffer[kWrite_buffer_size];
    int ep = kWrite_buffer_size;
    int sp = 0;
    int seSize = se.size();
    int t = 0;
    std::vector<std::string> chunks;
    chunks = splitString(se, kWrite_buffer_size);
    while (true) {
      strcopy(write_buffer, chunks[0]);
      t = write(socket_fd, write_buffer, kWrite_buffer_size);
      bytes_wrote += t;
      for (int i = 1; i < chunks.size(); i++) {
        if (t < 0) {
          std::cerr << strerror(errno) << std::endl;

          exit(-1);
        }

        if (t == 0) {
          std::clog << "Server dropped connection!" << std::endl;
          exit(-2);
        }
        strcopy(write_buffer, chunks[i]);
        t = write(socket_fd, write_buffer, kWrite_buffer_size);
        bytes_wrote += t;
      }

      // Reads from the socket
      t = read(socket_fd, read_buffer, kRead_buffer_size);
      std::string results = ""; 
      const char kKill_msg[] = "quit";
      while (t > 0) {
        if (strcmp(read_buffer, kKill_msg) == 0) {
          std::cout << "Server shutting down..." << std::endl;

          bytes_read = 0;  // message handled, disconnect client
          exit(0);
        }
        if (read_buffer[0] == kEoT) {
          break;
        }
        std::cout << read_buffer[0];
        t = read(socket_fd, read_buffer, kRead_buffer_size);
        bytes_read += t;
      }
      std::clog << "BYTES RECEIVED: " << bytes_read << std::endl;
      if (t == 0) {
        std::cout << "Server disconnected" << std::endl;

        close(socket_fd);
      } else if (t < 0) {
        std::cerr << strerror(errno) << std::endl;

        exit(-1);
      }
      break;
    }

  }
};

#endif  // PROJ2_INC_CLIENT_H_

