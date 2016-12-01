#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <stdint.h>
#include "graph.hpp"
#include "mongoose.h"

using namespace std;

#define ENDPOINT "/api/v1/"
#define ENDPOINT_LEN (sizeof(ENDPOINT)-1)

Graph g;

uint64_t tok2int(json_token *tok){
	uint64_t res = 0;
	for (int i = 0; i < tok->len; i++)
		res = res*10 + tok->ptr[i] - '0';
	return res;
}

int check_uri_valid(mg_str *s){
	if (s->len < ENDPOINT_LEN)
		return 0;
	int i;
	for (i = 0; i < s->len && i < ENDPOINT_LEN; i++){
		if (s->p[i] != ENDPOINT[i])
			return 0;
	}
	return 1;
}

#define DEBUG 0
// Define an event handler function
static void ev_handler(mg_connection *nc, int ev, void *ev_data) {
	mbuf *io = &nc->recv_mbuf;
	http_message *hm;
	int n_tok = 10;
	json_token tok[10], *t;

	if (ev == MG_EV_HTTP_REQUEST){
		hm = (http_message *) ev_data;
		if (!check_uri_valid(&hm->uri)){
#if DEBUG
			printf("uri error: %.*s\n", hm->uri.len, hm->uri.p);
#endif
			return;
		}
		const char *func = hm->uri.p + ENDPOINT_LEN;
		int func_len = hm->uri.len - ENDPOINT_LEN;
		if (strncmp(func, "add_node", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_id");
			uint64_t node_id = tok2int(t);
#if DEBUG
			printf("add_node %llu\n", node_id);
#endif

			ret = g.add_node(node_id);
			if (ret == 1){
				char buf[100];
				int len = sprintf(buf, "{\"node_id\":%llu}", node_id);
				mg_send_head(nc, 200, len, "Content-Type: text/json");
				mg_printf(nc, "%.*s", len, buf);
#if DEBUG
				printf("200, %s\n", buf);
#endif
			}else if (ret == 0){
				mg_send_head(nc, 204, 0, "Content-Type: text/json");
#if DEBUG
				printf("204\n");
#endif
			}
		}else if (strncmp(func, "add_edge", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_a_id");
			uint64_t a = tok2int(t);
			t = find_json_token(tok, "node_b_id");
			uint64_t b = tok2int(t);
#if DEBUG
			printf("add_edge %llu %llu\n", a, b);
#endif

			ret = g.add_edge(a,b);
			if (ret == 1){
				char buf[100];
				int len = sprintf(buf, "{\"node_a_id\":%llu,\"node_b_id\":%llu}", a, b);
				mg_send_head(nc, 200, len, "Content-Type: text/json");
				mg_printf(nc, "%.*s", len, buf);
#if DEBUG
				printf("200, %s\n", buf);
#endif
			}else if (ret == 0){
				mg_send_head(nc, 204, 0, "Content-Type: text/json");
#if DEBUG
				printf("204\n");
#endif
			}else if (ret == -1){
				mg_send_head(nc, 400, 0, "Content-Type: text/json");
#if DEBUG
				printf("400\n");
#endif
			}
		}else if (strncmp(func, "remove_node", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_id");
			uint64_t node_id = tok2int(t);
#if DEBUG
			printf("remove_node %llu\n", node_id);
#endif

			ret = g.remove_node(node_id);
			if (ret == 1){
				char buf[100];
				int len = sprintf(buf, "{\"node_id\":%llu}", node_id);
				mg_send_head(nc, 200, len, "Content-Type: text/json");
				mg_printf(nc, "%.*s", len, buf);
#if DEBUG
				printf("200, %s\n", buf);
#endif
			}else if (ret == -1){
				mg_send_head(nc, 400, 0, "Content-Type: text/json");
#if DEBUG
				printf("400\n");
#endif
			}
		}else if (strncmp(func, "remove_edge", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_a_id");
			uint64_t a = tok2int(t);
			t = find_json_token(tok, "node_b_id");
			uint64_t b = tok2int(t);
#if DEBUG
			printf("remove_edge %llu %llu\n", a, b);
#endif

			ret = g.remove_edge(a,b);
			if (ret == 1){
				char buf[100];
				int len = sprintf(buf, "{\"node_a_id\":%llu,\"node_b_id\":%llu}", a, b);
				mg_send_head(nc, 200, len, "Content-Type: text/json");
				mg_printf(nc, "%.*s", len, buf);
#if DEBUG
				printf("200, %s\n", buf);
#endif
			}else if (ret == -1){
				mg_send_head(nc, 400, 0, "Content-Type: text/json");
#if DEBUG
				printf("400\n");
#endif
			}
		}else if (strncmp(func, "get_node", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_id");
			uint64_t node_id = tok2int(t);
#if DEBUG
			printf("get_node %d\n", node_id);
#endif

			ret = g.get_node(node_id);
			if (ret == 1){
				mg_send_head(nc, 200, 17, "Content-Type: text/json");
				mg_printf(nc, "{\"in_graph\":true}");
#if DEBUG
				printf("200, {\"in_graph\":true}\n");
#endif
			}else if (ret == 0){
				mg_send_head(nc, 200, 18, "Content-Type: text/json");
				mg_printf(nc, "{\"in_graph\":false}");
#if DEBUG
				printf("200, {\"in_graph\":false}\n");
#endif
			}
		}else if (strncmp(func, "get_edge", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_a_id");
			uint64_t a = tok2int(t);
			t = find_json_token(tok, "node_b_id");
			uint64_t b = tok2int(t);
#if DEBUG
			printf("get_edge %llu %llu\n", a, b);
#endif

			ret = g.get_edge(a, b);
			if (ret == 1){
				mg_send_head(nc, 200, 17, "Content-Type: text/json");
				mg_printf(nc, "{\"in_graph\":true}");
#if DEBUG
				printf("200, {\"in_graph\":true}\n");
#endif
			}else if (ret == 0){
				mg_send_head(nc, 200, 18, "Content-Type: text/json");
				mg_printf(nc, "{\"in_graph\":false}");
#if DEBUG
				printf("200, {\"in_graph\":false}\n");
#endif
			}else if (ret == -1){
				mg_send_head(nc, 400, 0, "Content-Type: text/json");
#if DEBUG
				printf("400\n");
#endif
			}
		}else if (strncmp(func, "get_neighbors", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_id");
			uint64_t node_id = tok2int(t);
#if DEBUG
			printf("get_neighbors %d\n", node_id);
#endif

			vector<uint64_t> neighbors;
			ret = g.get_neighbors(node_id, neighbors);
			if (ret == 1){
				stringstream s;
				s<<"{\"node_id\":"<<node_id<<",\"neighbors\":[";
				if (neighbors.size()>0)
					s<<neighbors[0];
				for (int i = 1; i < neighbors.size(); i++)
					s<<','<<neighbors[i];
				s<<"]}";
				mg_send_head(nc, 200, s.str().size(), "Content-Type: text/json");
				mg_printf(nc,"%.*s", s.str().size(), s.str().c_str());
#if DEBUG
				printf("200, %s\n", s.str().c_str());
#endif
			}else if (ret == -1){
				mg_send_head(nc, 400, 0, "Content-Type: text/json");
#if DEBUG
				printf("400\n");
#endif
			}
		}else if (strncmp(func, "shortest_path",func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_a_id");
			uint64_t a = tok2int(t);
			t = find_json_token(tok, "node_b_id");
			uint64_t b = tok2int(t);
#if DEBUG
			printf("shortest_path %llu %llu\n", a, b);
#endif

			ret = g.shortest_path(a, b);
			if (ret >= 0){
				char buf[100];
				int len = sprintf(buf, "{\"distance\":%d}", ret);
				mg_send_head(nc, 200, len, "Content-Type: text/json");
				mg_printf(nc, "%.*s", len, buf);
#if DEBUG
				printf("200, %s\n", buf);
#endif
			}else if (ret == -1){
				mg_send_head(nc, 204, 0, "Content-Type: text/json");
#if DEBUG
				printf("204\n");
#endif
			}else if (ret == -2){
				mg_send_head(nc, 400, 0, "Content-Type: text/json");
#if DEBUG
				printf("400\n");
#endif
			}
		}
		/*
		json_token *t = find_json_token(tok, "node_id");
		printf("%.*s\n",t->len, t->ptr);
		switch (t->type){
			case JSON_TYPE_EOF:
				printf("EOF\n");
				break;
			case JSON_TYPE_STRING:
				printf("string\n");
				break;
			case JSON_TYPE_NUMBER:
				printf("number\n");
				break;
			case JSON_TYPE_OBJECT:
				printf("object\n");
				break;
			case JSON_TYPE_TRUE:
				printf("true\n");
				break;
			case JSON_TYPE_FALSE:
				printf("false\n");
				break;
			case JSON_TYPE_NULL:
				printf("NULL\n");
				break;
			case JSON_TYPE_ARRAY:
				printf("array\n");
				break;
			default:
				break;
		}
		*/
	}
}

int main(int argc, char** argv) {
	char port[10] = "8000";
	if (argc >= 2){
		strcpy(port, argv[1]);
	}
	mg_mgr mgr;
	mg_connection *conn;

	mg_mgr_init(&mgr, NULL);	// Initialize event manager object

	// Note that many connections can be added to a single event manager
	// Connections can be created at any point, e.g. in event handler function
	conn = mg_bind(&mgr, port, ev_handler);	// Create listening connection and add it to the event manager
	mg_set_protocol_http_websocket(conn);

	for (;;) {	// Start infinite event loop
		mg_mgr_poll(&mgr, 1000);
	}

	mg_mgr_free(&mgr);
	return 0;
}
