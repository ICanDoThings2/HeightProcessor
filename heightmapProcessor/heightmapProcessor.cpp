// heightmapProcessor.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 
// Should use the latest version of C++.
//

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <map>
#include <vector>

enum fileCmdEnums{ update, changeDir, info, invalid };

std::filesystem::path curDir;

std::map<std::string, fileCmdEnums> stringToEnumCMD{ 
    {"update", fileCmdEnums::update}, 
    {"changeDir", fileCmdEnums::changeDir},
    {"cd", fileCmdEnums::changeDir},
    {"info", fileCmdEnums::info} // Not sure this is the best name for it, since it's going to summarize a bunch of data.

};

fileCmdEnums checkCMDCommand( std::string asCMD )
{

    fileCmdEnums toReturn = fileCmdEnums::invalid;

    try
    {
        toReturn = stringToEnumCMD.at(asCMD);
    }
    catch (const std::out_of_range& oor)
    {
        std::cout << "No command of that name found!\n";
        return fileCmdEnums::invalid;
    }

    return toReturn;
    
}

void changeToDir(std::string asDir)
{

}

void getCMD()
{
    std::cout << "Input your command\n";

    std::string asCmd ("");

    std::cin >> asCmd;

    switch ( checkCMDCommand(asCmd) )
    {
        case fileCmdEnums::update:
            std::cout << "Updating file names!\n";
            break;

        case fileCmdEnums::changeDir:
            std::string newPath = asCmd.substr(3);

            if (std::filesystem::exists(newPath))
            {
                curDir = newPath;
            }
            else
            {
                std::cout << "Path does not exist!";
            }

            break;


    }
        


    getCMD();

}

int main()
{
    std::filesystem::path;

    std::cout << "Directory Not Set!\n";

    /*for (const auto& file : )
    {

    }*/

    getCMD();




}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
