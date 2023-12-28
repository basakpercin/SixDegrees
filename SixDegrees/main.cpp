/*
 * main.cpp
 *
 * COMP 15 Project 2: Six Degrees of Collaboration
 * 
 * by Basak Percin, July-August 2022
 *
 * An implementation of SixDegrees simulator.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include "Artist.h"
#include "CollabGraph.h"
#include "SixDegrees.h"

using namespace std;

/* handleLine
 *    Purpose: Handles commands for the sixDegrees (bfs, dfs,not and quit).
 * Parameters: line string, vector<string> &commands, SixDegrees pointer.
 *    Returns: None
*/

void handleLine(string line, vector<string> &commands, vector<Artist> &notList,
                                                     SixDegrees *sixDegrees)
{
    if (line == "quit") {
        exit(0);
    }

    bool skipNot = false;
    if (commands.size() < 3) {
        if(commands.size() == 0 and !(line == "bfs" or line == "dfs" or 
            line == "not")) {
                *(sixDegrees->output) << line << " is not a command. "
                                      << "Please try again."  << endl;
                return;
            }
        if (commands.size() == 2) {
            skipNot = true;
        }
        commands.push_back(line);
    }

    if (commands.size() != 3) {
        return;
    }

    Artist s = Artist(commands[1]);
    Artist f = Artist(commands[2]);

    if (commands.front() == "bfs") {
        sixDegrees->bfs_function(s, f);
        commands.clear();
        return;
    }

    if (commands.front() == "dfs") {
        sixDegrees->dfs_function(s, f);
        commands.clear();
        return;
    }

    if (line == "*") {
        sixDegrees->not_function(s, f, notList);
        commands.clear();
        notList.clear();
        return;
    } else {
        if (!skipNot) {
        Artist toAdd = Artist(line);
        notList.push_back(toAdd);            
        }
    }
}

int main(int argc, char *argv[])
{
    // if no of command line arguments is not 1, 2, or 3:
    if (argc < 2 or argc > 4) {
        cerr << "Usage: ./SixDegrees dataFile [commandFile] [outputFile]" 
        << endl;
        exit(0);
    }
    
    ifstream dataFile(argv[1]);
    string line;
    vector<Artist *> artistList;
    SixDegrees *sixDegree = new SixDegrees();

    //opens dataFile, creates and populates graph
    if (dataFile.is_open()) {
        Artist *curArtist = new Artist();
        bool nameSet = false;
        while(getline(dataFile, line)) {
            if (line != "*") {
                if (!nameSet) {
                    curArtist->set_name(line);
                    nameSet = true;
                } else {
                    curArtist->add_song(line);
                }
            } else {
                nameSet = false;
                artistList.push_back(curArtist);
                curArtist = new Artist();
            }
        }
        sixDegree->populateGraph(artistList);
        // sixDegree->print();
        // exit(0);

    } else {
        cerr << "Error: could not open file " << argv[1] << endl;
        exit(0);
    }
    
    ofstream outputFile;
    if (argc == 4) {
        outputFile = ofstream(argv[3]);
        sixDegree->configureOutput(&outputFile);
    }

    vector<string> commands;
    vector<Artist> notList;

    if (argc >= 3) {
        ifstream commandFile(argv[2]);
        if(commandFile.is_open()) {
            while(getline(commandFile, line)) {
                handleLine(line, commands, notList, sixDegree);
            }

            commandFile.close();
        } else {
            cerr << "Error: could not open file " << argv[2] << endl;
            exit(0);
        }

    } else {
        while(getline(cin, line)) {
            handleLine(line, commands, notList, sixDegree);
        }
    }

    return 0;
}