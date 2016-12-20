enum Operation {
	ADD_NODE = 1,
	REMOVE_NODE = 2,
	ADD_EDGE = 3,
	REMOVE_EDGE = 4,
	GET_NODE = 5
}

service BackupService {
    i64 backup(1: Operation op, 2: i64 param1, 3: i64 param2 = -1)
}
