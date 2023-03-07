// Copyright 2023 Jackie Wang
#include <proj2/inc/client.h>

std::string DomainSocketClient::operationFinder(int argc, char **argv) {
  bool b = true;
  std::string a;
  if (argc == 4) {
    return "n/a";
  }
  for (int i = 4; i < argc - 1; i+=2) {
    if (argv[i] == argv[4]) {
      continue;
    } else {
      b = false;
    }
  }
  a = argv[4];
  if ((a == "+") && b) {
    return "+";
  } else if ((a == "x") && b) {
    return "x";
  } else {
    return "MIXED";
  }
}

std::string DomainSocketClient::seCombiner(std::string op, int argc, char **argv) {
  const char kUS = static_cast<char>(31);
  const char kEoT = static_cast<char>(3);
  std::string se = argv[2]; 
  std::cout << se << std::endl;
  se += kUS + op + kUS + argv[3];
  if (argc > 5) {
    for (int i = 5; i < argc; i+=2) {
      if (i == argc - 1) {
        se += argv[i] + kEoT;
      } else {
        se += argv[i] + kUS;
      }
    }
  } else {
    se += kEoT;
  }
  return se;
}
void DomainSocketClient::bufferWriter(int start, int end, std::string str, char ch[]) {
  int j = 0;
  for (int i = start; i < end; i++) {
    ch[j] = str[i];
    j += 1;
  }

}

