#include "graph.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include "BinaryPQ.hpp"
#include "Eecs477PQ.hpp"
#include "PairingPQ.hpp"
#include "FibonacciPQ.hpp"
#include "SmoothPQ.hpp"
#include <chrono>

using namespace std;

int vec_find(vector<pair<double, int>> &vec, int i) {
    for (int j = 0; j < vec.size(); ++j) {
        if (vec[j].second == i) {
            return j;
        }
    }
    return -1;
}

unordered_map<int, PairingPQ<pair<double, int>>::Node *> pairing_map;
unordered_map<int, FibonacciHeap<pair<double, int>>::node *> fib_map;
unordered_map<int, SmoothPQ<pair<double, int>>::Node *> smooth_map;

double euclid_dist(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
void run_pairing(Graph graph, GraphNode source, vector<double> &dist, vector<int> &prev,
                 PairingPQ<pair<double, int>> &pairing, ofstream &out, double density)
{
    ofstream file;
    file.open("pairing_output.txt");
    dist[0] = 0;
    prev[0] = 0;
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    while(!pairing.empty())
    {
        pair<double, int> u = pairing.top();//get min
        //        if (u.second == 1) {
        //            cout << "debug!" << endl;
        //        }
        graph.vertices[u.second].visited = true;
        pairing.pop();
        for (int i = 0; i < graph.vertices[u.second].adj.size(); ++i)
        {
            pair <double, int> v = graph.vertices[u.second].adj[i];
            if (v.second) {
                double alt = dist[u.second] + v.first;
                if (alt < dist[v.second])
                {
                    //update dist and prev
                    dist[v.second] = alt;
                    prev[v.second] = u.second;
                    //decrease key
                    //if (!graph.vertices[v.second].visited) {
                    pairing.updateElt(pairing_map[v.second], pair<double, int>(alt, v.second));
                    //}
                }
            }
        }
    }
    
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    file << "Printing pairing heap result:" << endl;
    for (int i = 0; i < (int)prev.size(); i++)
    {
        file << "Node index: " << i << endl;
        file << "Node prev: " << prev[i] << endl;
        file << "Node distance: " << dist[i] << endl << endl;
    }
    file.close();
    cout << "Pairing heap number of seconds: " << (duration / 1000.0) << "\n";
    out << density << " " << (duration / 1000.0) << endl;
    
}
void run_fibonacci(Graph graph, GraphNode source, vector<double> &dist, vector<int> &prev,
                   FibonacciHeap<pair<double, int>> &fib, ofstream &out, double density )
{
    ofstream file;
    file.open("fib_output.txt");
    dist[0] = 0;
    prev[0] = 0;
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    while(!fib.isEmpty())
    {
        pair<double, int> u = fib.getMinimum();//get min
        //        if (u.second == 1) {
        //            cout << "debug!" << endl;
        //        }
        graph.vertices[u.second].visited = true;
        fib.removeMinimum();
        for (int i = 0; i < graph.vertices[u.second].adj.size(); ++i)
        {
            pair <double, int> v = graph.vertices[u.second].adj[i];
            if (v.second) {
                double alt = dist[u.second] + v.first;
                if (alt < dist[v.second])
                {
                    //update dist and prev
                    dist[v.second] = alt;
                    prev[v.second] = u.second;
                    //decrease key
                    //if (!graph.vertices[v.second].visited) {
                    fib.decreaseKey(fib_map[v.second], pair<double, int>(alt, v.second));
                    //}
                }
            }
        }
    }
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    file << "Printing fibonacci heap result:" << endl;
    for (int i = 0; i < (int)prev.size(); i++)
    {
        file << "Node index: " << i << endl;
        file << "Node prev: " << prev[i] << endl;
        file << "Node distance: " << dist[i] << endl << endl;
    }
    file.close();
    cout << "Fibonacci heap number of seconds: " << (duration / 1000.0) << "\n";
    out << density << " " << (duration / 1000.0) << endl;
}

void run_smooth(Graph graph, GraphNode source, vector<double> &dist, vector<int> &prev, SmoothPQ<pair<double, int>> &smooth, ofstream &out, double density) {
    ofstream file;
    file.open("smooth_output.txt");
    dist[0] = 0;
    prev[0] = 0;
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    while(!smooth.empty())
    {
        pair<double, int> u = smooth.top();//get min
        //        if (u.second == 1) {
        //            cout << "debug!" << endl;
        //        }
        graph.vertices[u.second].visited = true;
        smooth.pop();
        for (int i = 0; i < graph.vertices[u.second].adj.size(); ++i)
        {
            pair <double, int> v = graph.vertices[u.second].adj[i];
            if (v.second) {
                double alt = dist[u.second] + v.first;
                if (alt < dist[v.second])
                {
                    //update dist and prev
                    dist[v.second] = alt;
                    prev[v.second] = u.second;
                    //decrease key TO IMPLEMENT
                    smooth.updateElt(smooth_map[v.second], pair<double, int>(alt, v.second));
                }
            }
        }
    }
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    
    cout << "Smooth heap number of seconds: " << (duration / 1000.0) << endl;
    file << "Printing snooth sheap result:" << endl;
    for (int i = 0; i < (int)prev.size(); i++) {
        file << "Node index: " << i << endl;
        file << "Node prev: " << prev[i] << endl;
        file << "Node distance: " << dist[i] << endl << endl;
    }
    file.close();
    out << density << " " << (duration / 1000.0) << endl;
}

void run_binary(Graph graph, GraphNode source, vector<double> &dist, vector<int> &prev,
                BinaryPQ<pair<double, int>> &binary,  ofstream &out, double density)
{
    ofstream file;
    file.open("binary_output.txt");
    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
    while(!binary.empty())
    {
        pair<double, int> u = binary.top();
        graph.vertices[u.second].visited = true;
        binary.pop();
        for (int i = 0; i < graph.vertices[u.second].adj.size(); ++i)
        {
            if (graph.vertices[u.second].adj[i].second)
            {
                pair<double, int> v= graph.vertices[u.second].adj[i];
                if (!graph.vertices[v.second].visited) {
                    binary.push(v);
                    graph.vertices[v.second].visited = true;
                }
                double alt = dist[u.second] + v.first;
                if (alt < dist[v.second])
                {
                    dist[v.second] = alt;
                    prev[v.second] = u.second;
                }
            }
        }
    }
    dist[0] = 0;
    prev[0] = 0;
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
    cout << "Binary heap number of seconds: " << (duration / 1000.0) << endl;
    file << "Printing binary heap result:" << endl;
    for (int i = 0; i < (int)prev.size(); i++) {
        file << "Node index: " << i << endl;
        file << "Node prev: " << prev[i] << endl;
        file << "Node distance: " << dist[i] << endl << endl;
    }
    file.close();
    out << density << " " << (duration / 1000.0) << endl;
}

int main() {
    cout << "Program is running" << endl;
    int num_nodes = 0;
    int threshold;
    ifstream fin;
    /*fin.open("/Users/premshen/Desktop/EECS 477/eecs477dijkstra/eecs477dijkstra/SF.csv");
     fin.open("SF.csv");
     int i;
     double x, y;*/
    
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    cout << "How many nodes? : " << endl;
    cin >> num_nodes;
    cout << "What is the threshold for edge cutoff? : " << endl;
    cin >> threshold;
    cout << "What is your x range?: " << endl;
    cin >> x_min >> x_max;
    cout << "What is your y range?: " << endl;
    cin >> y_min >> y_max;
    int rand_max_x = x_max - x_min;
    int rand_max_y = y_max - y_min;
    srand((unsigned)time(0));
    
    Graph graph;
    
    for (int i = 0; i < num_nodes; ++i) {
        GraphNode node;
        node.index = i;
        int x_rand = (rand() % (rand_max_x + 1)) + x_min;
        int y_rand = (rand() % (rand_max_y + 1)) + y_min;
        node.x_coord = x_rand;
        node.y_coord = y_rand;
        graph.vertices.push_back(node);
    }
    
    /*while(fin >> i >> x >> y)
     {
     //cout << "reading vertex" << endl;
     GraphNode n;
     n.x_coord = x;
     n.y_coord = y;
     n.index = i;
     num_nodes += 1;
     n.degree = 0;
     graph.vertices.push_back(n);
     }
     fin.close();
     cout << "finish reading vertex csv" << endl;
     //fin.open("/Users/premshen/Desktop/EECS 477/eecs477dijkstra/eecs477dijkstra/edges.csv");
     fin.open("edges.csv");
     int junk, u, v;
     double distance;
     while(fin >> junk >> u >> v >> distance)
     {
     pair<double, int> p(distance,v);
     pair<double, int> q(distance,u);
     graph.vertices[u].adj.push_back(p);
     graph.vertices[v].adj.push_back(q);
     graph.vertices[u].degree++;
     graph.vertices[v].degree++;
     }
     fin.close();
     cout << "finish reading edge csv" << endl;
     GraphNode n0;
     GraphNode n1;
     GraphNode n2;
     GraphNode n3;
     GraphNode n4;
     GraphNode n5;
     GraphNode n6;
     GraphNode n7;
     
     n0.index = 0;
     n1.index = 1;
     n2.index = 2;
     n3.index = 3;
     n4.index = 4;
     n5.index = 5;
     n6.index = 6;
     n7.index = 7;
     
     n0.degree = 0;
     n1.degree = 0;
     n2.degree = 0;
     n3.degree = 0;
     n4.degree = 0;
     n5.degree = 0;
     n6.degree = 0;
     n7.degree = 0;
     
     
     n0.x_coord = 0;
     n0.y_coord = 10;
     n1.x_coord = 2;
     n1.y_coord = 2;
     n2.x_coord = 3;
     n2.y_coord = 3;
     n3.x_coord = 1;
     n3.y_coord = 8;
     n4.x_coord = 8;
     n4.y_coord = 5;
     n5.x_coord = 9;
     n5.y_coord = 1;
     n6.x_coord = 3;
     n6.y_coord = 9;
     n7.x_coord = 4;
     n7.y_coord = 9;
     
     graph.vertices.push_back(n0);
     graph.vertices.push_back(n1);
     graph.vertices.push_back(n2);
     graph.vertices.push_back(n3);
     graph.vertices.push_back(n4);
     graph.vertices.push_back(n5);
     graph.vertices.push_back(n6);
     graph.vertices.push_back(n7);*/
    
    for (int i = 0; i < num_nodes; ++i) {
        for (int j = i + 1; j < num_nodes; ++j) {
            double dist = euclid_dist(graph.vertices[i].x_coord, graph.vertices[i].y_coord,
                                      graph.vertices[j].x_coord, graph.vertices[j].y_coord);
            if (dist <= threshold) {
                pair<double, int> p(dist,j);
                pair<double, int> q(dist,i);
                graph.vertices[i].adj.push_back(p);
                graph.vertices[j].adj.push_back(q);
                graph.vertices[i].degree++;
                graph.vertices[j].degree++;
            }
        }
    }
    double density = 0.0;
    for (int i = 0; i < graph.vertices.size(); ++i) density += graph.vertices[i].adj.size();
    density /= (num_nodes * (num_nodes - 1));
    cout << "Density:" << density << "\n";
    GraphNode source = graph.vertices[0];
    vector<double> dist(num_nodes, std::numeric_limits<double>::infinity());
    vector<double> dist1(num_nodes, std::numeric_limits<double>::infinity());
    vector<double> dist2(num_nodes, std::numeric_limits<double>::infinity());
    vector<double> dist3(num_nodes, std::numeric_limits<double>::infinity());
    
    vector<int> prev(num_nodes, -1);
    vector<int> prev1(num_nodes, -1);
    vector<int> prev2(num_nodes, -1);
    vector<int> prev3(num_nodes, -1);
    
    PairingPQ<pair<double,int>> *pairing = new PairingPQ<pair<double,int>>;
    BinaryPQ<pair<double, int>> *binary = new BinaryPQ<pair<double, int>>;
    FibonacciHeap<pair<double, int>> *fib = new FibonacciHeap<pair<double, int>>;
    SmoothPQ<pair<double, int>> *smooth = new SmoothPQ<pair<double, int>>;
    //    for (int i = 0; i < graph.vertices[0].adj.size();i++) {
    //        cout << graph.vertices[0].adj[i].second << endl;
    //    }
    for (int i = 0; i < num_nodes; i++)
    {
        int val = vec_find(graph.vertices[0].adj, i);
        if (i == 0)
        {
            pairing_map[i] = pairing->addNode(pair<double, int>(0.0, i));
            binary->push(pair<double, int>(0.0, i));
            fib_map[i] = fib->insert(pair<double, int>(0.0, i));
            smooth_map[i] = smooth->addNode(pair<double, int>(0.0, i));
        }
        else if (val != -1) {
            pairing_map[i] = pairing->addNode(graph.vertices[0].adj[val]);
            binary->push(graph.vertices[0].adj[val]);
            fib_map[i] = fib->insert(graph.vertices[0].adj[val]);
            smooth_map[i] = smooth->addNode(graph.vertices[0].adj[val]);
            
        } else {
            pairing_map[i] = pairing->addNode(pair<double, int>(numeric_limits<double>::infinity(), i));
            fib_map[i] = fib->insert(pair<double, int>(numeric_limits<double>::infinity(), i));
            smooth_map[i] = smooth->addNode(pair<double, int>(numeric_limits<double>::infinity(), i));
        }
    }
    /*for (auto i:pairing_map) {
     cout << i.first << " " << i.second->getElt().first << " " << i.second->getElt().second << endl;
     }
     cout << endl;*/
    
    
    for (int i = 0; i < graph.vertices[0].adj.size(); ++i) {
        dist[graph.vertices[0].adj[i].second] = graph.vertices[0].adj[i].first;
        prev[graph.vertices[0].adj[i].second] = 0;
        dist1[graph.vertices[0].adj[i].second] = graph.vertices[0].adj[i].first;
        prev1[graph.vertices[0].adj[i].second] = 0;
        dist2[graph.vertices[0].adj[i].second] = graph.vertices[0].adj[i].first;
        prev2[graph.vertices[0].adj[i].second] = 0;
        dist3[graph.vertices[0].adj[i].second] = graph.vertices[0].adj[i].first;
        prev3[graph.vertices[0].adj[i].second] = 0;
    }
    cout << "finish initializing map" << endl;
    /*for (auto i:pairing_map) {
     cout << i.first << " " << i.second->getElt().first << " " << i.second->getElt().second << endl;
     }
     cout << endl;
     
     for (auto i:smooth_map) {
     cout << i.first << " " << i.second->getElt().first << " " << i.second->getElt().second << endl;
     }*/
    
    cout << endl;
    /*for (int i = 0; i < dist.size();i++) {
     cout << dist[i] << endl;
     }*/
    //    for (auto i:smooth_map) {
    //        cout << i.first << " " << i.second->getElt().first << " " << i.second->getElt().second << endl;
    //    }
    //    cout << endl;
    
    ofstream out;
    out.open("time_output");
    out << "X Y" << endl;
    
    run_pairing(graph, source, dist2, prev2, *pairing, out, density);
    cout << "finish pairing" << endl;
    run_smooth(graph, source, dist, prev, *smooth, out, density);
    cout << "finish smooth" << endl;
    run_binary(graph, source, dist, prev, *binary, out, density);
    cout << "finish binary" << endl;
    run_fibonacci(graph, source, dist3, prev3, *fib, out, density);
    cout << "finish fibonacci" << endl;
    cout << "Done!" << endl;
    out.close();
    return 0;
}
