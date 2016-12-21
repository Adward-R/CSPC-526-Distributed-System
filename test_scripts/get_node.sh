echo get_node $2 from $1
curl -D - -H "Content-Type: application/json" -X POST -d {"node_id":$2} http://$1:8000/api/v1/get_node
