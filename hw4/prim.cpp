#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Vertex {
    public:
        Vertex(int key) : key(key), inTree(false) {}
        Vertex() {}
        const int getKey() const {return key;}
        const bool isInTree() const {return inTree;}
        bool operator<(const Vertex& rhs) const {
            return key < rhs.getKey();
        }
        bool operator==(const Vertex& rhs) const {
            return key == rhs.getKey();
        }
    private:
        int key;
        bool inTree;
};

namespace std {
    template <>
    struct hash<Vertex> {
        size_t operator()(const Vertex& v) const {
            return hash<int>()(v.getKey());
        }
    };
}

class Edge {
    public:
        Edge(Vertex& l, Vertex& r, int weight) : l(l), r(r), weight(weight) {}
        const Vertex& getLeftVertex() const {return l;}
        const Vertex& getRightVertex() const {return r;}
        const int getWeight() const {return weight;}
    private:
        Vertex& l;
        Vertex& r;
        int weight;
};

int num_vertices = 0, num_edges = 0;
unordered_map<Vertex, vector<Edge> > adj_list;

Vertex init_map(int key) {
    Vertex tmp = Vertex(key);
    unordered_map<Vertex, vector<Edge> >::iterator it = adj_list.find(tmp);
    if(it == adj_list.end()) {
        vector<Edge> edge_list = vector<Edge>();
        adj_list[tmp] = edge_list;
        return tmp;
    } else
        return (*it).first;
}

void read_input() {
    int tmp = 0, i = 0;
    bool firstLine = true;
    Vertex r, l;
    while(scanf("%d", &tmp) != -1) {
        if(firstLine) {
            if(i == 0) {
                num_vertices = tmp;
                i++;
            } else {
                num_edges = tmp;
                i = 0;
                firstLine = false;
            }
            continue;
        }
        if(i == 0) {
            l = init_map(tmp);
            i++;
        } else if(i == 1) {
            r = init_map(tmp);
            i++;
        } else if(i == 2) {
            Edge e = Edge(l, r, tmp);
            adj_list[l].push_back(e);
            adj_list[r].push_back(e);
            i = 0;
        } else 
            throw invalid_argument("i out of range");
        
    }
}



int main(int argc, char** argv) {

}