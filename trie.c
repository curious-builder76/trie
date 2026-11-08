#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>


typedef unsigned char uchar;


#define MAX_NODES 255

typedef struct __node{
	int seen_null;
	struct __node* nodes;
}trie_t;

typedef trie_t node_t;


int trie_init(trie_t* trie){
	size_t mem_required=sizeof(node_t)*MAX_NODES;
	node_t* nodes=malloc(mem_required);
	if(!nodes){
		return 1;
	}
	memset(nodes, 0, mem_required);
	trie->nodes=nodes;
	return 0;
}

trie_t* trie_new(){
	trie_t* trie=malloc(sizeof(trie_t));
	if(!trie){
		return NULL;
	}
	memset(trie, 0, sizeof(trie_t));
	return trie;
}

int trie_insert(trie_t* trie,char* string){
	uchar* data= (uchar*)string;
	while(1){
		if(trie->nodes==NULL && trie_init(trie)!=0){
			return 1;
		}
		uchar ch=*data++;
		if(ch==0){
			trie->seen_null=1;
			return 0;
		}
		trie=trie->nodes+ch;
	}
}

int trie_contains(trie_t* trie,char* string){
	uchar* data= (uchar*)string;
	while(1){
		uchar ch=*data++;

		if(ch==0 && trie->seen_null){
			return 1;
		}
		trie=trie->nodes+ch;
		if(trie->nodes==NULL){
			return 0;
		}
	}
}

void trie_destroy0(trie_t* trie){
	if(!trie || !trie->nodes)
		return;
	for(size_t idx=0;idx<MAX_NODES;idx++){
		trie_destroy0(trie->nodes+idx);
	}
	free(trie->nodes);
}

void trie_destroy(trie_t* trie){
	trie_destroy0(trie);
	free(trie);
}
