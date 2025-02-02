// heightmapProcessor.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 
// Should use the latest version of C++.
//

#include "pugixml.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <map>
#include <vector>
#include "heightmapTile.h"
#include "heightmapIsland.h"
#include <list>


pugi::xml_document baseDoc; 

enum fileCmdEnums{ update, changeDir, info, invalid, quit, singleFileDir };

static std::filesystem::path curDir;

std::map<std::string, fileCmdEnums> stringToEnumCMD{ 
    {"update", fileCmdEnums::update}, 
    {"changeDir", fileCmdEnums::changeDir},
    {"cd", fileCmdEnums::changeDir},
    {"sf", fileCmdEnums::singleFileDir},
    {"info", fileCmdEnums::info}, // Not sure this is the best name for it, since it's going to summarize a bunch of data.
    {"quit", fileCmdEnums::quit}

};

fileCmdEnums checkParaCMDCommand(std::string asCMD) // A command with parameters, instead of just a plain one like update, info, etc.
{
   /*
   * Don't blame me, C++ hates Switch statements that aren't enum or something.
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
    if (forPath.has_extension()) // I think this was due to if I just had the condition before it would throw false or something were there not an extension.
    {
        return forPath.extension().string().compare("\".xml\"");
    }
    
    return false;
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

    }

    return true;
}

std::string XMLCoordsCenter( std::filesystem::directory_entry )
{
    std::string Center = "";

    return Center;
}


void passByRefTest(std::vector<heightmapTile>& ourRef)
{

}


/*
Planned to update the current directory. 
It should move single files from a folder up to their parent directory if they're the only ones present then delete said folder.
Though if I simply extract the string of zips it should simply do that anyways so maybe that will be left out.
It should also rename the files to have their coordinates center too for simplicity.
*/

void updateDirFiles()
{
    int numFolders = 0;

    std::list<heightmapTile> tiles;

    if (std::filesystem::exists(curDir) && !curDir.empty())
    {
        for ( std::filesystem::directory_entry inCurDir : std::filesystem::directory_iterator{ curDir })
        {
            if (isXMLFile(inCurDir))
            {

                std::cout << inCurDir.path(); std::cout << "\n";

                pugi::xml_document tDoc;
                pugi::xml_parse_result loadResult = tDoc.load_file( std::filesystem::absolute( inCurDir.path() ).c_str() );

                if (loadResult.status == pugi::status_ok)
                {
                    heightmapTile tTile = heightmapTile( &tDoc );
                    tiles.emplace_back(tTile);
                }

                if (loadResult.status != pugi::status_ok)
                {
                    std::cout << "Error opening file "; std::cout << inCurDir.path(); std::cout << "\n";
                }

            }
        }

        std::cout << "Load complete. ";  std::cout << tiles.size(); std::cout << " were processed.\n";

        // heightmapTile* baseRef = tiles.at(0).get();// processedTiles.at(0);

        heightmapTile &baseRef = tiles.front();
        
        for ( heightmapTile &aTile : tiles) // Make sure all the files are formatted the same.
        {
            if (!aTile.sameFormat(baseRef))
            {
                std::cout << "Discrepancy found!";
                return; 
            }

            aTile.findNeighbors( tiles ); // Find our neighbors
        }

        std::cout << "Done!\n";

        // std::list<heightmapTile> firstIslandTiles; 

        heightmapIsland islandHandler = heightmapIsland(tiles); // = heightmapIsland::heightmapIsland(tiles);

        // std::cout << "There are "; std::cout << baseRef.allConnected( firstIslandTiles ).size(); std::cout << " tiles connected to first";


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

        case fileCmdEnums::quit:
            return;

        default:
            break;


    }
        


    getCMD();

}

int main()
{
    std::cout << "Directory Not Set!\n";

    /*uint16_t testNum = 60000;

    uint8_t red, green;

    red = heightmapIsland::redFromHeight(testNum);
    green = heightmapIsland::greenFromHeight(testNum);

    heightmapIsland::generateTest();*/

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
