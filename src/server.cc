// Copyright 2023 Jackie Wang
#include <proj2/inc/server.h>
std::vector<vector<string>> DomainSocketServer::fileParser(std::string path){
 
	vector<vector<string>> content;
	vector<string> row;
	string line, word;
 
	fstream file (path, ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();
 
			stringstream str(line);
 
			while(getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
    return content;
	}
	else {
		row.push_back("INVALID FILE\n");
    content.push_back(row);
    return content;
  }
}

std::vector<std::string> DomainSocketServer::stringParser(std::string str) {
  const char kUS = static_cast<char>(31);
  // Vector of string to save tokens
  std::vector <string> tokens;
     
  // stringstream class check1
  stringstream check1(str);

  std::string intermediate;
   
  // Tokenizing w.r.t. space ' '
  while(getline(check1, intermediate, kUS))
  {
    tokens.push_back(intermediate);
  }
   
  return tokens;
}

bool DomainSocketServer::charFinder(std::string str) {
  const char kEoT = static_cast<char>(3);
  std::size_t found = str.find(kEoT);
  if (found != std::string::npos)
    return true;
  return false;
}

std::string DomainSocketServer::stringConverter(char *ch) {
  std::string str = ch;
  
  return str;
}

bool DomainSocketServer::checker(std::string key, std::vector<std::string> str) {
  bool b = true;
  if (str[1] == "x") {
    for (int i = 2; i < str.size(); i++) {
      if (key.find(str[i]) != std::string::npos)
        bool = true;
      else
        return false;
    }
    return bool;
  } else {
    if(key.find(str[2]) != std::string::npose)
      return true;
    else
      return false;
  }
}

std::string DomainSocketServer::searcher(std::vector<std::string> str, std::vector<std::vector<std::string>> fi) {
  std::string nl = "\n";
  std::string results = "";
  std::string temp = "";
  int num = 1;
  if (str[1] == "+") {
    for (int i = 2; i < str.size(); i++) {
      for (int j = 0; j < fi.size(); j++) {
        for (int k = 0; k < fi[j].size(); k++) {
          if (k < fi[j].size() - 1)
            temp += fi[j][k] + ", ";
          else
            temp += f[j][k];
        }
        if ((temp.find(str[i]) != std::string::npose) &&
          (results.find(temp) == std::string::npose)) {
          results += std::to_string(num) + "\t" + temp + nl;
        }
      temp = "";
      }
    }
  } else {
    for (int j = 0; j < fi.size(); j++) {
      for (int k = 0; k < fi[j].size(); k++) {
        if (k < fi[j].size() - 1)
          temp += fi[j][k] + ", ";
        else
          temp += f[j][k];
      }
      if (checker(temp, str)) {
        results += std::to_string(num) + "\t" + temp + nl;
        num += 1;
      }
      temp = "";
    }
  }
}

char[] DomainSocketServer::bufferWriter(int start, int end, std::string str) {
  char ch[end-start];
  int j = 0;
  for (int i = start; i < end; i++) {
    ch[j] = str[i];
    j += 1;
  }
  return ch;
}

