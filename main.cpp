#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
// Algorytm Prima

const bool DEBUG = true;

#include <iostream>

struct MSTEdge {
    int start_vertex,
            end_vertex,
            vertex_weight;
};

class MSTQueue {
private:
    MSTEdge *heap;
    int current_heap_position;
public:
    explicit MSTQueue(int number_of_elements);

    ~MSTQueue();

    MSTEdge front();

    void push(MSTEdge edge);

    void pop();
};

struct TreeNode {
    TreeNode *next;
    int vertex, weight;
};

class MSTree {
private:
    TreeNode **neighbours_table;
    int num_of_cells_in_table;
    int tree_weight;
public:
    explicit MSTree(int number_of_vertexes);

    ~MSTree();

    void addEdge(MSTEdge edge);

    TreeNode *get_node_from_tree(int node_number);

    void printTree();
};


MSTQueue::MSTQueue(int number_of_elements) {
    heap = new MSTEdge[number_of_elements];
    current_heap_position = 0;
}

MSTQueue::~MSTQueue() {
    delete[] heap;
}

MSTEdge MSTQueue::front() {
    return heap[0];
}

void MSTQueue::push(MSTEdge edge) {
    unsigned int position_for_edge, j;

    position_for_edge = current_heap_position++;
    j = (position_for_edge - 1u) >> 1;
    if (DEBUG) {
        printf("i: %d, j: %d\n", position_for_edge, j);
    }

    while (position_for_edge && (heap[j].vertex_weight > edge.vertex_weight)) {
        heap[position_for_edge] = heap[j];
        position_for_edge = j;
        j = (position_for_edge - 1u) >> 1;
        if (DEBUG) {
            printf("i: %d, j: %d\n", position_for_edge, j);
        }
    }

    heap[position_for_edge] = edge;
}

void MSTQueue::pop() {
    int i, j;
    MSTEdge edge{};

    if (current_heap_position) {
        edge = heap[--current_heap_position];

        i = 0;
        j = 1;

        while (j < current_heap_position) {
            if ((j + 1 < current_heap_position) && (heap[j + 1].vertex_weight < heap[j].vertex_weight)) j++;
            if (edge.vertex_weight <= heap[j].vertex_weight) break;
            heap[i] = heap[j];
            i = j;
            j = (j << 1) + 1;
        }

        heap[i] = edge;
    }
}

MSTree::MSTree(int number_of_vertexes) {

    neighbours_table = new TreeNode *[number_of_vertexes];
    for (int current_vertex_number = 0; current_vertex_number < number_of_vertexes; current_vertex_number++) {
        neighbours_table[current_vertex_number] = nullptr;
    }
    num_of_cells_in_table = number_of_vertexes - 1;
    tree_weight = 0;
}

MSTree::~MSTree() {
    TreeNode *node_to_check, *next_node;

    for (int current_cell_number = 0; current_cell_number <= num_of_cells_in_table; current_cell_number++) {
        node_to_check = neighbours_table[current_cell_number];
        while (node_to_check) {
            next_node = node_to_check;
            node_to_check = node_to_check->next;
            delete next_node;
        }
    }

    delete[] neighbours_table;
}

void MSTree::addEdge(MSTEdge edge) {
    TreeNode *node;

    tree_weight += edge.vertex_weight;
    node = new TreeNode;
    node->vertex = edge.end_vertex;
    node->weight = edge.vertex_weight;
    node->next = neighbours_table[edge.start_vertex];
    neighbours_table[edge.start_vertex] = node;

    node = new TreeNode;
    node->vertex = edge.start_vertex;
    node->weight = edge.vertex_weight;
    node->next = neighbours_table[edge.end_vertex];
    neighbours_table[edge.end_vertex] = node;
}

TreeNode *MSTree::get_node_from_tree(int node_number) {
    return neighbours_table[node_number];
}

void MSTree::printTree() {
    TreeNode *node;

    std::cout << std::endl;
    for (int i = 0; i <= num_of_cells_in_table; i++) {
        std::cout << "Vertex " << i << " - ";
        for (node = neighbours_table[i]; node; node = node->next) {
            std::cout << node->vertex << ":" << node->weight << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl << "Minimal Spanning Tree Weight = " << tree_weight << std::endl << std::endl;
}

void load_edges_from_stdin(int number_of_edges, MSTree &graph) {
    MSTEdge edge{};
    for (int i = 0; i < number_of_edges; i++) {
        std::cin >> edge.start_vertex >> edge.end_vertex >> edge.vertex_weight;
        graph.addEdge(edge);
    }
}

void initialize_already_visited(int number_of_vertexes, bool *already_visited) {
    already_visited[0] = true;
    for (int i = 1; i < number_of_vertexes; i++) {
        already_visited[i] = false;
    }
}

void find_mst(int number_of_vertexes, int number_of_edges, MSTree &graph) {
    bool *already_visited = new bool[number_of_vertexes];

    initialize_already_visited(number_of_vertexes, already_visited);

    int current_vertex;
    current_vertex = 0;

    MSTQueue queue(number_of_edges);
    TreeNode *tree_node;
    MSTree final_mst(number_of_vertexes);
    MSTEdge edge{};
    for (int i = 1; i < number_of_vertexes; i++) {
        for (tree_node = graph.get_node_from_tree(current_vertex); tree_node; tree_node = tree_node->next) {
            if (!already_visited[tree_node->vertex]) {
                edge.start_vertex = current_vertex;
                edge.end_vertex = tree_node->vertex;
                edge.vertex_weight = tree_node->weight;
                queue.push(edge);
            }
        }
        do {
            edge = queue.front();
            queue.pop();
        } while (already_visited[edge.end_vertex]);

        final_mst.addEdge(edge);
        already_visited[edge.end_vertex] = true;
        current_vertex = edge.end_vertex;
    }
    if (DEBUG) {
        final_mst.printTree();
    }
    delete[] already_visited;
}

int main() {
    int number_of_vertexes, number_of_edges;

    std::cin >> number_of_vertexes >> number_of_edges;

    MSTree graph(number_of_vertexes);

    load_edges_from_stdin(number_of_edges, graph);

    find_mst(number_of_vertexes, number_of_edges, graph);

    return 0;
}