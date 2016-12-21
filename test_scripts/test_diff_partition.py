import os
import commands
import re
import sys
import random
import subprocess
import time

def whichPartition(i):
	return i % 3
vm = ["10.142.0.2", "10.142.0.3", "10.142.0.4"]

n = 10
for i in range (n):
	p = whichPartition(i)
	ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":%d}' http://%s:8000/api/v1/add_node"""%(i, vm[p]))
	print ret
for i in range(n):
	for j in range(n):
		pi = whichPartition(i)
		pj = whichPartition(j)
		if pi < pj:
			p = pi
		else:
			p = pj
		print "add_edge %d %d to partition %d"%(i, j, p)
		ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":%d,"node_b_id":%d}' http://%s:8000/api/v1/add_edge"""%(i,j,vm[p]))
		print ret
		print "=========="

a = [[1,2],[2,4], [4,8]]
for line in a:
	i,j = line
	pi = whichPartition(i)
	pj = whichPartition(j)
	if pi < pj:
		p = pi
	else:
		p = pj
	ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":%d,"node_b_id":%d}' http://%s:8000/api/v1/remove_edge"""%(i,j,vm[p]))
	print ret
	print "=========="
