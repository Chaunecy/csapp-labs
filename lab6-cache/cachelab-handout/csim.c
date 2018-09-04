#include <stdio.h>
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

struct globalArgs_t {
	int s;
	int E;
	int b;
	int v;
	int h;
	char *t;
	
} globalArgs;


/**
 * 处理命令行参数。
 * 没有处理参数不足的情况
 */
void parseCommand(int argc, char **argv, char *optstring) {
	int opt = getopt(argc, argv, optstring);
	while(opt != -1) {
		switch(opt) {
			case 'h':
				globalArgs.h = 1;
				break;
			case 'v':
				globalArgs.v = 1;
				break;
			case 's':
				globalArgs.s = *optarg - '0';
				break;
			case 'E':
				globalArgs.E = *optarg - '0';
				break;
			case 'b':
				globalArgs.b = *optarg - '0';
				break;
			case 't':
				globalArgs.t = optarg;
				break;
			default:
				printf("unknown option, some error occurred");
				exit(1);
		}
		
		opt = getopt(argc, argv, optstring);
	}
}

int main(int argc, char **argv)
{
	parseCommand(argc, argv, "s:E:b:t:vh?");
	
	printf("s: %d, E: %d, b: %d, t: %s, h: %d, v: %d\n", globalArgs.s, globalArgs.E, globalArgs.b, globalArgs.t, globalArgs.h, globalArgs.v);
	
    printSummary(9, 8, 6);
    return 0;
}


