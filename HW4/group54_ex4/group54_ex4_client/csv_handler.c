#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "csv_handler.h"


/*This reads Leaderboard.csv and print it in the needed format*/
int RefreshLeaderboard(char *filename)  // DEBUG - Should be on server
{
	FILE *fp_leaderboard = NULL;
	char line[1024], client_name[20]="", W[5]="", L[5]="", W_L[15]="" ; //DEBUG CONSTANTS
	int field_count, ret_val = SUCCESS;
	if (NULL == (fp_leaderboard = fopen(filename, "r")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		goto EXIT;
	}
	fp_leaderboard = fopen(filename, "r");
	while (fgets(line, 1024, fp_leaderboard))
	{
		field_count = 0;
		char *field = strtok(line, ",");
		while (field) {
			if (field_count == 0) {
				strcpy(client_name, field);
			}
			if (field_count == 1) {
				strcpy(W, field);
			}
			if (field_count == 2) {
				strcpy(L, field);
			}
			if (field_count == 3) {
				strtok(field, "\n");
				strcpy(W_L, field);
			}
			field = strtok(NULL, ",");
			field_count++;
		}
		printf("%-8s%-8s%-8s%s\n", client_name, W, L, W_L);
		strcpy(client_name, ""); strcpy(W, ""); strcpy(L, ""); strcpy(W_L, "");
	}
	fclose(fp_leaderboard);
EXIT:
	return ret_val;
}
int UpdateLeaderBoardFile(char *filename, char *name, int win)
{
	FILE *fp_leaderboard = NULL;
	char line[1024], client_name[20] = "", W[5] = "", L[5] = "", W_L[15] = ""; //DEBUG CONSTANTS
	int field_count, ret_val = SUCCESS;
	//int win = 0, lost = 0;
	float ratio = -1;
	if (NULL == (fp_leaderboard = fopen(filename, "r")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		goto EXIT;
	}
	fp_leaderboard = fopen(filename, "r");
	while (fgets(line, 1024, fp_leaderboard))
	{
		field_count = 0;
		char *field = strtok(line, ",");
		while (field) {
			if (field_count == 0) {
				strcpy(client_name, field);
			}
			if (field_count == 1) {
				strcpy(W, field);
			}
			if (field_count == 2) {
				strcpy(L, field);
			}
			if (field_count == 3) {
				strtok(field, "\n");
				strcpy(W_L, field);
			}
			field = strtok(NULL, ",");
			field_count++;
		}
		printf("%-8s%-8s%-8s%s\n", client_name, W, L, W_L);
		strcpy(client_name, ""); strcpy(W, ""); strcpy(L, ""); strcpy(W_L, "");
	}
EXIT:
	return ret_val;
}

/*Move to linked list module*/
int LeaderBoardLinkedList()
{
	Node;
}
/*Function to create a node*/

Node *CreateNode(char *name, int win, int lose)
{
	Node *new_element = NULL;
	if (NULL == (new_element = (Node*)malloc(sizeof(Node))))
	{
		printf("Node Memory Allocation Failed");
		return;
	}
	strcpy(new_element->name, name);
	new_element->won = win;
	printf("%d\n", new_element->won);
	new_element->lost = lose;
	if (lose == 0)
	{
		new_element->ratio = -1;
	}
	else
	{
		new_element->ratio = Round(((double)win) / lose,3);
	}
	new_element->next = NULL;// DEBUG
	return new_element;
}

/*Free linked list function*/
void List_destroyList(Node *head)
{
	Node *current = head;
	Node *next = NULL;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

/*Node *DetectNode(Node** head, char *name, int w)
{
	Node *new_elem = NULL, *curr = *head;
	if (*head == NULL)
	{
		new_elem = CreateNode(name, w, !w);
		return new_elem;
	}

	while (curr->next != NULL)
	{
		if (strcpr(curr->name, name))
		{

		}
	}
	//if not detected:
	new_elem = CreateNode(name, w, !w);
	return new_elem;

}
*/

void sortedInsert(Node **head, Node* new_node)
{
	Node* current;

	/* Special case for the head end */
	if (NULL == *head || new_node->ratio == -1)
	{
		new_node->next = *head;
		*head = new_node;
	}
	else if (new_node->ratio >= (*head)->ratio)
	{
		new_node->next = *head;
		*head = new_node;
	}
	else
	{
		/* Locate the node before the point of insertion */
		current = *head;
		while (current->next != NULL && new_node->ratio < current->next->ratio)
		{
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}
/* Function to print linked list */
void printList(Node *head)
{
	Node *temp = head;
	while (temp != NULL)
	{
		printf("W/L = %.3f\n", temp->ratio);
		temp = temp->next;
	}
}

Node *DetectAndUpdateElement(Node **head, char* name,int w)
{
	Node *curr, *prev;

	/* For 1st node, indicate there is no previous. */
	prev = NULL;
	if (head == NULL)
	{
		curr = CreateNode(name, w, !w);
		return curr;
	}
	/*
	 * Visit each node, maintaining a pointer to
	 * the previous node we just visited.
	 */
	for (curr = *head; curr != NULL; prev = curr, curr = curr->next) {
		if (strcmp(curr->name, name) == 0) 
		{  /* Found it. */
			if (prev == NULL) {
				/* Fix beginning pointer. */
				*head = curr->next;
			}
			else {
				/*
				 * Fix previous node's next to
				 * skip over the removed node.
				 */
				prev->next = curr->next;
			}

			/* Updating the detected node*/
			curr->won += w;
			curr->lost += !w;
			if (curr->lost == 0)
			{
				curr->ratio = -1;
			}
			else 
			{
				curr->ratio = (curr->won / curr->lost);
			}
			return curr;
		}
	}
	curr = CreateNode(name, w, !w);
	return curr;
}








/*CHECK IF FILE EXIST 
if (access(fname, F_OK) != -1) {
	// file exists
}
else {
	// file doesn't exist
}
*/

double Round(double x, int p)
{
	if (x != 0.0) {
		return ((floor((fabs(x)*pow((double)(10.0), p)) + 0.5)) / pow((double)(10.0), p))*(x / fabs(x));
	}
	else {
		return 0.0;
	}
}