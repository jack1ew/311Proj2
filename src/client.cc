// Copyright 2023 Jackie Wang
#include <proj2/inc/client.h>

std::string DomainSocketClient::operationFinder(int argc, std::vector<std::string> argv) {
  int p = 0;
  int m = 0;
  int val = (argc - 2)/2 - 1;
  std::string a;
  if (argc < 5) {
    return "n/a";
  }
  for (int i = 4; i < argc; i += 2) {
    if (argv[i] == "+") {
      p += 1;
    } else if (argv[i] == "x") {
      m += 1;
    }
  }
  a = argv[4];
  if ((a == "+") && (p == val)) {
    return "+";
  } else if ((a == "x") && (m == val)) {
    return "x";
  } else {
    return "MIXED";
  }
}

std::string DomainSocketClient::seCombiner(std::string op, int argc, std::vector<std::string> args) {
  const char kUS = static_cast<char>(31);
  const char kEoT = static_cast<char>(3);
  std::string se;

  // Check for minimum number of arguments
  if (argc < 4) {
    throw std::runtime_error("Not enough arguments");
  }

  // Concatenate the first three arguments
  se = args[2];
  se += kUS + op;

  // Concatenate remaining arguments
  for (int i = 3; i < argc; i += 2) {
    if (i < argc - 1) {
      se += kUS + args[i];
    }
  }

  // Add end-of-transmission character
  se += kUS + args[argc - 1] + kEoT;

  return se;
}

void DomainSocketClient::bufferWriter(int start, int end, std::string str, char ch[]) {
  int j = 0;
  for (int i = start; i < end; i++) {
    ch[j] = str[i];
    str[i] = NULL;
    std::cout << ch[j];
    j += 1;
  }
  std::cout<<std::endl;

}

std::vector<std::string> DomainSocketClient::splitString(std::string str, int chunkSize) {
  std::vector<std::string> ret;
  int strLength = str.length();
  int numChunks = strLength / chunkSize;
  int remainingChars = strLength % chunkSize;
  for (int i = 0; i < numChunks; i++) {
    ret.push_back(str.substr(i * chunkSize, chunkSize));
  }
  if (remainingChars > 0) {
    ret.push_back(str.substr(numChunks * chunkSize, remainingChars));
  }
  return ret;
}

void DomainSocketClient::strcopy(char ch[], std::string str) {
  for(int i = 0; i < 1; i++) {
    ch[i] = str[i];
  }
}
