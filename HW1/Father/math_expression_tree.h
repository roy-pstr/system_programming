#pragma once
#define MATH_STRING_MAX_LEN 256

typedef struct expression_node {
	char math_string[MATH_STRING_MAX_LEN];
	struct expression_node *left;
	struct expression_node *right;
} expression_node_st;

expression_node_st* Tree_newNode(char math_string[]);

bool Tree_isLeaf(expression_node_st* node);