import os
import commands
import re
import sys
import random
import subprocess
import time

class TestLarge:
	def __init__(self):
		self.g = {}

	def kill(self):
		ret = commands.getoutput("""sudo killall cs426_graph_server""")
	# try until the connection established
	def try_connect(self):
		ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '' http://127.0.0.1:8000/api/v1/check""")
		ret = ret.replace('\n', ' ')
		match = re.search("Connection refused", ret)
		while match != None:
			ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '' http://127.0.0.1:8000/api/v1/check""")
			ret = ret.replace('\n', ' ')
			match = re.search("Connection refused", ret)
	def resume(self):
		hdl = subprocess.Popen(['sudo', './cs426_graph_server', '8000', '/dev/sdb'], stdout=subprocess.PIPE)
		self.try_connect()
	def checkpoint(self):
		ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '' http://127.0.0.1:8000/api/v1/checkpoint""")
		ret = ret.replace('\n', ' ')
		match = re.search("HTTP.* ([0-9]*) ", ret)
		retcode = int(match.group(1))
		if retcode == 507:
			print "Checkpoint full, exit"
			sys.exit(0)

	def add_node(self,node):
		ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":%d}' http://127.0.0.1:8000/api/v1/add_node"""%node)
		ret = ret.replace('\n', ' ')
		match = re.search("HTTP.* ([0-9]*) ", ret)
		retcode = int(match.group(1))
		while retcode == 507:
			print "checkpoint: add_node %d"%node
			self.checkpoint()
			ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":%d}' http://127.0.0.1:8000/api/v1/add_node"""%node)
			ret = ret.replace('\n', ' ')
			match = re.search("HTTP.* ([0-9]*) ", ret)
			retcode = int(match.group(1))

		expected = 0
		if not self.g.has_key(node):
			expected = 200
			self.g[node] = {}
		else:
			expected = 204
		if (expected != retcode):
			print "add_node %d error: expect %d, get %d"%(node, expected, retcode)
			return False
#print "add_node %d: %d"%(node, retcode)
		return True

	def add_edge(self, a, b):
		ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":%d,"node_b_id":%d}' http://127.0.0.1:8000/api/v1/add_edge"""%(a,b))
		ret = ret.replace('\n', ' ')
		match = re.search("HTTP.* ([0-9]*) ", ret)
		retcode = int(match.group(1))
		while retcode == 507:
			print "checkpoint: add_edge %d %d"%(a,b)
			self.checkpoint()
			ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":%d,"node_b_id":%d}' http://127.0.0.1:8000/api/v1/add_edge"""%(a,b))
			ret = ret.replace('\n', ' ')
			match = re.search("HTTP.* ([0-9]*) ", ret)
			retcode = int(match.group(1))

		expected = 0
		if a == b or not self.g.has_key(a) or not self.g.has_key(b):
			expected = 400
		elif self.g[a].has_key(b):
			expected = 204
		else:
			self.g[a][b] = 1
			self.g[b][a] = 1
			expected = 200
		if (expected != retcode):
			print "add_edge %d %d error: expect %d, get %d"%(a, b, expected, retcode)
			return False
#print "add_edge %d %d: %d"%(a,b, retcode)
		return True
	def remove_node(self,node):
		ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":%d}' http://127.0.0.1:8000/api/v1/remove_node"""%node)
		ret = ret.replace('\n', ' ')
		match = re.search("HTTP.* ([0-9]*) ", ret)
		retcode = int(match.group(1))
		while retcode == 507:
			print "checkpoint: remove_node %d"%node
			self.checkpoint()
			ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_id":%d}' http://127.0.0.1:8000/api/v1/remove_node"""%node)
			ret = ret.replace('\n', ' ')
			match = re.search("HTTP.* ([0-9]*) ", ret)
			retcode = int(match.group(1))

		expected = 0
		if self.g.has_key(node):
			expected = 200
			for i in self.g[node].keys():
				self.g[i].pop(node)
			self.g.pop(node)
		else:
			expected = 400
		if (expected != retcode):
			print "remove_node %d error: expect %d, get %d"%(node, expected, retcode)
			return False
#print "remove_node %d: %d"%(node, retcode)
		return True
	def remove_edge(self, a, b):
		ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":%d,"node_b_id":%d}' http://127.0.0.1:8000/api/v1/remove_edge"""%(a,b))
		ret = ret.replace('\n', ' ')
		match = re.search("HTTP.* ([0-9]*) ", ret)
		retcode = int(match.group(1))
		while retcode == 507:
			print "checkpoint: remove_edge %d %d"%(a,b)
			self.checkpoint()
			ret = commands.getoutput("""curl -D - -H "Content-Type: application/json" -X POST -d '{"node_a_id":%d,"node_b_id":%d}' http://127.0.0.1:8000/api/v1/remove_edge"""%(a,b))
			ret = ret.replace('\n', ' ')
			match = re.search("HTTP.* ([0-9]*) ", ret)
			retcode = int(match.group(1))

		expected = 0
		if a == b or not self.g.has_key(a) or not self.g.has_key(b):
			expected = 400
		elif not self.g[a].has_key(b):
			expected = 400 
		else:
		 	self.g[a].pop(b)
			self.g[b].pop(a)
			expected = 200
		if (expected != retcode):
			print "remove_edge %d %d error: expect %d, get %d"%(a, b, expected, retcode)
			return False
#print "remove_edge %d %d: %d"%(a,b, retcode)
		return True

def rand_node():
	return random.randint(1,50)
test = TestLarge()
'''
for i in range(50):
	test.add_node(rand_node())

for i in range(1000):
	test.add_edge(rand_node(),rand_node())
test.kill()
test.resume()

for i in range(50):
	test.remove_node(rand_node())

for i in range(1000):
	test.remove_edge(rand_node(),rand_node())
'''

for i in range(50):
	test.add_node(rand_node())
for i in range(1000):
	test.add_edge(rand_node(),rand_node())

for i in range(20000):
	rd = random.randint(0,999)
	if rd < 100:
		test.add_node(rand_node())
	elif rd < 200:
		test.remove_node(rand_node())
	elif rd < 700:
		test.add_edge(rand_node(), rand_node())
	elif rd < 950:
		test.remove_edge(rand_node(), rand_node())
	elif rd < 999:
		test.kill()
		test.resume()
	else:
		test.checkpoint()
		print "checkpoint"
		pass
test.kill()
