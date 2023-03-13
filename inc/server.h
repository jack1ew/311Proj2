// Copyright 2023 Jackie Wang
#ifndef PROJ2_INC_SERVER_H_
#define PROJ2_INC_SERVER_H_
#include <proj2/inc/unix_domain_socket.h>

class DomainSocketServer : public UnixDomainSocket {
 public:
  using ::UnixDomainSocket::UnixDomainSocket;
  const char kEoT = static_cast<char>(3);
  const char kUS = static_cast<char>(31);

  std::string stringConverter(char *ch) const;
  std::vector<std::string> stringParser(std::string str) const;
  std::vector<std::vector<std::string>> fileParser(std::string path) const;
  std::string searcher(std::vector<std::string> str, 
                       std::vector<std::vector<std::string>> fi) const;
  bool charFinder(std::string str) const;
  void seeking(std::string str) const;
  bool checker(std::string key, std::vector<std::string> str) const;
  void bufferWriter(int start, int end, std::string str, char ch[]) const;
  void sanitizer(std::string str) const;
  std::string stringCombiner(std::vector<std::string> str) const;
  bool finder(std::vector<std::string> vec, std::string str) const;
  std::vector<std::string> splitString(std::string str, int chunkSize) const;
  void strcopy(char ch[], std::string str) const;
  void RunServer() const {
    int sock_fd;  // unnamed socket file descriptor
    int client_req_sock_fd;  // client connect request socket file descriptor

    // (1) create a socket
    //       AF_UNIX -> file system pathnames
    //       SOCK_STREAM -> sequenced bytestream
    //       0 -> default protocol (let OS decide correct protocol)
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if ( sock_fd < 0 ) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    // (2) bind socket to address for the server
    unlink(socket_path_.c_str());  // sys call to delete file if it already
                                   //   exists (using Unix system calls for
                                   //   sockets, no reason to be non-Unix
                                   //   portable now).  :-/
    int success = bind(sock_fd,
                       // sockaddr_un is a Unix sockaddr and so may be cast "up"
                       //   to that pointer type (think of it as C polymorphism)
                       reinterpret_cast<const sockaddr*>(&sock_addr_),
                       // size needs be known due to underlying data layout,
                       //   i.e., there may be a size difference between parent
                       //   and child
                       sizeof(sock_addr_));

    // log errors
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    // (3) Listen for connections from clients
    size_t n = get_nprocs_conf();
    size_t kMax_client_conns = n - 1;
    success = listen(sock_fd, kMax_client_conns);
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }
    std::clog << "SERVER STARTED" << "\nMAX CLIENTS = " << kMax_client_conns << std::endl;
    const size_t kRead_buffer_size = 10;  // read 4 byte increaments
    char read_buffer[kRead_buffer_size];
    const ssize_t kWrite_buffer_size = 1;
    char write_buffer[kWrite_buffer_size];
    int bytes_read = 0;
    int bytes_wrote = 0;
    std::string search_string = "";
    std::string file_name = "";
    std::vector<std::string> search_s;
    std::string fileOutput;
    int t = 0;
    while (true) {
      // (4) Accept connection from a client
      client_req_sock_fd = accept(sock_fd, nullptr, nullptr);
      if (client_req_sock_fd  < 0) {
        std::cerr << strerror(errno) << std::endl;
        continue;
      }

      std::clog << "CLIENT CONNECTED" << std::endl;
      
      // (5) Receive data from client(s)
      bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      const char kKill_msg[] = "quit";  // TODO(lewisjs): trim whitespace
                                        //   from read_buffer for comparison
      while (bytes_read > 0) {
        if (strcmp(read_buffer, kKill_msg) == 0) {
          std::clog << "Server shutting down..." << std::endl;
          bytes_read = 0;  // message handled, disconnect client
          exit(0);
        }
        std::ostringstream oss;
        oss.write(read_buffer, bytes_read);
        std::string output = oss.str();

        // Combines the bytes read into a string and stops 
        search_string += output;
        if((search_string.find(kEoT) != std::string::npos)) {
          search_string.erase(std::remove(search_string.begin(), search_string.end(), kEoT), search_string.end());
          break;
        }
        bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      }
      std::cout<<search_string<<std::endl;
      if (bytes_read == 0) {
        std::cout << "Client disconnected" << std::endl;
        
        close(client_req_sock_fd);
      } else if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }
      
      // Prints path, operation, and seeking
      //std::cout << search_s[2] << std::endl;
      search_s = stringParser(search_string);
      search_string = "";
      std::clog << "PATH: " << search_s[0] << std::endl;
      std::clog << "OPERATION: " << search_s[1] << std::endl;
      std::clog << "SEEKING: ";
      for (int i = 2; i < search_s.size(); i++) {
        std::clog << search_s[i];
        if (i == search_s.size()-1) {
          std::clog << std::endl;
        } else {
          // Attempting to remove the kEoT
          std::clog << ", ";
        }
      }
      
      // Reults of the search
      std::vector<std::vector<std::string>> out = fileParser(search_s[0]);
      fileOutput = searcher(search_s, out);
      std::vector<std::string> chunks = splitString(fileOutput, kWrite_buffer_size);
      strcopy(write_buffer, chunks[0]);
      std::cout << chunks[0];
      t = write(client_req_sock_fd, write_buffer, kWrite_buffer_size);
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
        /*
        if(ep < seSize && !checker(write_buffer)) {
          bufferWriter(sp, ep , se, write_buffer);
          ep += kWrite_buffer_size;
          sp += kWrite_buffer_size;
          t = write(socket_fd, write_buffer, kWrite_buffer_size);
          bytes_wrote += t;
        } else {
          bufferWriter(sp, seSize, se, write_buffer);
          t = write(socket_fd, write_buffer, kWrite_buffer_size);
          bytes_wrote += t;
        }*/
        std::cout << chunks[i];
        strcopy(write_buffer, chunks[i]);
        t = write(client_req_sock_fd, write_buffer, kWrite_buffer_size);
        bytes_wrote += t;
      }
      write_buffer[0] = kEoT;
      t = write(client_req_sock_fd, write_buffer, kWrite_buffer_size);
      bytes_wrote[0] = kEoT;
      std::clog << "BYTES SENT: " << bytes_wrote << std::endl;
      /*
      if (ep < outSize) {
        bufferWriter(sp, ep, fileOutput, write_buffer);
        ep += kWrite_buffer_size;
        sp += kWrite_buffer_size;
        t = write(client_req_sock_fd, write_buffer, kWrite_buffer_size);
        bytes_wrote += t;
      } else {
        bufferWriter(sp, outSize, fileOutput, write_buffer);
        sp += outSize - sp;
        t = write(client_req_sock_fd, write_buffer, kWrite_buffer_size);
        bytes_wrote += t;
      }
      while(t > 0) {
        if (bytes_wrote == 0) {
          std::clog << "Nothing Wrote" << std::endl;
          exit(-2);
        }
        if (bytes_wrote < 0) {
          std::cerr << strerror(errno) << std::endl;
          exit(-1);
        }
        
        if (ep < outSize) {
          bufferWriter(sp, ep, fileOutput, write_buffer);
          ep += kWrite_buffer_size;
          sp += kWrite_buffer_size;
          t = write(client_req_sock_fd, write_buffer, kWrite_buffer_size);
          bytes_wrote += t;
        } else {
          bufferWriter(sp, outSize, fileOutput, write_buffer);
          sp += outSize - sp;
          t = write(client_req_sock_fd, write_buffer, kWrite_buffer_size);
          bytes_wrote += t;
        }
      }
      std::clog << "BYTES SENT: " << bytes_wrote << "\n";
      if (out[0][0] == "INVALID FILE\n")
        close(client_req_sock_fd);
     */ 
    }
  }
};

#endif  // PROJ2_INC_SERVER_H_

