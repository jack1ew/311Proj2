// Copyright 2023 Jackie Wang
#include <proj2/inc/server.h>
std::vector<std::vector<std::string>> DomainSocketServer::fileParser(std::string path) const{
  std::ifstream file(path);
  std::vector<std::vector<std::string>> data;
  std::string line;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::vector<std::string> fields;
      std::stringstream ss(line);
      std::string field;
      while (std::getline(ss, field, ',')) {
        // remove any white space after the comma
        field.erase(std::remove_if(field.begin(), field.end(), ::isspace), field.end());
        fields.push_back(field);
      }
      data.push_back(fields);
    }
    return data;
  } else {
    std::vector<std::string> fields;
    fields.push_back("INVALID FILE\n");
    data.push_back(fields);
    return data;
  }
}

void DomainSocketServer::sanitizer(std::string str) const {
  std::string temp = "";
  for (int i = 0; i < str.size(); ++i) {
    if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')) {
      temp = temp + str[i];
    }
  }
  str = temp;
}

std::vector<std::string> DomainSocketServer::stringParser(std::string str) const{
  // Vector of string to save tokens
  std::vector <std::string> tokens;
     
  // stringstream class check1
  std::stringstream check1(str);

  std::string intermediate;
   
  // Tokenizing w.r.t. space ' '
  while(getline(check1, intermediate, kUS))
  {
    tokens.push_back(intermediate);
  }
   
  return tokens;
}

bool DomainSocketServer::charFinder(std::string str) const{
  std::size_t found = str.find(kEoT);
  if (found != std::string::npos)
    return true;
  return false;
}

std::string DomainSocketServer::stringConverter(char *ch) const{
  std::string str;
  str.assign(ch, ch + strlen(ch));
  return str;
}

std::string DomainSocketServer::stringCombiner(std::vector<std::string> str) const{
  std::string result = "";
  int n = str.size();
  for (int i = 0; i < n; i++) {
    if (i == n - 1) {
      result += str[i];
    } else {
      result += str[i] + ", ";
    }
  }
  return result;
}

bool DomainSocketServer::checker(std::string key, std::vector<std::string> str) const{
  bool b = true;
  if (str[1] == "x") {
    for (int i = 2; i < str.size(); i++) {
      if (key.find(str[i]) != std::string::npos)
        b = true;
      else
        return false;
    }
    return b;
  } else {
    if(key.find(str[2]) != std::string::npos)
      return true;
    else
      return false;
  }
}

bool DomainSocketServer::finder(std::vector<std::string> vec, std::string str) const{
  for (int i = 0; i < vec.size(); i++) {
    if (str.compare(vec[i]) == 0) {
      return true;
    }
    
  }
  return false;
}

std::string DomainSocketServer::searcher(std::vector<std::string> str, std::vector<std::vector<std::string>> fi) const{
  std::string results = "";
  std::string temp = "";
  int num = 1;
  std::string n = "";
  int check = 0;
  bool b = true;
  if ((str[1].compare("+") == 0) || (str[1].compare("n/a") == 0)) {
    for (int i = 2; i < str.size(); i++) {
      for (int j = 0; j < fi.size(); j++) {
        if (finder(fi[j], str[i])) {
          temp = stringCombiner(fi[j]);
          if (results.find(temp) == std::string::npos) {
            n = std::to_string(num);
            results += n + "\t" + temp + "\n";
            num += 1;
          }
        }
      }
    }
  } else if(str[1].compare("x") == 0){
    for (int i = 0; i < fi.size(); i++) {
      for (int j = 0; j < fi[i].size(); j++) {
        for (int k = 2; k < str.size(); k++) {
          if (str[k].compare(fi[i][j]) == 0) {
            check += 1;
          }
        }
      }
      if(check == (str.size() - 2)) {
        n = std::to_string(num);
        temp = stringCombiner(fi[i]);
        results += n + "\t" + temp + "\n";
        num += 1;
      }
    }
  }
  return results;
}

void DomainSocketServer::bufferWriter(int start, int end, std::string str, char ch[]) const{
  int j = 0;
  for (int i = start; i < end; i++) {
    ch[j] = str[i];
    j += 1;
  }
}

