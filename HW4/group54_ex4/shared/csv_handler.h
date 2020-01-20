#ifndef _CSV_HANDLER_H
#define _CSV_HANDLER_H
#include "stdbool.h"
#include "utils.h"
#include "msg_protocol.h"

//#define CSV_NAME "C:\\Users\\Ilay Amar\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"
//#define GS_NAME "C:\\Users\\Ilay Amar\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\GameSession.txt"
#define GS_NAME "C:\\Users\\roypa\\OneDrive\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\GameSession.txt"
#define CSV_NAME "C:\\Users\\roypa\\OneDrive\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"
//#define CSV_NAME ".\\Leaderboard.csv"
//#define GS_NAME ".\\GameSession.txt"

typedef struct node_st
{
	char name[20];
	int won;
	int lost;
	double ratio;
	struct node_st *next;
} Node;


/*###################*/
/*Relevant functions*/
Node *DetectAndUpdateElement(Node **head, char* name, int w);
void sortedInsert(Node** head, Node* new_node);
Node *CreateNode(char *name, int win, int lose);
int RefreshLeaderboard(char *filename, Node **Head);
void DestroyLinkedList(Node *head);
double Round(double x, int p);
void printList(Node *head);
int LinkedListToCsv(Node *head, char *filename);
//void LinkedListToStr(Node *head, char **leaderboard_str, int buff_size);
int LengthOfLinkedList(Node *head);
bool IsFileExists(char* filename);
void LinkedListToParam(Node *head, param_node **head_msg);
void DestroyParamLinkedList(param_node *head);
param_node *CreateParamNode(char *line);


#endif