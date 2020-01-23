#ifndef _CSV_HANDLER_H
#define _CSV_HANDLER_H
#include "stdbool.h"
#include "utils.h"

//#define CSV_NAME "C:\\Users\\Ilay Amar\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"
//#define GS_NAME "C:\\Users\\Ilay Amar\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\GameSession.txt"
//#define GS_NAME "C:\\Users\\roypa\\OneDrive\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\GameSession.txt"
//#define CSV_NAME "C:\\Users\\roypa\\OneDrive\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"
#define CSV_NAME ".\\Leaderboard.csv"
#define GS_NAME ".\\GameSession.txt"

typedef struct lb_node_st
{
	char name[20];
	int won;
	int lost;
	double ratio;
	struct lb_node_st *next;
} LB_Node;

/*Relevant functions*/
LB_Node *DetectAndUpdateElement(LB_Node **head, char* name, int w);
void sortedInsert(LB_Node** head, LB_Node* new_node);
LB_Node *CreateNode(char *name, int win, int lose);
int RefreshLeaderboard(char *filename, LB_Node **Head);
void DestroyLinkedList(LB_Node *head);

void printList(LB_Node *head);
int LinkedListToCsv(LB_Node *head, char *filename);
void LinkedListToStr(LB_Node *head, char **leaderboard_str, int buff_size);
int LengthOfLinkedList(LB_Node *head);
bool IsFileExists(char* filename);



#endif