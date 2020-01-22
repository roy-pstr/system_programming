#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "csv_handler.h"

/*This reads Leaderboard.csv and print it in the needed format*/

/*Implement Leaderboard.csv into linked list*/
int RefreshLeaderboard(char *filename, LB_Node **head)
{
	if (NULL != *head)
	{
		DestroyLinkedList(*(head));
		*head = NULL;
	}
	FILE* fp_leaderboard = NULL;
	char line[LINE_MAX_LEN], client_name[USERNAME_MAX_LEN] = "", W[INT_MAX_LEN] = "", L[INT_MAX_LEN] = "", W_L[RATIO_MAX_LEN] = ""; 
	int wins, loses;
	double ratio;
	int field_count, ret_val = SUCCESS, line_num = 0;
	if (NULL == (fp_leaderboard = fopen(filename, "r")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		goto EXIT;
	}
	//fp_leaderboard = fopen(filename, "r");
	while (fgets(line, LINE_MAX_LEN, fp_leaderboard))
	{
		field_count = 0;
		char *field = strtok(line, ",");
		while (field) {
			if (field_count == 0) {
				strcpy(client_name, field);
			}
			if (field_count == 1) {
				strcpy(W, field);
				wins = (int)strtol(field, NULL, 10);
			}
			if (field_count == 2) {
				strcpy(L, field);
				loses = (int)strtol(field, NULL, 10);
			}
			if (field_count == 3) {
				strtok(field, "\n");
				strcpy(W_L, field);
				ratio = atof(field);
			}
			field = strtok(NULL, ",");
			field_count++;
		}
		if (line_num != 0)
		{
			LB_Node *line_node = CreateNode(client_name, wins, loses);
			sortedInsert(head, line_node);
		}
		line_num ++;
		strcpy(client_name, ""); strcpy(W, ""); strcpy(L, ""); strcpy(W_L, "");
	}
	
EXIT:
	if (fp_leaderboard != NULL)
	{
		fclose(fp_leaderboard);
	}
	return ret_val;
}

/*Function to create a node*/
LB_Node *CreateNode(char *name, int win, int lose) {
	LB_Node *new_element = NULL;
	if (NULL == (new_element = (LB_Node*)malloc(sizeof(LB_Node))))
	{
		printf("Node Memory Allocation Failed");
		return new_element;
	}
	strcpy(new_element->name, name);
	new_element->won = win;
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
void DestroyLinkedList(LB_Node *head)
{
	LB_Node *current = head;
	LB_Node *next = NULL;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}

/*Function that inserts a new node into a linked list (sorted by w/l ratio)*/
void sortedInsert(LB_Node **head, LB_Node* new_node)
{
	LB_Node* current;

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
void printList(LB_Node *head)
{
	LB_Node *temp = head;
	while (temp != NULL)
	{
		printf("Name = %s\nWins = %d\nLoses= %d\nW/L = %.3f\n", temp->name,temp->won,temp->lost,temp->ratio); 
		temp = temp->next;
	}
}

/*Function that update linked list after any move*/
LB_Node *DetectAndUpdateElement(LB_Node **head, char* name,int w)
{

	LB_Node *curr, *prev;

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
				curr->ratio = Round((curr->won +0.0)/ curr->lost,3);
			}
			return curr;
		}
	}
	/* if wasn't detected*/
	curr = CreateNode(name, w, !w);
	return curr;
}//DEBUG DRAW?

/*Updating Leaderboard.csv file from linked list*/
int LinkedListToCsv(LB_Node *head, char *filename)
{
	int ret_val = SUCCESS;
	LB_Node *temp = head;
	FILE *fp_leaderboard = NULL;
	if (NULL == (fp_leaderboard = fopen(filename, "w")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		goto EXIT;
	}
	fprintf(fp_leaderboard, "Name,W,L,W/L Ratio\n");
	while (temp != NULL)
	{
		if (temp->ratio == -1)
		{
			fprintf(fp_leaderboard, "%s,%d,%d,\n", temp->name, temp->won, temp->lost);
		}
		else
		{
			fprintf(fp_leaderboard, "%s,%d,%d,%.3f\n", temp->name, temp->won, temp->lost, temp->ratio);
		}
		temp = temp->next;
	}
EXIT:
	if (NULL != fp_leaderboard)
	{
		fclose(fp_leaderboard);
	}
	return ret_val;
}

///*Simple function to get length of linked list*/
//int LengthOfLinkedList(LB_Node *head)
//{
//	int num_of_elements = 0;
//	LB_Node *temp = head;
//	while (temp != NULL)
//	{
//		num_of_elements++;
//		temp = temp->next;
//	}
//	return num_of_elements;
//}
//
///*Function for printing the user the Leaderboard*/
//void LinkedListToStr(LB_Node *head, char **leaderboard_str,int buff_size)
//{
//	int length = 0;
//	LB_Node *temp = head;
//	//int buff_size = num_of_elements*LINE_MAX_LEN + 100;
//	length += snprintf(*leaderboard_str + length, buff_size - length, "Name\t\tWon\t\tLost\t\tW/L Ratio\n");
//	while (temp != NULL)
//	{
//		if (temp->ratio == -1)
//		{
//			length += snprintf(*leaderboard_str + length, buff_size - length, "%s\t\t%d\t\t%d\t\t\n", temp->name, temp->won, temp->lost, temp->ratio);
//		}
//		else
//		{
//			length += snprintf(*leaderboard_str + length, buff_size - length, "%s\t\t%d\t\t%d\t\t%.3f\n", temp->name, temp->won, temp->lost, temp->ratio);
//		}
//		temp = temp->next;
//	}
//}






