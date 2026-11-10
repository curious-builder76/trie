#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

#include "trie.h"

#define MAX_NODES 255

#pragma pack(push, 1)
typedef struct __trie{
	uint8_t seen_null;
	uint8_t capacity;
	struct __trie* nodes;
}trie_t;

#pragma pack(pop)
typedef unsigned char uchar;

typedef trie_t node_t;

trie_t* trie_new(){
	trie_t* root;
	if((root=malloc(sizeof(trie_t)))==NULL)
		return root;
	memset(root,0,sizeof *root);
	return root;
}

static inline int trie_grow(trie_t* trie, uchar ch){
	if(trie->capacity >=MAX_NODES){
		/* At maximum capacity*/
		return 0;
	}
	uint16_t cap= trie->capacity ? trie->capacity : 8;

	while(cap < ch){
		cap*=2;
	}
	if(cap>=MAX_NODES)
		cap=MAX_NODES;
	size_t mem_required=sizeof(node_t)*cap;

	node_t* nodes;
	if(trie->nodes==NULL){
		nodes=malloc(mem_required);
	}else{
		nodes=realloc(trie->nodes,mem_required);
	}
	if(!nodes){
		return 1;
	}

	memset(nodes+ trie->capacity, 0, (cap-trie->capacity)*sizeof(node_t));
	trie->nodes=nodes;
	trie->capacity=cap;
	return 0;
}

int trie_insert(trie_t* trie, char* input){
	uchar* data= (uchar*)input;

	while(1){
		uchar ch=*data++;
		if(ch==0){
			trie->seen_null=1;
			return 0;
		}
		
		if(trie_grow(trie,ch)!=0){
			return 1;
		}
		trie= trie->nodes+ (ch-1);
	}
}

int trie_contains(trie_t* trie,char* input){
	uchar* data=(uchar*)input;

	while(1){
		uchar ch=*data++;
		if(ch==0){
			return trie->seen_null;
		}
		if( ch >= trie->capacity){
			return 0;
		}
		trie= trie->nodes + (ch-1);
	}
}

void trie_destroy0(trie_t* trie){
	if(!trie || !trie->nodes){
		return;
	}

	for(uint16_t idx=0;idx<trie->capacity;idx++){
		trie_t* child=trie->nodes+idx;
		trie_destroy0(child);
	}
	free(trie->nodes);
}
void trie_destroy(trie_t* trie){
	trie_destroy0(trie);
	free(trie);
}

