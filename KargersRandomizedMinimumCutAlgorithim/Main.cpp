#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <SDL.h>
const int ScreenWidth = 1980;
const int ScreenHeight = 1080;
SDL_Window* window = SDL_CreateWindow("Kargers", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenWidth, ScreenHeight, 0);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;
using namespace std;
// a structure to represent a unweighted edge in graph
struct AssignmentStructure
{
    int first;
    vector<int>seconds;
};
struct Edge
{
    int src, dest;
};
class Vector2
{
public:
    int x = 0;
    int y = 0;
};
// a structure to represent a connected, undirected
// and unweighted graph as a collection of edges.
struct Graph
{
    // V-> Number of vertices, E-> Number of edges
    int V, E;

    // graph is represented as an array of edges.
    // Since the graph is undirected, the edge
    // from src to dest is also edge from dest
    // to src. Both are counted as 1 edge here.
    Edge* edge;

    Vector2* Points;

    vector<int> CutIndexes;
};

// A structure to represent a subset for union-find
struct subset
{
    int parent;
    int rank;
};

// Function prototypes for union-find (These functions are defined
// after kargerMinCut() )
int find(subset subsets[], int i);
void Union(subset subsets[], int x, int y);

// A very basic implementation of Karger's randomized
// algorithm for finding the minimum cut. Please note
// that Karger's algorithm is a Monte Carlo Randomized algo
// and the cut returned by the algorithm may not be
// minimum always
int kargerMinCut(Graph* graph)
{
    int V = graph->V, E = graph->E;
    Edge* edge = graph->edge;

    subset* subsets = new subset[V];

    for (int v = 0;v < V;v++)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }
    int vertices = V;

    while (vertices > 2)
    {
        int i = rand() % E;
        int subset1 = find(subsets, edge[i].src);
        int subset2 = find(subsets, edge[i].dest);

        if (subset1 == subset2)
            continue;

        else
        {
            vertices--;
            Union(subsets, subset1, subset2);
        }

    }
    int cutedges = 0;
    for (int i = 0; i < E; i++)
    {
        int subset1 = find(subsets, edge[i].src);
        int subset2 = find(subsets, edge[i].dest);
        if (subset1 != subset2)
        {
            graph->CutIndexes.push_back(i);
            cutedges++;
        }
    }

    return cutedges;
}

// A utility function to find set of an element i
// (uses path compression technique)
int find(subset subsets[], int i)
{
    // find root and make root as parent of i
    // (path compression)
    if (subsets[i].parent != i)
        subsets[i].parent =
        find(subsets, subsets[i].parent);

    return subsets[i].parent;
}

// A function that does union of two sets of x and y
// (uses union by rank)
void Union(subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    // Attach smaller rank tree under root of high
    // rank tree (Union by Rank)
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;

    // If ranks are same, then make one as root and
    // increment its rank by one
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Creates a graph with V vertices and E edges
Graph* createGraph(int V, int E)
{
    Graph* graph = new Graph;
    graph->V = V;
    graph->E = E;
    graph->edge = new Edge[E];
    graph->Points = new Vector2[graph->V];
    for (int i = 0;i < V;i++)
    {
        graph->Points[i].x = rand() % ScreenWidth;
        graph->Points[i].y = rand() % ScreenHeight;
    }
    return graph;
}
void SDL_RenderFillCircle(int x, int y, int radius)
{
    int offsetx, offsety, d;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;

    while (offsety >= offsetx) {

        SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
            x + offsety, y + offsetx);
        SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
            x + offsetx, y + offsety);
        SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
            x + offsetx, y - offsety);
        SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
            x + offsety, y - offsetx);


        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}
vector<string>removeDupWord(string str)
{
    vector<string> ReturnVal;
    string word = "";
    for (int i = 0;i<str.size();i++)
    {
        if (str[i] == '\t')
        {
            ReturnVal.push_back(word);
            word = "";
        }
        else {
            word = word + str[i];
        }
    }
    return ReturnVal;
}
void print(std::vector<string> const& input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
}
// Driver program to test above functions
int main(int argc, char* argv[])
{
    srand((unsigned)time(0));

    bool quit = false;

    vector<string>Lines;
    vector<vector<string>>Words;
    vector<AssignmentStructure>GraphInfo;
    vector<Edge> Edges;
    ifstream inputFile("kargerMinCut.txt");
    string line;
    string num;
    Edge edge;
    AssignmentStructure numStruct;
    bool IsFirstNum = true;
    while (getline(inputFile, line))
    {
        //cout << line << endl;
        Lines.push_back(line);
    }
    inputFile.close();
    for (int i = 0;i < Lines.size();i++)
    {
        Words.push_back(removeDupWord(Lines[i]));
    }
    for (int i = 0;i < Words.size();i++)
    {
        numStruct.first = stoi(Words[i][0]);
        for (int j = 1;j < Words[i].size();j++)
        {
            edge.src = numStruct.first-1;
            edge.dest = stoi(Words[i][j])-1;
            Edges.push_back(edge);
            //numStruct.seconds.push_back(stoi(Words[i][j]));
        }
        //GraphInfo.push_back(numStruct);
        //numStruct.seconds.empty();
    }
    int V = 200;  // Number of vertices in graph
    int E = Edges.size();  // Number of edges in graph
    struct Graph* graph = createGraph(V, E);
    int a = 0;
    for (Edge edge : Edges)
    {
        graph->edge[a].src = edge.src;
        graph->edge[a].dest = edge.dest;
        a++;
    }
    printf("\nCut found by Karger's randomized algo is %d\n",
        kargerMinCut(graph));
    while (!quit)
    {
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int j = 0;j < E;(j++) % E - 1)
        {
            for (int s = 0;s < graph->CutIndexes.size();s++)
            {
                if (j == graph->CutIndexes[s])
                {
                    SDL_SetRenderDrawColor(renderer,255, 0, 0, 255);
                }
            }
            SDL_RenderDrawLine(renderer, graph->Points[graph->edge[j].src].x, graph->Points[graph->edge[j].src].y, graph->Points[graph->edge[j].dest].x, graph->Points[graph->edge[j].dest].y);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        for (int i = 0;i < V;i++)
        {
            SDL_RenderFillCircle(graph->Points[i].x, graph->Points[i].y, 6);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}