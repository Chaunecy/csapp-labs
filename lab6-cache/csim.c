#include <stdio.h>
#include <string.h>
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
	unsigned int lru;
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
		group[i].occupied = 0;
		group[i].lru = 1;
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

char * parse_load(int S, int tag_bit, int B, int size) {
	group_t *g = &group[S];
	int idx = -1;
	cache_line_t * line = g->line;
	for (int i = 0; i < globalArgs.E; i++) {
		if (line[i].tag_bit == tag_bit && line[i].valid == 1) {
			idx = i;
			break;
		}
	}
	// 找到了
	if (idx != -1) {
		hit++;
		line[idx].used = g->lru++;
		g->occupied = (g->occupied < globalArgs.E) ? (g->occupied + 1) : g->occupied;
		
		return "hit";
	} else { // 没找到
		miss++;
		// 还有 valid = 0 的位置
		// printf("occupied: %d\n", g.occupied);
		if (g->occupied < globalArgs.E) {
			for (int i = 0; i < globalArgs.E; i++) {
				if (line[i].valid == 0) {
					line[i].valid = 1;
					line[i].tag_bit = tag_bit;
					line[i].used = g->lru++;
					break;
				}
			}
			return "miss";
		} else {
			evict++;
			int min = g->lru;
			int min_idx = -1;
			for (int i = 0; i < globalArgs.E; i++) {
				if (line[i].used < min) {
					min_idx = i;
					min = line[i].used;
				}
			}
			if (min_idx != -1) {
				line[min_idx].valid = 1;
				line[min_idx].used = g->lru++;
				line[min_idx].tag_bit = tag_bit;
			} else {
				printf("you should never see this!\n");
				exit(-1);
			}
			return "miss eviction";
		}
	}
}

void parse_line(char * strbuf) {
	if (strbuf[0] == 'I') {
		return;
	}
	char *pEnd;
	long int addr, size = 0;
	addr = strtol(strbuf + 3, &pEnd, 16);
	size = strtol(pEnd + 1, NULL, 16);
	
	int S = (addr >> globalArgs.b) & ((1 << globalArgs.b) - 1);
	int tag_bit = (addr >> (globalArgs.b + globalArgs.s));
	int B = addr & ((1 << globalArgs.b) - 1);
	
	char * desc;
	switch(strbuf[1]) {
		case 'L':
			desc = parse_load(S, tag_bit, B, size);
			printf("%s %s\n", strbuf, desc);
			break;
		case 'S':
			desc = parse_load(S, tag_bit, B, size);
			printf("%s %s\n", strbuf, desc);
			break;
		case 'M':
			desc = parse_load(S, tag_bit, B, size);
			char * desc1 = parse_load(S, tag_bit, B, size);
			printf("%s %s %s\n", strbuf, desc, desc1);
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
		if (strbuf[strlen(strbuf) - 1] == '\n') {
			strbuf[strlen(strbuf) - 1] = '\0';
		}
		if (!feof(fp)) {
			//printf("%s\n", strbuf);
			parse_line(strbuf);
		}		

	}
	
	printf("s: %d, E: %d\n", globalArgs.s, globalArgs.E);
	
    printSummary(0, 0, 0);
    free_cache();
    return 0;
}


