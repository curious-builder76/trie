#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>


typedef unsigned char uchar;

typedef struct __node{
	int init_flag;
	uint64_t stored_chars[4];
	struct __node* nodes;
}trie_t;


#define MAX_NODES 256

int trie_init(trie_t* trie){
	size_t mem_required=MAX_NODES*sizeof(trie_t);
	trie_t* nodes=malloc(mem_required);
	if(!nodes) return 1;
	memset(nodes, 0, mem_required);
	trie->nodes=nodes;
	trie->init_flag=1;
	return 0;
}

trie_t* trie_new(){
	trie_t* root=malloc(sizeof(trie_t));
	memset(root, 0, sizeof *root);
	if(trie_init(root)!=0){
		free(root);
		return NULL;
	}
	root->init_flag=1;
	return root;
}


int trie_insert(trie_t* trie, char* string){
	uchar ch=*(uchar*)(string++);

	uint64_t byte_index= ch >> 8;
	uint64_t bit_index= ch & 63;

	trie->stored_chars[byte_index] |= (1lu<<bit_index);
	if(ch==0){
		return 0;
	}
	trie_t* child=trie->nodes + ch;
	if(!child->init_flag)
		if(trie_init(child)!=0){
			return 1;
		}
	return trie_insert(trie, string);

}


int trie_contains(trie_t* trie,char* string){
	
	uchar ch= *(uchar*)(string++);
	
	uint64_t byte_index=ch>>8;
	uint64_t bit_index= ch & 63;
	
	int flag=trie->stored_chars[byte_index] & (1lu<<bit_index);
	if(!flag)
		return 0;

	if(ch==0){
		return 1;
	}

	trie_t* child=trie->nodes + ch;

	if(!child->init_flag)
		return 0;

	return trie_contains(trie,string);
}

void trie_destroy0(trie_t* trie){
	for(size_t idx=0;idx<MAX_NODES;idx++){
		trie_t* child=trie->nodes + idx;
		if(trie->init_flag)
			trie_destroy0(child);
	}
	free(trie->nodes);

}

void trie_destroy(trie_t* trie){
	trie_destroy0(trie);
	free(trie);
}
