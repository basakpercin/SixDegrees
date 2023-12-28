/*
 * SixDegrees.cpp
 *
 * COMP 15 Project 2: Six Degrees of Collaboration
 * 
 * Created by Basak Percin, July-August 2022
 *
 * An implementation of SixDegrees.h class
 */

#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
#include <exception>

#include "SixDegrees.h"
#include "Artist.h"
#include "CollabGraph.h"

using namespace std;


/* SixDegrees Constructor
 *    Purpose: Initializes a SixDegrees instance.
 * Parameters: None.
 *    Returns: None
 */
SixDegrees::SixDegrees()
{
    graph = CollabGraph();
    output = &cout;
}

/* populateGraph
 *    Purpose: Populates CollabGraph object.
 * Parameters: vector of artist pointers containing artists in dataFile.
 *    Returns: None
 */

void SixDegrees::populateGraph(vector<Artist *> artistList)
{
    for (Artist *artistToAdd : artistList) {
        graph.insert_vertex(*artistToAdd);
    }

    setEdges(artistList);
}

/* setEdges
 *    Purpose: Finds the collaborations and inserts as edges.
 * Parameters: vector of artist pointers containing artists in dataFile.
 *    Returns: None
 */

void SixDegrees::setEdges(vector<Artist *> artistList)
{
    for (auto out = artistList.begin(); out != artistList.end(); out++) {
        for (auto in = artistList.begin(); in != artistList.end(); in++) {
            if (out == in)
            {
                continue;
            }

            string edgeName = "";
            edgeName = (*out)->get_collaboration(**in);

            if(edgeName == "") {
                continue;
            }
            graph.insert_edge(**out, **in, edgeName);
        }
    }
}

/* print()
 *    Purpose: Prints the graph.
 * Parameters: vector of artist pointers containing artists in dataFile.
 *    Returns: None
 */

void SixDegrees::print()
{
    graph.print_graph(*output);
}

/* clear_metadata()
 *    Purpose: Accesses clear_metadata function of CollabGraph.
 * Parameters: None.
 *    Returns: None
 */
void SixDegrees::clear_metadata()
{
    graph.clear_metadata();
    pathFound = false;
}

/* bfs_function()
 *    Purpose: Finds and prints the shortest path between 2 artists.
 * Parameters: 2 artists, source and destination.
 *    Returns: None
 */
void SixDegrees::bfs_function(const Artist &from, const Artist &to)
{
    queue<Artist> q;
    vector<Artist> neighbors;

    if (not validate_from_to(from, to)) {
        return;
    }

    q.push(from);
    while (!q.empty()) {
        Artist curArtist = q.front();
        q.pop();
        if (graph.is_marked(curArtist)) {
            continue; //discard
        }
        graph.mark_vertex(curArtist);

        if (curArtist == to) {
            print_path(from,to);
            clear_metadata();
            return;
        }
        neighbors = graph.get_vertex_neighbors(curArtist);
        for (size_t i = 0; i < neighbors.size(); i++) {
            if (graph.is_marked(neighbors[i]) == false) {
                graph.set_predecessor(neighbors[i], curArtist);
                q.push(neighbors[i]);
            }
        }
    }

    print_path(from, to);
    clear_metadata();
    return;
}

/* dfs_function()
 *    Purpose: Finds and prints a path between 2 artists.
 * Parameters: 2 artists, source and destination.
 *    Returns: None
 */
void SixDegrees::dfs_function(const Artist &from, const Artist &to)
{
    if (not validate_from_to(from, to)) {
        return;
    }

    clear_metadata();
    Artist nullArtist = Artist();
    dfs_private(from, nullArtist, from, to);
    if (!pathFound) {
        print_path(from, to);
        return;
    }
}

/* not_function()
 *    Purpose: Excludes artists in the list of artists to be excluded then
 *          finds the shortest path between the 2 artists (from, to).
 * Parameters: 2 artists, source and destination (from, to) and a list of 
            artists which are going to be excluded.
 *    Returns: None
 */
void SixDegrees::not_function(const Artist &from, const Artist &to, 
                                                vector<Artist> notList)
{
    bool returnValue = validate_from_to(from, to);
    bool validateValue = validateList(notList);

    if (!returnValue or !validateValue) {
        return;
    }

    for (size_t i = 0; i < notList.size(); i++) {
        exclude(notList[i]);
    }

    bfs_function(from, to);
}



/* exclude()
 *    Purpose: Marks artists to exclude as visited.
 * Parameters: Artist to exclude.
 *    Returns: None
 */
void SixDegrees::exclude(const Artist &exclude)
{
    graph.mark_vertex(exclude);
}



/* configureOutput()
 *    Purpose: Configures the output so its easier to cout and write to file.
 * Parameters: An ostream to print to.
 *    Returns: None
 */
void SixDegrees::configureOutput(ostream *out)
{
    output = out;
}

/* print_path()
 *    Purpose: Prints the path between 2 artists.
 * Parameters: 2 artists, source and destination.
 *    Returns: None
 */
void SixDegrees::print_path(const Artist &from, const Artist &to)
{
    stack<Artist> reportedPath = graph.report_path(from, to);
    if (reportedPath.empty()) {
        *output << "A path does not exist between "
                << "\"" << from.get_name() << "\"" << " and "
                << "\"" << to.get_name() << "\"." << endl;
        return;
    } 

    while (reportedPath.size() > 1) {
        Artist a = reportedPath.top();
        reportedPath.pop();
        Artist b = reportedPath.top();

        *output << "\"" << a.get_name() << "\"" << " collaborated with "
                << "\"" << b.get_name() << "\"" << " in "
                << "\"" << graph.get_edge(a, b) << "\"." << endl;
        
    }
    *output << "***" <<endl;
}

/* validate_from_to()
 *    Purpose: Checks if source and destination artists are in the graph.
 * Parameters: 2 artists, source and destination.
 *    Returns: boolean
 */
bool SixDegrees::validate_from_to(const Artist &from, const Artist &to) {

    bool returnValue = true;
    if (graph.is_vertex(from) == false) {
        *output << "\"" << from.get_name() << "\"" 
        << " was not found in the dataset :(" << endl;
        returnValue = false;
    }

    if (graph.is_vertex(to) == false) {
        *output << "\"" << to.get_name() << "\"" 
        << " was not found in the dataset :(" << endl;
        returnValue = false;
    }
    return returnValue;
}

/* validateList()
 *    Purpose: Takes the list of artists to be excluded and iterates over each
            artist to check whether that artist is a artist in the dataset.
 * Parameters: Artist list
 *    Returns: boolean
 */

bool SixDegrees::validateList(vector<Artist> List)
{
    bool validateValue = true;
    for (size_t i = 0; i < List.size(); i++) {
        if (validateExclude(List[i]) == false) {
            validateValue = false;
        }
    }
    return validateValue;
}


/* validateExclude()
 *    Purpose: Checks if artist to exclude is in the dataset.
 * Parameters: Artist.
 *    Returns: boolean
 */
bool SixDegrees::validateExclude(const Artist &exclude)
{
    bool validateValue = true;
    if (graph.is_vertex(exclude) == false) {
        *output << "\"" << exclude.get_name() << "\"" 
        << " was not found in the dataset :(" << endl;
        validateValue = false;
    }
    return validateValue;
}

/* dfs_private()
 *    Purpose: Private part of dfs function, finds and prints a path between 
               2 artists.
 * Parameters: 4 artists, curArtist, parent, source and destination.
 *    Returns: None
 */
void SixDegrees::dfs_private(const Artist &curArtist, const Artist &parent, 
                                         const Artist &from, const Artist &to)
{
    vector<Artist> neighbors;
    graph.mark_vertex(curArtist);
    if (parent.get_name() != "") {
        graph.set_predecessor(curArtist, parent);
    }
    if (curArtist == to) {
        print_path(from, to);
        pathFound = true;
        return;
    }

    neighbors = graph.get_vertex_neighbors(curArtist);
    for (size_t i = 0; i < neighbors.size(); i++) {
        if (!graph.is_marked(neighbors[i])) {
            if (!pathFound) {
                dfs_private(neighbors[i], curArtist, from, to);
            }
        }
    }
}