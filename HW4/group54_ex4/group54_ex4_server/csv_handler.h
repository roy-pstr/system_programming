#ifndef _CSV_HANDLER_H
#define _CSV_HANDLER_H

//#define CSV_NAME "C:\\Users\\roypa\\OneDrive\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"
#define CSV_NAME "C:\\Users\\Ilay Amar\\Documents\\GitHub\\system_programming\\HW4\\group54_ex4\\Leaderboard.csv"
//#define CSV_NAME "Leaderborad.csv"
#define ROUNDF(f, d) ((floor(((x)*pow(10,d))+.5))/pow(10,d))
typedef struct node_st
{
	char name[20];
	int won;
	int lost;
	float ratio;
	struct node_st *next;
} Node;

/*OLD are not relevant at the moment*/
int OLD_RefreshLeaderboard(char *filename);
int OLD_UpdateLeaderBoardFile(char *filename, char *name, int win);

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
void LinkedListToStr(Node *head, char **leaderboard_str, int buff_size);
int LengthOfLinkedList(Node *head);
int IsFileExists(char* filename);
/*Move to node module*/

#endif