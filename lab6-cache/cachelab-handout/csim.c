#include <stdio.h>
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

struct globalArgs_t {
	int s;
	int E;
	int b;
	int v;
	int h;
	char *t;
	
} globalArgs;

typedef struct {
	int valid;
	int tag_bit;
	unsigned int used;
} cache_line_t;

typedef struct {
	int occupied;
	cache_line_t * line;
} group_t;
group_t *group;

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

void init_cache() {
	int s = globalArgs.s;
	int S = 1 << s;
	int E = globalArgs.E;
	group = (group_t *) malloc(sizeof(group_t) * S);
	if (group == NULL) {
		printf("cannot allocate memory\n");
		exit(1);
	}
	for (int i = 0; i < S; i++) {
		group[i].occupied = 1;
		group[i].line = (cache_line_t *) malloc(sizeof(cache_line_t) * E);
		if (group[i].line == NULL) {
			printf("cannot allocate memory\n");
			exit(1);
		}
		cache_line_t * tmp_line = group[i].line;
		for (int j = 0; j < E; j++) {
			tmp_line[j].valid = 0;
			tmp_line[j].tag_bit = 0;
			tmp_line[j].used = 0;
		}
	}
}

void free_cache() {
	int s = globalArgs.s;
	int S = 1 << s;
	for (int i = 0; i < S; i++) {
		free(group[i].line);
	}
	free(group);
}

int hit = 0;
int miss = 0;
int evict = 0;

void parse_line(char * strbuf) {
	printf("%c\n", strbuf[1]);
	if (strbuf[0] == 'I') {
		return;
	}
	char *pEnd;
	long int addr, size = 0;
	addr = strtol(strbuf + 3, &pEnd, 10);
	size = strtol(pEnd + 1, NULL, 10);
	printf("addr: 0x%d, size: %d\n", (unsigned int)addr, (int)size);
	switch(strbuf[1]) {
		case 'L':
			break;
		case 'S':
			break;
		case 'M':
			break;
		default:
			break;
	}
}

int main(int argc, char **argv)
{
	parseCommand(argc, argv, "s:E:b:t:vh?");
	init_cache();
	
	char strbuf[BUFFER_SIZE];
	FILE *fp;
	
	if ((fp = fopen(globalArgs.t, "r")) == NULL) {
		printf("cannot open file: %s\n", globalArgs.t);
		return -1;
	}
	
	while(!feof(fp)) {
		fgets(strbuf, BUFFER_SIZE, fp);
		parse_line(strbuf);
	}
	printf("occupied: %d\n", group[0].occupied);
	printf("s: %d, E: %d, b: %d, t: %s, h: %d, v: %d\n", globalArgs.s, globalArgs.E, globalArgs.b, globalArgs.t, globalArgs.h, globalArgs.v);
	
    printSummary(hit, miss, evict);
    free_cache();
    return 0;
}


