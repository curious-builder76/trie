#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#include "trie.h"

#define printf(...) 

typedef unsigned char uchar;


typedef struct __trie{
	uint8_t min_seen;
	uint8_t max_seen;
	uint16_t capacity;
	uint64_t stored_chars[4];
	struct __trie* nodes;
}trie_t;

int trie_init(trie_t*);

trie_t* trie_new(){
	trie_t* root=malloc(sizeof(trie_t));
	if(!root)
		return NULL;
	memset(root,0,sizeof *root);
	if(trie_init(root)!=0){
		free(root);
		root=NULL;
	}
	return root;
}

int trie_init(trie_t* trie){
	if(!trie) return 1;
	uchar* node= (uchar*)trie;
	printf("Hexdump:\n");
	for(size_t idx=0;idx<sizeof(trie_t);idx++){
		printf("%02x ",*node++);
	}
	printf("\n");
	trie->capacity=64;
	size_t mem_required=sizeof(trie_t)*trie->capacity;
	trie_t* nodes=malloc(mem_required);
	trie->min_seen=255;
	if(!nodes)
		return 1;
	memset(nodes,0, mem_required);
	trie->nodes=nodes;
	return 0;
}

static int rearrange_nodes(trie_t* trie, uchar ch){
	printf("%c\n",ch);
	if(ch < trie->max_seen || ch >= trie->min_seen ){
		return 0; /* Already sorted */
	}
	if(trie->min_seen > trie->max_seen){
		trie->min_seen=ch;
		trie->max_seen=ch;
		printf("First insertion.\n");
		return 0;
	}
	size_t used=trie->max_seen-trie->min_seen;
	/* Fail to grow intentionaly*/
	if(used>=trie->capacity)
		return 1;
	if(ch > trie->max_seen){
		trie->max_seen=ch;
		return 0;
	}
	if( ch < trie->min_seen){
		size_t distance= trie->min_seen-ch;
		memmove(trie->nodes+distance, trie->nodes, used*sizeof(trie_t));
		trie->min_seen=ch;
		return 0;
	}
	printf("Here");
	return 1;
}

static int trie_insert0(trie_t* trie,uchar* string){
	uchar ch=*string++;
	if(rearrange_nodes(trie,ch)){
		return 1;
	}
	uint64_t byte_index= ch>> 6;
	uint64_t bit_index = ch & 63;

	int initalized= trie->stored_chars[byte_index] & (1llu<< bit_index);
	trie_t* child=trie->nodes + (ch-trie->min_seen);
	printf("Index: %d Cap: %d\n",ch-trie->min_seen, trie->capacity);
	if(ch-trie->min_seen < 0 ){
		printf("Critical: The index was found to be -ve.\n");
		return 127;
	}
	if(!initalized && trie_init(child)!=0){
		return 1;
	}
	trie->stored_chars[byte_index] |= (1llu<<bit_index);

	if(ch==0){
		return 0;
	}
	return trie_insert0(child,string);
}

int trie_insert(trie_t* trie, char* string){
	return trie_insert0(trie, (uchar*)string);
}

int trie_contains0(trie_t* trie, uchar* string){
	uchar ch= *string++;

	uint64_t byte_index= ch >> 6;
	uint64_t bit_index = ch & 63;

	int flag= trie->stored_chars[byte_index] & (1llu<<bit_index);
	if(!flag)
		return 0;
	if(ch==0)
		return 1;
	trie_t* child=trie->nodes + (ch-trie->min_seen);
	/* sanity check */
	if(!child || !child->nodes){
		return 0;
	}
	return trie_contains0(child, string);

}
int trie_contains(trie_t* trie, char* string){
	return trie_contains0(trie,(uchar*)string);
}

void trie_destroy(trie_t* trie){
	printf("MEMORY WAS LEAKED ON PURPOSE.\n"
			"DESTRUCTION IS STILL UNDER CONSTRUCTION\n");
	return;
	/*
	size_t used=trie->max_seen-trie->min_seen;
	for(size_t idx=0;idx<used;idx++){
		trie_t* child=trie->nodes +idx;
		trie_destroy(child);
	}
	if(trie->nodes){
		free(trie->nodes);
		trie->nodes=NULL;
	}
	*/
}

