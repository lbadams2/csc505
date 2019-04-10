#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;

int num_vertices = 0, num_edges = 0;
vector<bool> in_tree;
unordered_map<int, vector<int> > adj_list;
unordered_map<int, unordered_map<int, int> > weight_map;

class MinHeap {
    public:
        MinHeap(int size, int branch_factor) {
            this->capacity = size;
            this->heap_array = vector<pair<int, int> >(size);
            this->branch_factor = branch_factor;
            this->heap_size = 0;
            this->vertex_location = vector<int>(size);
            int tmp = branch_factor;
            while(tmp >>= 1 != 0)
                lg_branches++;
        }
        
        int getSize() {return heap_size;}
        int firstChild(int i){return (i << lg_branches) + 1;}
        int lastChild(int i){return this->firstChild(i) + (branch_factor - 1);}
        int parent(int i) {
            if(i == 0)
                return 0;
            else
                return (i-1) >> lg_branches;
        }

        void swapVertices(int i1, int i2) {
            swap(heap_array[i1], heap_array[i2]);
            vertex_location[heap_array[i2].second] = i1;
            vertex_location[heap_array[i1].second] = i2;
        }
        
        void insertKey(int key, int vertex) {
            heap_array.push_back(pair<int, int>(key, vertex));
            heap_size++;            
            int i = heap_size - 1;
            vertex_location[vertex] = i;
            while(i != 0 && heap_array[this->parent(i)].first > heap_array[i].first) {
                int parent_index = this->parent(i);
                this->swapVertices(i, parent_index);
                i = parent_index;
            }
        }

        void minHeapify(int i) {
            int firstChild = this->firstChild(i);
            int lastChild = this->lastChild(i);
            int smallest = i;

            for(int j = firstChild; j <= lastChild; j++)
                if(j < heap_size && heap_array[j].first < heap_array[smallest].first)
                    smallest = j;
            if(smallest != i) {
                this->swapVertices(smallest, i);
                minHeapify(smallest);
            }
        }

        pair<int, int> removeMin() {
            if(heap_size <= 0)
                return pair<int, int>(-1, -1);
            if(heap_size == 1) {
                heap_size--;
                return heap_array[0];
            }
            pair<int, int> min = heap_array[0];
            heap_array[0] = heap_array[heap_size-1];
            heap_size--;
            this->minHeapify(0);
            return min;
        }

        void decreaseKey(int vertex, int weight) {
            int heap_index = vertex_location[vertex];
            heap_array[heap_index].first = weight;
            while(heap_index != 0 && heap_array[this->parent(heap_index)].first > heap_array[heap_index].first) {
                int parent_index = this->parent(heap_index);
                this->swapVertices(heap_index, parent_index);
                heap_index = parent_index;
            }
        }

        bool inHeap(int vertex) {
            int loc = vertex_location[vertex];
            if(loc >= heap_size)
                return false;
            else
                return true;
        }

        int getKey(int vertex) {
            int loc = vertex_location[vertex];
            return heap_array[loc].first;
        }

    private:
        vector<pair<int, int> > heap_array;
        int capacity;
        int branch_factor;
        int lg_branches;
        int heap_size;
        vector<int> vertex_location;
};

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

int prim(vector<int> vertices, int bf) {
    MinHeap heap = MinHeap(vertices.size(), bf);
    int total_weight = 0;
    int first = true;
    for(int v : vertices) {
        if(first) {
            heap.insertKey(0, v);
            first = false;
            continue;
        }
        heap.insertKey(INT_MAX, v);
    }
    while(heap.getSize() > 0) {
        pair<int, int> node = heap.removeMin();
        total_weight += node.first;
        int vertex = node.second;
        for(int v : adj_list[vertex]){
            int weight = weight_map[vertex][v];
            if(heap.inHeap(v) && weight < heap.getKey(v))
                heap.decreaseKey(v, weight);
        }
    }
    return total_weight;
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
    while(bf < ratio)
        bf = bf << 1;
    bf = (bf < 2) ? 2 : bf;
    int total_weight = 0;
    for(vector<int> component : components) {
        int weight = prim(component, bf);
        total_weight += weight;
    }
    cout << bf << " " << components.size() << " " << total_weight << "\n";
}