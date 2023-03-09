// Copyright 2023 Jackie Wang
#include <proj2/inc/client.h>

std::string DomainSocketClient::operationFinder(int argc, char **argv) {
  bool b = true;
  std::string a;
  if (argc < 5) {
    return "n/a";
  }
  for (int i = 4; i < argc; i += 2) {
    std::cout << b << std::endl;
    if (strcmp(argv[i], "+") == 0) {
      // skip to the next iteration of the loop
      continue;
    } else if (strcmp(argv[i], "x") == 0) {
      // skip to the next iteration of the loop
      continue;
    } else {
      b = false;
      break;
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
  std::string se;
  
  if (argc < 4) {
    return "";
  }
  
  // Concatenate the first three arguments
  se = argv[2];
  se += kUS + op + kUS + argv[3];

  // Concatenate remaining arguments
  for (int i = 5; i < argc; i += 2) {
    if (i < argc - 1) {
      se += kUS + argv[i];
    }
  }

  // Add end-of-transmission character
  se += kEoT;

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
  for(int i = 0; i < 10; i++) {
    ch[i] = str[i];
  }
}
