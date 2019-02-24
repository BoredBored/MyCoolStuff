#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include "json_fwd.hpp"
#include "json.hpp"

using json = nlohmann::json;

bool isValidIPv4(const char* IPAddress);

const std::map<std::string, int> options {
    {"-h", 0},
    {"--help", 0},
    {"-f", 1},
    {"--find", 1},
    {"-e", 2},
    {"--exists", 2},
    {"-d", 3},
    {"--delete", 3},
    {"-a", 4},
    {"--add", 4},
    {"-r", 5},
    {"--rename", 5},
};

enum OPERATIONS {
  FIND,
  EXISTS,
  DELETE,
  ADD,
  RENAME
};

void find(std::vector<std::string>& data, json& filejson){
//      std::cout<<"find\n";
//      std::cout <<"Num of ips "<< filejson.size() << '\n';
        for(unsigned int d=0;d<data.size();++d){
                const auto& id = data[d];
                //std::cout << id << '\n';
                if(isValidIPv4(id.c_str())){
                        //std::cout<<"Valid ip\n";
                        for(unsigned int i=0;i<filejson.size();++i){
                                if(filejson[i]["ip"]==id){
                        //              std::cout << "FOUND\n";
                                        if(!filejson[i]["als"].size()){
                                                std::cout<<id<<" has no aliases\n";
                                                return;
                                        }
                                        std::cout<<filejson[i]["als"][0].get<std::string>()<<'\n';
                                        return;
                                }

                        }
                std::cout<<id<< " ip not found\n";
                return;
                }
                else {
                        for(unsigned int i=0;i<filejson.size();++i){
                                for(unsigned int g=0;g<filejson[i]["als"].size();++g){
                                        auto& al=filejson[i]["als"][g];
                                        if(id==al){
                                                std::cout<<filejson[i]["ip"].get<std::string>()<<'\n';
                                                return;
                                        }
                                }
                        }
                }
        }
}

typedef void (*functionArray)(std::vector<std::string>& data, json& filejson);
//why use a switch when you can use an array?
//plus more modular
const functionArray operations[]={find};


struct MatchPathSeparator
{
        bool operator()( char ch ) const
        {
            return ch == '/';
        }
};
std::string basename(const std::string& pathname )
{
    return std::string(
                std::find_if( pathname.rbegin(), pathname.rend(),
                              MatchPathSeparator() ).base(),
                pathname.end() );
}

void help(const char* pname){
    //pname=pname.replace(pname.begin(),pname.end(), "/");
    std::cout << \
"Give an alias to your server's ip to make ssh easier\n\
usage: "<<basename(pname)<<" [Option(s)] [alias(es)/ip(s)]\n\
\n\
[--help/-h]                          Show this help message\n\
[--find/-f]      [alias]             Output ip based on alias\n\
[--exists/-e]    [IP] [alias]        Check if server ip exists\n\
[--delete/-d]    [IP]                Delete ip (and alias)\n\
[--add/-a]       [IP] [alias]        Add ip (and alias)\n\
[--rename/-r]    [IP] [new alias]    Add ip (and alias)\n\
\n\
If you find a bug contact me at\n\
      Alex Angel <https://github.com/BoredBored/MyCoolStuff/>\n";
}

bool isValidIPv4(const char* IPAddress)
{
   unsigned int a,b,c,d;
   return sscanf(IPAddress,"%d.%d.%d.%d", &a, &b, &c, &d) == 4;
}

int main(int argc, char * argv[]) {
  if (argc==1){
        help(argv[0]);
        return 1;
  }
  std::vector<int> eventQue;
  std::vector<std::string> eventData;
  // optData;
  for(int i=1;i<argc;++i){
    //
    const std::string opt = argv[i];
    //std::cout << i << ' '<< opt<<'\n';
    try{
      const int e = options.at(opt);
      //+= faster than =
      eventQue.push_back(e);
      //std::cout<<"  "<<opt << " == "<<e<<'\n';
      //help is equal to 0
      if(!e){
        help(argv[0]);
        return 0;
      }
    }
    catch(const std::out_of_range& err){
      //if(isValidIPv4(opt.c_str())){
        //std::cout<<"  "<<opt << " == ip"<<'\n';
      //}
      eventData.push_back(opt.c_str());
    }
  }
  if(!eventData.size()){
    std::cout<<"Please specify parameters\n";
    return 1;
  }
  if(eventQue.size()>eventData.size()){
          std::cout<<"Too many parameters\n";
          return 1;
  }
  std::ifstream jsonFile("file.json");
  json jsonData;
  jsonFile >> jsonData;
  if(!eventQue.size()){
    operations[FIND](eventData, jsonData);
    return 0;
  }
  std::cout << "Executing events\n";
  operations[FIND](eventData, jsonData);
}
