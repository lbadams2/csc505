#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int num_vertices = 0, num_edges = 0;
vector<bool> in_tree;
unordered_map<int, vector<int> > adj_list;
unordered_map<int, unordered_map<int, int> > weight_map; 

void init_map(int vertex_index) {
    unordered_map<int, vector<int> >::iterator it = adj_list.find(vertex_index);
    if(it == adj_list.end()) {
        vector<int> edge_list = vector<int>();
        adj_list[vertex_index] = edge_list;
        weight_map[vertex_index] = unordered_map<int, int>();
    }
}

void read_input() {
    int tmp = 0, i = 0;
    bool firstLine = true;
    int l, r;
    while(scanf("%d", &tmp) != -1) {
        if(firstLine) {
            if(i == 0) {
                num_vertices = tmp;
                in_tree = vector<bool>(tmp);
                adj_list.reserve(tmp);
                weight_map.reserve(tmp);
                i++;
            } else {
                num_edges = tmp;
                i = 0;
                firstLine = false;
            }
            continue;
        }
        if(i == 0) {
            l = tmp;
            init_map(tmp);
            i++;
        } else if(i == 1) {
            r = tmp;
            init_map(tmp);
            i++;
        } else if(i == 2) {
            adj_list[l].push_back(r);
            weight_map[l][r] = tmp;
            adj_list[r].push_back(l);
            weight_map[r][l] = tmp;
            i = 0;
        } else 
            throw invalid_argument("i out of range");
    }
}

void build_component(int vertex, vector<int>& component_vertices) {
    for(int v : adj_list[vertex]) {
        if(!in_tree[v]) {
            component_vertices.push_back(v);
            in_tree[v] = true;
            build_component(v, component_vertices);
        }
    }
}

int main(int argc, char** argv) {
    read_input();
    vector<vector<int> > components;
    for(int i = 0; i < num_vertices; i++) {
        if(!in_tree[i]) {
            vector<int> component = vector<int>();
            build_component(i, component);
            components.push_back(component);
            in_tree[i] = true;
        }
    }
    double ratio = (double)num_edges / (double)num_vertices;
    int bf = 1;
    while(bf < ratio) {
            bf = bf << 1;
    }
    if(bf < 2)
        bf = 2;
    cout << bf << " " << components.size() << "\n";
}