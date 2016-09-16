echo add_node 1
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_node 1
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_node 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":2}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_edge 1 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":2}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo add_edge 1 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":2}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo add_edge 1 3
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":3}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo add_edge 1 1
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":1}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo get_node 1
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1}' http://127.0.0.1:8000/api/v1/get_node
echo
echo =============
echo get_node 3
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":3}' http://127.0.0.1:8000/api/v1/get_node
echo
echo =============
echo get_edge 1 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":2}' http://127.0.0.1:8000/api/v1/get_edge
echo
echo =============
echo add_node 4
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":4}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_node 5
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":5}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_edge 1 4
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":4}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo get_neighbors 1
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1}' http://127.0.0.1:8000/api/v1/get_neighbors
echo
echo =============
echo get_neighbors 3
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":3}' http://127.0.0.1:8000/api/v1/get_neighbors
echo
echo =============
echo shortest_path 1 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":2}' http://127.0.0.1:8000/api/v1/shortest_path
echo
echo =============
echo shortest_path 2 4
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":2,"node_b_id":4}' http://127.0.0.1:8000/api/v1/shortest_path
echo
echo =============
echo shortest_path 1 5
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":5}' http://127.0.0.1:8000/api/v1/shortest_path
echo
echo =============
echo shortest_path 1 3
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":3}' http://127.0.0.1:8000/api/v1/shortest_path
echo
echo =============
echo remove_edge 1 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":2}' http://127.0.0.1:8000/api/v1/remove_edge
echo
echo =============
echo add_edge 1 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":2}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo remove_node 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":2}' http://127.0.0.1:8000/api/v1/remove_node
echo
echo =============
echo remove_node 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":2}' http://127.0.0.1:8000/api/v1/remove_node
echo
echo =============
echo add_node 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":2}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_edge 1 2
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":2}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo add_node 1234567890123
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1234567890123}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_node 9876543210987
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":9876543210987}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_node 1000000000001
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1000000000001}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_node 1000000000002
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1000000000002}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_node 1000000000003
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1000000000003}' http://127.0.0.1:8000/api/v1/add_node
echo
echo =============
echo add_edge 1234567890123 9876543210987
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1234567890123,"node_b_id":9876543210987}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo add_edge 1234567890123 9876543210987
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1234567890123,"node_b_id":9876543210987}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo add_edge 1234567890123 1000000000001
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1234567890123,"node_b_id":1000000000001}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo add_edge 1000000000002 1000000000001
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1000000000002,"node_b_id":1000000000001}' http://127.0.0.1:8000/api/v1/add_edge
echo
echo =============
echo shortest_path 1234567890123 9876543210987
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1234567890123,"node_b_id":9876543210987}' http://127.0.0.1:8000/api/v1/shortest_path
echo
echo =============
echo shortest_path 1000000000002 9876543210987
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1000000000002,"node_b_id":9876543210987}' http://127.0.0.1:8000/api/v1/shortest_path
echo
echo =============
echo shortest_path 1000000000003 9876543210987
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1000000000003,"node_b_id":9876543210987}' http://127.0.0.1:8000/api/v1/shortest_path
echo
echo =============
echo shortest_path 1000000000004 9876543210987
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1000000000004,"node_b_id":9876543210987}' http://127.0.0.1:8000/api/v1/shortest_path
echo
echo =============
echo get_neighbors 1234567890123
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1234567890123}' http://127.0.0.1:8000/api/v1/get_neighbors
echo
echo =============
echo get_neighbors 1000000000004
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1000000000004}' http://127.0.0.1:8000/api/v1/get_neighbors
echo
echo =============
echo get_neighbors 1000000000003
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":1000000000002}' http://127.0.0.1:8000/api/v1/get_neighbors
echo
