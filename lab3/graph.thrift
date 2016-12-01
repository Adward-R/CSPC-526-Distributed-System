namespace cpp graph

enum Operation {
    ADD_NODE = 1,
    REMOVE_NODE = 2,
    ADD_EDGE = 3,
    REMOVE_EDGE = 4
}

struct Graph_ops {
    1: Operation op,
    2: i64 param1,
    3: i64 param2 = -1
}

service GraphService {
    i64 mutate(Graph_ops& graph_ops)
}
