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
echo get_edge 1 1
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":1,"node_b_id":1}' http://127.0.0.1:8000/api/v1/get_edge
echo
echo =============
echo get_node 201
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":201}' http://127.0.0.1:8000/api/v1/get_node
echo
echo =============
echo get_node 401
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":401}' http://127.0.0.1:8000/api/v1/get_node
echo
echo =============
echo get_node 501
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":501}' http://127.0.0.1:8000/api/v1/get_node
echo
echo =============
echo get_node 505
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":505}' http://127.0.0.1:8000/api/v1/get_node
echo
echo =============
echo get_node 506
curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":506}' http://127.0.0.1:8000/api/v1/get_node
echo
echo =============
