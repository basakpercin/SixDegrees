/*
 * test_main.cpp
 * CS15 Project 2 - the Six Degrees of Collaboration
 * 
 * by Basak Percin, July - August 2022
 *
 * An implementation of the tests for the Six Degrees project.
 * 
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

vector<Artist*> populate_sixDegrees(SixDegrees *sixDegree, string filename) {
    ifstream dataFile(filename);
    string line;
    vector<Artist *> artistList;

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
    } else {
        cerr << "Error: could not open file " << filename << endl;
        exit(0);
    }
    return artistList;
}

vector<Artist> makeNotList(vector<Artist*> &artistList, int start, int end) {
    vector<Artist> result;
    for (size_t i=start; i<=end; i++) {
        result.push_back(*artistList[i]);
    }
    return result;
}

int main(int argc, char *argv[])
{
    SixDegrees *sixDegree = new SixDegrees();
    vector<Artist*> artists =populate_sixDegrees(sixDegree, "artists.txt");
    vector<Artist> emptyNotArray;
    Artist nonExistentArtist = Artist("xx");
    vector<Artist> nonExistentArtistArray;
    nonExistentArtistArray.push_back(nonExistentArtist);
    nonExistentArtistArray.push_back(nonExistentArtist);
    nonExistentArtistArray.push_back(nonExistentArtist);

    // Test same artist
    sixDegree->dfs_function(*artists[0], *artists[0]);
    sixDegree->bfs_function(*artists[0], *artists[0]);
    sixDegree->not_function(*artists[0], *artists[0], emptyNotArray);
    
    // Connected artists but one not
    sixDegree->not_function(*artists[0], *artists[1], makeNotList(artists,1,1));

    // Connected artists both not
    sixDegree->not_function(*artists[0], *artists[1], makeNotList(artists,0,1));

    // Non-existent artist with a valid artist
    sixDegree->dfs_function(*artists[0], nonExistentArtist);
    sixDegree->bfs_function(*artists[0], nonExistentArtist);
    sixDegree->not_function(*artists[0], nonExistentArtist, emptyNotArray);

    // Not with non existent artists
    sixDegree->not_function(*artists[0], *artists[1], nonExistentArtistArray);

    // HAPPY CASES
    sixDegree->dfs_function(*artists[0], *artists[4]);
    sixDegree->dfs_function(*artists[0], *artists[3]);
    sixDegree->dfs_function(*artists[0], *artists[2]);

    sixDegree->bfs_function(*artists[0], *artists[4]);
    sixDegree->bfs_function(*artists[0], *artists[3]);
    sixDegree->bfs_function(*artists[0], *artists[2]);

    sixDegree->not_function(*artists[0], *artists[4],  makeNotList(artists,1,3));
    sixDegree->not_function(*artists[0], *artists[4],  makeNotList(artists,2,3));
    sixDegree->not_function(*artists[1], *artists[2],  makeNotList(artists,3,4));
    sixDegree->not_function(*artists[1], *artists[2],  emptyNotArray);
}
