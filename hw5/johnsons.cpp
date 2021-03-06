#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;

int num_vertices = 0, num_edges = 0, num_queries = 0;
unordered_map<int, vector<int> > adj_list;
unordered_map<int, unordered_map<int, int> > weight_map;
unordered_map<int, unordered_map<int, int> > adjusted_weight_map;
vector<pair<int, int> > queries;
vector<vector<int> > true_distance_matrix;
vector<vector<int> > adjusted_distance_matrix;
vector<int> single_source_distance;
vector<pair<int, int> > adjusted_edges;

class MinHeap {
public:
    MinHeap(int size, int branch_factor) {
        capacity = size;
        heap_array.reserve(size);// = vector<pair<int, int> >(size);
        this->branch_factor = branch_factor;
        heap_size = 0;
        vertex_location.reserve(num_vertices);// = vector<int>(size);
        lg_branches = 0;
        int tmp = branch_factor;
        while(tmp >>= 1 != 0)
            lg_branches++;
    }
    
    int getSize() {return heap_size;}
    int firstChild(int i){return (i << lg_branches) + 1;}
    int lastChild(int i){return firstChild(i) + (branch_factor - 1);}
    int parent(int i) {
        if(i == 0)
            return 0;
        else
            return (i-1) >> lg_branches;
    }
    
    void swapVertices(int i1, int i2) {
        swap(heap_array[i1], heap_array[i2]);
        vertex_location[heap_array[i2].second] = i2;
        vertex_location[heap_array[i1].second] = i1;
    }
    
    void insertKey(int key, int vertex) {
        heap_array[heap_size] = pair<int, int>(key, vertex);
        heap_size++;
        int i = heap_size - 1;
        vertex_location[vertex] = i;
        while(i != 0 && heap_array[parent(i)].first > heap_array[i].first) {
            int parent_index = parent(i);
            swapVertices(i, parent_index);
            i = parent_index;
        }
    }
    
    void minHeapify(int i) {
        int first_child = firstChild(i);
        int last_child = lastChild(i);
        int smallest = i;
        
        for(int j = first_child; j <= last_child; j++)
            if(j < heap_size && heap_array[j].first < heap_array[smallest].first)
                smallest = j;
        if(smallest != i) {
            swapVertices(smallest, i);
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
        swapVertices(0, heap_size-1);
        heap_size--;
        minHeapify(0);
        return min;
    }
    
    void decreaseKey(int vertex, int weight) {
        int heap_index = vertex_location[vertex];
        heap_array[heap_index].first = weight;
        while(heap_index != 0 && heap_array[parent(heap_index)].first > heap_array[heap_index].first) {
            int parent_index = parent(heap_index);
            swapVertices(heap_index, parent_index);
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
    bool firstLine = true, queryLine = false;
    int l, r, currentEdge = 0;
    while(scanf("%d", &tmp) != -1) {
        if(firstLine) {
            if(i == 0) {
                num_vertices = tmp;
                adj_list.reserve(tmp + 1);
                weight_map.reserve(tmp + 1);
                adjusted_weight_map.reserve(tmp + 1);
                true_distance_matrix = vector<vector<int> >(num_vertices + 1, vector<int>(num_vertices + 1, INT_MAX));
                adjusted_distance_matrix = vector<vector<int> >(num_vertices, vector<int>(num_vertices, INT_MAX));
                single_source_distance.resize(tmp + 1, INT_MAX);
                single_source_distance[num_vertices] = 0;
                i++;
            } else {
                num_edges = tmp;
                adjusted_edges.resize(num_edges + num_vertices);
                i = 0;
                firstLine = false;
            }
            continue;
        } else if(!firstLine && currentEdge < num_edges) {
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
                i = 0;
                adjusted_edges[currentEdge++] = pair<int, int>(l, r);
                if(currentEdge == num_edges)
                    queryLine = true;
            } else
                throw invalid_argument("i out of range");
        } else if(queryLine) {
            num_queries = tmp;
            queryLine = false;
        } else {
            if(i == 0) {
                l = tmp;
                i++;
            } else if(i == 1) {
                r = tmp;
                i = 0;
                queries.push_back(pair<int, int>(l, r));
            }
        }
    }
}

// distance based off a source vertex by which d was initialized
void relax(int source, int end, unordered_map<int, unordered_map<int, int> >& wm, vector<int>& d) {
    int weight = wm[source][end];
    if(d[source] != INT_MAX)
        if(d[end] > d[source] + weight)
            d[end] = d[source] + weight;
}

bool bellman_ford() {
    for(int i = 0; i < num_vertices; i++) {
        for(pair<int, int> edge : adjusted_edges)
            relax(edge.first, edge.second, weight_map, single_source_distance);
    }
    for(pair<int, int> edge : adjusted_edges) {
        int s_distance = single_source_distance[edge.first];
        int e_distance = single_source_distance[edge.second];
        if(e_distance > s_distance + weight_map[edge.first][edge.second])
            return false;
    }
    return true;
}

void dijkstra(int s) {
    MinHeap heap = MinHeap(num_vertices, 2);
    for(int v = 0; v < num_vertices; v++)
        if(v == s)
            heap.insertKey(0, v);
        else
            heap.insertKey(INT_MAX, v);
    vector<int>& distance_row = adjusted_distance_matrix[s];
    distance_row[s] = 0;
    while(heap.getSize() > 0) {
        pair<int, int> node = heap.removeMin();
        int vertex = node.second;
        for(int v : adj_list[vertex]) {
            int d_before = distance_row[v];
            relax(vertex, v, adjusted_weight_map, distance_row);
            if(distance_row[v] < d_before)
                heap.decreaseKey(v, distance_row[v]);
        }
    }
    cout << "";
}

bool johnson() {
    int new_vertex = num_vertices;
    vector<int> edge_list = vector<int>(num_vertices);
    int edge_count = num_edges;
    for(int i = 0; i < num_vertices; i++) {
        edge_list[i] = i;
        adjusted_edges[edge_count++] = pair<int, int>(new_vertex, i);
        weight_map[new_vertex][i] = 0;
    }
    adj_list[new_vertex] = edge_list;
    bool res = bellman_ford();
    if(!res)
        return false;
    for(pair<int, int> edge : adjusted_edges) {
        int source = edge.first;
        int end = edge.second;
        int h_source = single_source_distance[source];
        int h_end = single_source_distance[end];
        adjusted_weight_map[source][end] = weight_map[source][end] + h_source - h_end;
    }
    for(int i = 0; i < num_vertices; i++) {
        dijkstra(i);
        for(int i = 0; i < num_vertices; i++) {
            int h_source = single_source_distance[i];
            for(int j = 0; j < num_vertices; j++) {
                int h_end = single_source_distance[j];
                if(adjusted_distance_matrix[i][j] == INT_MAX)
                    true_distance_matrix[i][j] = adjusted_distance_matrix[i][j];
                else
                    true_distance_matrix[i][j] = adjusted_distance_matrix[i][j] + h_end - h_source;
            }
        }
    }
    return true;
}

int main() {
    read_input();
    bool res = johnson();
    if(!res)
        cout << "Negative edge weight cycle\n";
    else
        for(pair<int, int> q : queries) {
            int len = true_distance_matrix[q.first][q.second];
            if(len < INT_MAX)
                cout << q.first << " -> " << q.second << " = " << len << '\n';
            else
                cout << q.first << " -> " << q.second << " = x\n";
        }
    return 0;
}
