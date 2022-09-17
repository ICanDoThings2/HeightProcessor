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
#include "heightmapTile.h"

enum fileCmdEnums{ update, changeDir, info, invalid, quit, singleFileDir };

std::filesystem::path curDir;

std::map<std::string, fileCmdEnums> stringToEnumCMD{ 
    {"update", fileCmdEnums::update}, 
    {"changeDir", fileCmdEnums::changeDir},
    {"cd", fileCmdEnums::changeDir},
    {"sf", fileCmdEnums::singleFileDir},
    {"info", fileCmdEnums::info} // Not sure this is the best name for it, since it's going to summarize a bunch of data.

};

fileCmdEnums checkParaCMDCommand(std::string asCMD) // A command with parameters, instead of just a plain one like update, info, etc.
{
   /*
   * Don't blame me, C++ hates Switch statements that aren't enum or smthing.
   */

    if (asCMD.substr(0, 2).compare("cd") == 0)
    {
        return fileCmdEnums::changeDir;
    }

    return fileCmdEnums::invalid; // If we found no other case it fit.
}

fileCmdEnums checkCMDCommand( std::string asCMD )
{

    fileCmdEnums toReturn = fileCmdEnums::invalid;

    try
    {
        toReturn = stringToEnumCMD.at(asCMD);
    }
    catch (const std::out_of_range& oor)
    {
        return checkParaCMDCommand(asCMD);
    }

    return toReturn;
    
}



bool isXMLFile(std::filesystem::path forPath)
{
    if (forPath.has_extension())
    {
        return forPath.extension().string().compare("\".xml\"");
    }
    
    return false;
}

void makeTileOfFile(std::filesystem::directory_entry asFile)
{

    if ( isXMLFile( asFile.path() ) )
    {

    }

    return;
}

void renameFile( std::filesystem::directory_entry asFile )
{
    std::ifstream readingFile(asFile.path(), std::ifstream::in);

    std::string curLine;
    while (std::getline(readingFile, curLine))
    {

    }

}



/*
* Due to the way the data has been downloaded from the website, it's somethings only one file in a folder.
* That's quite silly, so this removes it after renaming it if it's in valid format.
*/

bool bSingleFileFolder( std::filesystem::directory_entry asFolder )
{
    std::filesystem::directory_entry firstFile;
    bool singleFile = true;

    if ( asFolder.is_directory())
    {
        for (auto const& inCurDir : std::filesystem::directory_iterator{ asFolder })
        {
            if ( !firstFile.exists() )
            {
                firstFile = inCurDir;
                continue;
            }
            else // There was already a file there, therefore it isn't a single file folder.
            {
                singleFile = false;
                return false; // If it's not single file folder, no point doing anything here. End of the story.
            }
        }

        asFolder.path().parent_path();
    }

    return true;
}

void renameToCoords()
{

}

void updateDirFiles()
{
    int numFolders = 0;

    if (std::filesystem::exists(curDir) && !curDir.empty())
    {
        for ( std::filesystem::directory_entry inCurDir : std::filesystem::directory_iterator{ curDir })
        {
            if ( bSingleFileFolder(inCurDir ) )
            {

            }
        }
    }
}

/*
* This displays directory info, if the directory is valid and not empty.
*/

void showDirInfo()
{
    if ( std::filesystem::exists(curDir) && !curDir.empty())
    {
        for (auto const& inCurDir : std::filesystem::directory_iterator{ curDir })
        {
            std::cout << std::filesystem::path(inCurDir).filename();
            std::cout << "\n";
        }
    }
}

void changeToDir(std::string asDir)
{
    curDir.assign(asDir);

    if (std::filesystem::exists( curDir ))
    {
        curDir = asDir;
        std::cout << "Dir set to " + asDir + "\n";
    }
    else
    {
        std::cout << asDir;
        std::cout << " is an Invalid path!\n";
    }
}

void getCMD()
{
    std::cout << "Input your command\n";

    std::string asCmd ("");

    std::getline(std::cin, asCmd); // To-do, replace with getline.

    switch ( checkCMDCommand(asCmd) )
    {

        case fileCmdEnums::info:
        {
            showDirInfo();
            break;
        }

        case fileCmdEnums::update:
        {
            updateDirFiles();
            break;
        }

        case fileCmdEnums::changeDir:
        {
            std::string newPath = asCmd.substr(3);
            changeToDir(newPath);
            break;
        }

        default:
            break;


    }
        


    getCMD();

}

int main()
{
    std::filesystem::path;

    std::cout << "Directory Not Set!\n";

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
