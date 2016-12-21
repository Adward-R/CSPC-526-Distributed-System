echo get_edge $2 $3 from $1
curl -D - -H "Content-Type: application/json" -X POST -d {"node_a_id":$2",node_b_id":$3} http://$1:8000/api/v1/get_edge
