#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

//https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start (in place)

void say(std::string& str){
    std::cout << str << '\n';
}

std::string basename(const std::string& pathname )
{
    struct MatchPathSeparator
    {
        bool operator()( char ch ) const
        {
            return ch == '/';
        }
    };
    return std::string(
                std::find_if( pathname.rbegin(), pathname.rend(),
                              MatchPathSeparator() ).base(),
                pathname.end() );
}

void help(std::string& pname, int exitstat){
    pname=basename(pname);
    //pname=pname.replace(pname.begin(),pname.end(), "/");
    say("The purpose of this is is to retreave a user@ip string from a file based on a given alias");
    say("usage: "+pname+" [alias/ip] [Options]");
    say("");
    say("[--help/-h]                          Show this help message");
    say("[--find/-f]      [alias]             Output ip based on alias");
    say("[--exists/-e]    [IP] [alias]        Check if server ip exists");
    say("[--delete/-d]    [IP]                Delete ip (and alias)");
    say("[--add/-a]       [IP] [alias]        Add ip (and alias)");
    say("[--rename/-r]    [IP] [new alias]    Add ip (and alias)");
    say("");
    say("If you find a bug contact me at");
    say("      Alex Angel <https://github.com/BoredBored/MyCoolStuff/>");
    exit(exitstat);
}

bool file_exists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

vector<string> split(std::string& str,std::string& token){
    vector<string>result;
    boost::split(result, str,boost::is_any_of(token));
    return result;
}

bool validIP(std::string& ip){
    boost::system::error_code ec;
    boost::asio::ip::address::from_string( ip, ec );
    if ( ec ){
        return false;
    } else {
        return true;
    }
}

void writeFile(std::vector<std::vector<std::string>>& filevec, std::string& sshAliasFile){
    std::string tmpfile = "";
    for(unsigned int y = 0; y < filevec.size(); y++){
        std::string line = "";
        for(unsigned int x = 0; x < filevec[y].size(); x++){
            line += (x && !filevec[y][x].empty() ? " ": "")+filevec[y][x];
        }
        tmpfile += line + (y == filevec.size()-1 ? "" : "\n");
    }
    std::ofstream sshfile;
    sshfile.open(sshAliasFile);
    if(sshfile){
        sshfile << tmpfile;
        sshfile.close();
    } else {
        say("Error opening ssh file");
        exit(1);
    }
}

int main(int argc, char *argv[] ){
    std::string serverName = "";
    std::string extraSrvName = "";
    std::string ip = "";
    bool findserv = false;
    bool addserv = false;
    bool delserv = false;
    bool rename = false;
    bool checkIfExists = false;

    std::string usrIPStr = "";
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    //pointers are weird
    const std::string home = homedir;
    std::string sshAliasFile = home+"/sshAlias";
    //say(sshAliasFile);
    //say(std::to_string(argc));
    if (argc==1)
    {
        help(argv[0], 1);
    }
    else
    {
        for (int i = 1; i < argc; i++) {
            //pointers are weird
            std::string opt = argv[i];
            //say(to_string(i));
            if(opt == "--help"|| opt == "-h"){
                help(argv[0], 0);
            } else if (opt == "--add"|| opt == "-a"){
                addserv = true;
            } else if (opt == "--delete" || opt == "-d") {
                delserv = true;
            } else if (opt == "--exists" || opt == "-e") {
                checkIfExists = true;
            }  else if (opt == "--find" || opt == "-f") {
                findserv = true;
            } else if (opt == "--rename" || opt == "-r"){
                rename = true;
            } else {
                if (argc == 2){
                    if(validIP(opt)){
                        ip = opt;
                        checkIfExists = true;
                    } else {
                        if(serverName.empty()){
                            serverName = opt;
                        } else {
                            extraSrvName = opt;
                        }
                        findserv = true;
                        //i = argc;
                    }
                } else if(argc >= 3) {
                    if(validIP(opt)){
                        ip = opt;
                    } else {
                        serverName = opt;
                    }
                } else {
                    say("Invalid options");
                    return 1;
                }
            }
        }
    return 0;
}
