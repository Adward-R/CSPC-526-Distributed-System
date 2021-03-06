#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <stdint.h>
#include <pthread.h>

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "graph.hpp"
#include "mongoose.h"
#include "backup_client.hpp"
#include "backup_server.hpp"
#include "storage_server.hpp"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

#define ENDPOINT "/api/v1/"
#define ENDPOINT_LEN (sizeof(ENDPOINT)-1)

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

StorageServer storageServer;

#define DEBUG 0
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

			ret = storageServer.add_node(node_id);
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
			}else if (ret == -100){
				mg_send_head(nc, 500, 0, "Content-Type: text/json");
#if DEBUG
				printf("500\n");
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

			ret = storageServer.add_edge(a,b);
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
			}else if (ret == -100){
				mg_send_head(nc, 500, 0, "Content-Type: text/json");
#if DEBUG
				printf("500\n");
#endif
			}

		}else if (strncmp(func, "remove_node", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_id");
			uint64_t node_id = tok2int(t);
#if DEBUG
			printf("remove_node %llu\n", node_id);
#endif

			ret = storageServer.remove_node(node_id);
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
			}else if (ret == -100){
				mg_send_head(nc, 500, 0, "Content-Type: text/json");
#if DEBUG
				printf("500\n");
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

			ret = storageServer.remove_edge(a,b);
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
			}else if (ret == -100){
				mg_send_head(nc, 500, 0, "Content-Type: text/json");
#if DEBUG
				printf("500\n");
#endif
			}

		}else if (strncmp(func, "get_node", func_len)==0){
			int ret = parse_json(hm->body.p, hm->body.len,tok,n_tok);
			t = find_json_token(tok, "node_id");
			uint64_t node_id = tok2int(t);
#if DEBUG
			printf("get_node %d\n", node_id);
#endif

			ret = storageServer.get_node(node_id);
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

			ret = storageServer.get_edge(a, b);
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
			ret = storageServer.get_neighbors(node_id, neighbors);
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

			ret = storageServer.shortest_path(a, b);
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
	}
}

void* run_webserver(void* arg){
	char* port = (char*) arg;

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

int main(int argc, char** argv) {
	char backupIp[20] = "";
	char port[10] = "8000";

	for (char opt; (opt = getopt(argc, argv, "b:")) != -1;){
		switch (opt){
			case 'b':
				strcpy(backupIp, optarg);
				break;
			default: /* '?' */
				printf("Usage: %s [-b <backupIp>] <port>\n", argv[0]);
				return 0;
		}
	}
	if (strlen(backupIp) != 0)
		printf("backup IP: %s\n", backupIp);

	if (strlen(backupIp) == 0 && argc >= 2){
		strcpy(port, argv[1]);
	}else if (strlen(backupIp) != 0 && argc >= 4){
		strcpy(port, argv[3]);
	}
	printf("listening on port %s\n", port);

	if (strlen(backupIp) != 0){
		int ret = storageServer.connectBackupClient(backupIp);
		if (ret == 0){
			printf("[main] Fail to connect to the backup server %s\n", backupIp);
			return 0;
		}
	}

	// Start web server
	pthread_t webserver_thread;

	if(pthread_create(&webserver_thread, NULL, run_webserver, port)) {
		fprintf(stderr, "Error creating webserver thread\n");
		return 0;
	}

#if 1
	// start thrift backup server
	TThreadedServer server(
			boost::make_shared<BackupServiceProcessor>(boost::make_shared<BackupServer>(&storageServer)), // pass  the storageServer to it, so it can update the storageServer
			boost::make_shared<TServerSocket>(THRIFT_PORT), //port
			boost::make_shared<TBufferedTransportFactory>(),
			boost::make_shared<TBinaryProtocolFactory>());

	server.serve();
#endif

	return 0;
}
