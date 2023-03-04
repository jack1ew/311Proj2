// Copyright 2023 Jackie Wang
#include <proj2/inc/server.h>
std::vector<std::vector<std::string>> DomainSocketServer::fileParser(std::string path) const{
 
	std::vector<std::vector<std::string>> content;
	std::vector<std::string> row;
	std::string line, word;
 
	std::fstream file (path, std::ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();
 
			std::stringstream str(line);
 
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
  std::string str = ch;
  
  return str;
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

std::string DomainSocketServer::searcher(std::vector<std::string> str, std::vector<std::vector<std::string>> fi) const{
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
            temp += fi[j][k];
        }
        if ((temp.find(str[i]) != std::string::npos) &&
          (results.find(temp) == std::string::npos)) {
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
          temp += fi[j][k];
      }
      if (checker(temp, str)) {
        results += std::to_string(num) + "\t" + temp + nl;
        num += 1;
      }
      temp = "";
    }
  }
}

void DomainSocketServer::bufferWriter(int start, int end, std::string str, char ch[]) const{
  int j = 0;
  for (int i = start; i < end; i++) {
    ch[j] = str[i];
    j += 1;
  }
}

