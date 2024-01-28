#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <array>
#include <algorithm>

// #define USE_FILE

#ifdef USE_FILE
    #include <fstream>    
#endif

const int IP_LEN = 4;

typedef std::vector<std::string> vector_of_strings;
typedef std::array<int, IP_LEN> ip_address;
typedef std::vector<ip_address> vector_of_ips;

std::vector<vector_of_strings> ip_pool;

vector_of_ips ips;

int print_ip_count = 0;

vector_of_strings Split(const std::string &str, char delimeter)
{
    vector_of_strings tokens;
    std::stringstream stream(str);
    std::string token;
    
    while (std::getline(stream, token, delimeter))
        tokens.push_back(token);
    
    return tokens;
}

void AddIPLine(std::string line) 
{
    if (line.empty()) return;
    
    vector_of_strings v = Split(line, '\t');
    if (v.empty()) return;
    
    vector_of_strings ip = Split(v.at(0), '.');
    if (ip.size() == IP_LEN)
        ip_pool.push_back(ip);
}

#ifdef USE_FILE

void ReadIpPoolFromFile(std::string filename)
{
    std::ifstream file(filename);
    std::string line;    

    while (std::getline(file, line))
        AddIPLine(line);    

    file.close();
}

#else

void ReadIpPoolFromCIN()
{
    for(std::string line; std::getline(std::cin, line);)
        AddIPLine(line);
}

#endif

void PrintIP(const ip_address& IP) 
{
    for (int i(0); i < IP_LEN; ++i)
        std::cout << IP[i] << ((i == (IP_LEN - 1)) ? "" : ".");
    std::cout << std::endl;
    
    print_ip_count++;
}

void FilterByByte(const int aByte0)
{
    for (const auto& ip : ips)
        if (ip[0] == aByte0)
            PrintIP(ip);
}

void FilterByByte(const int aByte0, const int aByte1)
{
    for (const auto& ip : ips)
        if ((ip[0] == aByte0) && (ip[1] == aByte1))
            PrintIP(ip);
}

void FilterByAnyByte(const int aByte)
{
    for (const auto& ip : ips)
        for (const auto& octet : ip)
            if (octet == aByte) {
                PrintIP(ip);
                break;
            }            
}

int main()
{
    try
    {

        #ifdef USE_FILE
            ReadIpPoolFromFile("ip_filter.tsv");
        #else    
            ReadIpPoolFromCIN();
        #endif       

        for (const auto& line : ip_pool)
        {
            ip_address buf;
            for (int i(0); i < IP_LEN; ++i)
                buf[i] = std::stoi(line.at(i)) & 0xFF;
            ips.push_back(buf);
        }    

        std::sort(ips.begin(), ips.end(), [](auto a, auto b)
        {
            for (int i = 0; i < IP_LEN; ++i)                        
                if (a[i] != b[i])
                    return a[i] > b[i];
            return false;
        });

        for(auto ip = ips.cbegin(); ip != ips.cend(); ++ip)
            PrintIP(*ip);

        FilterByByte(1);            
        FilterByByte(46, 70);
        FilterByAnyByte(46);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }    

    return 0;
}