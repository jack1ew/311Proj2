// Copyright 2023 Jackie Wang
#include <proj2/inc/client.h>

int main(int argc, char **argv) {
  DomainSocketClient dsc(argv[1]);
  dsc.RunClient(argc, argv);
}
