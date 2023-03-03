// Copyright 2023 Jackie Wang
#include <proj2/inc/client.h>

int main(int argc, char **argv) {
  DomainSocketClient dsc(argv[1], false);
  dsc.RunClient(argc, argv);
}
