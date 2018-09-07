#include <stdio.h>
#include <string.h>
#include "cachelab.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define IDX(S, i, E) ((S) * (E) + (i))


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

cache_line_t *group;

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
	group = (cache_line_t *) malloc(sizeof(cache_line_t) * S * E);
	if (group == NULL) {
		printf("cannot allocate memory\n");
		exit(1);
	}
	for (int i = 0, size = S * E; i < size; i++) {
		group[i].valid = 0;
		group[i].tag_bit = 0;
		group[i].used = 0;
	}
}

void free_cache() {

	free(group);
	group = NULL;
}

int hit = 0;
int miss = 0;
int evict = 0;
int counter = 1;
char * parse_load(int S, int tag_bit, int B, int size) {
	int E = globalArgs.E;
	for (int i = 0; i < E; i++) {
		if (group[IDX(S, i, E)].valid && group[IDX(S, i, E)].tag_bit == tag_bit) {
			hit++;
			group[IDX(S, i, E)].used = counter++;
			return "hit";
		}
	}
	miss++;
	for (int i = 0; i < E; i++) {
		
		if (group[IDX(S, i, E)].valid == 0) {
			group[IDX(S, i, E)].valid = 1;
			group[IDX(S, i, E)].tag_bit = tag_bit;
			group[IDX(S, i, E)].used = counter++;
			return "miss";
		}
	}
	evict++;
	int min_idx = -1;
	int min_val = counter;
	for (int i = 0; i < E; i++) {
		if (group[IDX(S, i, E)].used < min_val) {
			min_idx = i;
			min_val = group[IDX(S, i, E)].used;
		}
	}
	if (min_idx != -1) {
		group[IDX(S,min_idx, E)].tag_bit = tag_bit;
		group[IDX(S,min_idx, E)].used = counter++;
	}
	return "miss evict";
}

void parse_line(char * strbuf) {
	if (strbuf[0] == 'I') {
		return;
	}
	char *pEnd;
	long int addr, size = 0;
	addr = strtol(strbuf + 3, &pEnd, 16);
	size = strtol(pEnd + 1, NULL, 16);
	
	int S = (addr >> globalArgs.b) & ((1 << globalArgs.s) - 1);
	int tag_bit = (addr >> (globalArgs.b + globalArgs.s));
	int B = addr & ((1 << globalArgs.b) - 1);
	//printf("S: 0x%8x, tag_bit: %8x, B: %8d, size: %8d\t\t", S, tag_bit, B,(int)size);
	char * desc = "";
	desc = parse_load(S, tag_bit, B, size);

	if (strbuf[1] == 'M') {
		hit++;
		printf("%s %s hit\n", strbuf, desc);
	} else {
		printf("%s %s\n", strbuf, desc);
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
		if (strbuf[strlen(strbuf) - 1] == '\n') {
			strbuf[strlen(strbuf) - 1] = '\0';
		}
		if (!feof(fp)) {
			//printf("%s\n", strbuf);
			 parse_line(strbuf);
		}		

	}
	
	printf("s: %d, E: %d\n", globalArgs.s, globalArgs.E);
	
    printSummary(hit, miss, evict);
    free_cache();
    fclose(fp);
    return 0;
}


