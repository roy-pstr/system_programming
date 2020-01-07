#ifndef _CSV_HANDLER_H
#define _CSV_HANDLER_H

#define CSV_NAME "\\Leaderborad.csv"
#define ROUNDF(f, d) ((floor(((x)*pow(10,d))+.5))/pow(10,d))
typedef struct node_st
{
	char name[20];
	int won;
	int lost;
	float ratio;
	struct node_st *next;
} Node;
int RefreshLeaderboard(char *filename);
int UpdateLeaderBoardFile(char *filename, char *name, int win);
Node *DetectAndUpdateElement(Node **head, char* name, int w);
void sortedInsert(Node** head, Node* new_node);
Node *CreateNode(char *name, int win, int lose);


double Round(double x, int p);
/*Move to node module*/

#endif