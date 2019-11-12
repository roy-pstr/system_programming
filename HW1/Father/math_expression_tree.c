#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "math_expression_tree.h"

expression_node_st* Tree_newNode(char math_string[]) {
	// Allocate memory for new node  
	expression_node_st* node = (expression_node_st*)malloc(sizeof(expression_node_st));

	// Assign data to this node 
	strcpy_s(node->math_string, sizeof(node->math_string) ,math_string);

	// Initialize left and right children as NULL 
	node->left = NULL;
	node->right = NULL;

	return node;
}

bool Tree_isLeaf(expression_node_st* node) {
	return (node->left == NULL && node->right == NULL);
}

int Tree_calculateResult(expression_node_st* node) {
	if (Tree_isLeaf(node) == true) {
		return 0;
	}
	if (node->left != NULL) {

	}
	if (node->right != NULL) {

	}
}