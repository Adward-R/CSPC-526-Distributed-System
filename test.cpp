#include <cstdio>
#include "logblock.hpp"

int main(){
	Logentry l[10];
	printf("%d\n", sizeof(l));

	Logblock lb;
	printf("%d\n", sizeof(lb));

	lb.gen = 0;
	lb.n = 10;
	lb.chksum = 1108;
	int i = 0;
	for (Logblock::iterator it = lb.begin(); !it.end(); it.next()){
		it.entry()->opcode = i++;
		it.entry()->a = (i++)*1000;
		it.entry()->b = (i++)*1000;
	}
	for (int i = 0; i < 5 * lb.n; i++){
		printf("%d\n", ((uint32_t*)lb.entry_area)[i]);
	}
	return 0;
}
