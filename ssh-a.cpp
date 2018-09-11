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


using namespace std;

//https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start (in place)

std::string remove_whitespaces(std::string &str){
    boost::trim(str);
    std::regex e ("[ ](?=[^!-~])");
    return std::regex_replace (str, e, "");
}

/*
void split(const std::string& s, char delimiter, std::vector<std::string> &tokens)
{
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
}*/

void say(std::string str){
    cout << str + '\n';
}

std::string basename( std::string const& pathname )
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

void help(std::string pname, int exitstat){
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
    say("      Alexios Angeletakis <aangeletakis@gmail.com>");
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

vector<string> split(string str, string token){
    vector<string>result;
    boost::split(result, str,boost::is_any_of(token));
    return result;
}

bool validIP(std::string ip){
    boost::system::error_code ec;
    boost::asio::ip::address::from_string( ip, ec );
    if ( ec ){
        return false;
    } else {
        return true;
    }
}

void writeFile(std::vector< std::vector< std::string > > &filevec, std::string sshAliasFile){
    std::string tmpfile = "";
    for(unsigned int y = 0; y < filevec.size(); y++){
        std::string line = "";
        for(unsigned int x = 0; x < filevec[y].size(); x++){
            line += (x && !filevec[y][x].empty() ? " ": "")+filevec[y][x];
        }
        tmpfile += line + (y == filevec.size()-1 ? "" : "\n");
    }
    ofstream sshfile;
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

        std::vector< std::vector< std::string > > fileLayout_Value;
        //These extra brakets are here to dealocate memory from things that im not going to use later

        std::vector<std::string> lines;
        {
            std::string line = "";
            std::ifstream sshfile;
            sshfile.open(sshAliasFile);
            if(file_exists(sshAliasFile))
            {
                if(sshfile){
                    while (std::getline(sshfile,line)) {
                        line = remove_whitespaces(line);
                        if(!line.empty()){
                            lines.push_back(line);
                        }
                    }
                    sshfile.close();
                } else {
                    say("Error opening "+sshAliasFile);
                    return 1;
                }
            } else {
                if(!addserv){
                    say("SSH alias file ("+sshAliasFile+") not found");
                    return 1;
                }
            }
        }

        for(unsigned int linenum = 0; linenum < lines.size(); linenum++)
        {
            fileLayout_Value.push_back(split(lines[linenum], " "));
            //say("Size ("+to_string(linenum)+") : "+to_string(fileLayout_Value[linenum].size()));
            if (fileLayout_Value[linenum].size() > 2) {
                say("File "+sshAliasFile+": line "+std::to_string(linenum)+'\n'+lines[linenum]+"\nToo many argumants");
                return 1;
            } else if (fileLayout_Value[linenum].size() < 2) {
                if(fileLayout_Value[linenum].size() == 1){
                    if(!validIP(fileLayout_Value[linenum][0])){
                        say("Line ("+to_string(linenum+1)+") : '"+fileLayout_Value[linenum][0]+"' not a valid IP");
                    }
                } else {
                    say("File "+sshAliasFile+": line "+std::to_string(linenum+1)+'\n'+lines[linenum]+"\nNot enough argumants");
                    return 1;
                }
            }

            //say("Line: "+to_string(linenum)+" Missing whitespace, thus it was not read");
        }


        if (checkIfExists){
            for(unsigned int linenum = 0; linenum < lines.size(); linenum++)
            {
                if(fileLayout_Value[linenum][0] == ip){
                    say("true");
                    return 0;
                } else {
                    if(fileLayout_Value[linenum].size() == 2){
                        if(fileLayout_Value[linenum][1] == serverName){
                            say("true");
                            return 0;
                        }
                    }
                }
            }
            say("false");
            return 2;
        }

        //get ip based on alias
        if(findserv){
            //call it liner cuz I want to monify the string data
            for(unsigned int linenum = 0; linenum < lines.size(); linenum++)
            {
                if(fileLayout_Value[linenum].size() == 2){
                    if(fileLayout_Value[linenum][1] == serverName){
                        say(fileLayout_Value[linenum][0]);
                        return 0;
                    }
                }
            }
            say("Server '"+serverName+"' not found");
            return 1;

        }
        else if(rename){
            //This is here becuase I wanted to rename a server with
            //a given name to [name] without having to specify the IP
            /*if(ip.empty() && !extraSrvName.empty()){
                for(unsigned int linenum = 0; linenum < lines.size(); linenum++){
                    if(fileLayout_Value[linenum].size() == 2){
                        if (fileLayout_Value[linenum][1] == serverName) {
                            fileLayout_Value[linenum][1] = extraSrvName;
                            writeFile(fileLayout_Value, sshAliasFile);
                            say("Changed alias of "+fileLayout_Value[linenum][0]+" from '"+serverName+"' to '"+extraSrvName+"'");
                            return 0;
                        }
                    }
                }
                say("Could not find server '"+serverName+"'");
                return 1;
            } */
            for(unsigned int linenum = 0; linenum < lines.size(); linenum++)
            {
                if(fileLayout_Value[linenum][0] == ip){
                    std::string oldname = "";
                    if(fileLayout_Value[linenum].size() == 2){
                        if(fileLayout_Value[linenum][1] != serverName){
                            oldname = fileLayout_Value[linenum][1];
                            fileLayout_Value[linenum][1] = serverName;
                        } else {
                            say(ip+" already has the alias '"+serverName+"'");
                            return 1;
                        }
                    } else {
                        fileLayout_Value[linenum].push_back(serverName);
                    }
                    writeFile(fileLayout_Value, sshAliasFile);
                    say(oldname.empty() ? ("Added alias '"+serverName+"' to "+ip) : ("Changed alias of "+ip+" from '"+oldname+"' to '"+serverName+"'"));
                    return 0;
                }
            }
            say("Server ip not found");
            return 1;
        } else if (addserv){
            if(ip.empty()){
                say("Please specify an IP");
                return 1;
            }

            //check to see if ip/alias is already taken
            for(unsigned int linenum = 0; linenum < lines.size(); linenum++)
            {
                //if ip doesnt exist on line
                if(!(fileLayout_Value[linenum][0] == ip)){
                    if(fileLayout_Value[linenum].size() == 2 && !serverName.empty()){
                        if(fileLayout_Value[linenum][1] == serverName){
                            say("Server alias already taken");
                            return 1;
                        }
                    }
                } else {
                    if(serverName.empty()){
                        say("Server ("+ip+") already exists");
                        return 1;
                    } else {
                        //avoid segmentation fault
                        if(fileLayout_Value[linenum].size() == 2){
                            if(fileLayout_Value[linenum][1] == serverName){
                                say("Server already has that alias");
                                return 1;
                            }
                        } else {
                            fileLayout_Value[linenum].push_back(serverName);
                            writeFile(fileLayout_Value, sshAliasFile);
                            say("IP "+ip+" now has alias "+serverName);
                        }
                    }
                }
            }
            vector<std::string> vec;
            vec.push_back(ip);
            if(!serverName.empty()){
                vec.push_back(serverName);
            }
            fileLayout_Value.push_back(vec);
            writeFile(fileLayout_Value, sshAliasFile);
            say("IP '"+ip+'\''+(serverName.empty() ? "" : "and alias '"+serverName+"'")+" added");
            return 0;
        }

        else if(delserv){
            if(ip.empty()){
                say("Please specify an IP");
                return 1;
            } else {
                //check to see if ip/alias is already taken
                for(unsigned int linenum = 0; linenum < lines.size(); linenum++)
                {
                    //if ip doesnt exist on line
                    if(fileLayout_Value[linenum][0] == ip){
                        fileLayout_Value.erase(fileLayout_Value.begin()+linenum);
                        writeFile(fileLayout_Value, sshAliasFile);
                        say("IP '"+ip+'\''+" deleted");
                        return 0;
                    }
                }
            }
            say("Could not find "+(ip.empty() ? "" : "IP "+ip)+(serverName.empty() ? "" : ((ip.empty() ? "" :" and ")+("'"+serverName+"'"))));
            return 1;
        }
    }
    return 0;
}
