// Copyright 2023 Jackie Wang
#include <proj2/inc/server.h>

int main(int argc, char** argv) {
  DomainSocketServer dss(argv[1]);
  dss.RunServer();
  return 0;
}
