#include "..\shared\csv_handler.h"
#include "thread_communication.h"
#include "csv_handler.h"
#include "thread_tools.h"

HANDLE create_session_mtx = NULL;
HANDLE second_client_connected_event = NULL;
HANDLE first_player_write_event = NULL;
HANDLE second_player_write_event = NULL;
ErrorCode_t InitThreadCommunicationModule() {
	int ret_val = SUCCESS;

	if (NULL == (create_session_mtx = CreateMutexSimple())) {
		printf("Error when creating Mutex: %d\n", GetLastError());
		ret_val = MUTEX_CREATE_FAILED;
		goto EXIT;
	}
	if (NULL == (second_client_connected_event = CreateEventSimple())) {
		printf("Error when creating Event: %d\n", GetLastError());
		ret_val = MUTEX_CREATE_FAILED;
		goto EXIT;
	}
	if (NULL == (first_player_write_event = CreateEventSimple())) {
		printf("Error when creating Event: %d\n", GetLastError());
		ret_val = MUTEX_CREATE_FAILED;
		goto EXIT;
	}
	if (NULL == (second_player_write_event = CreateEventSimple())) {
		printf("Error when creating Event: %d\n", GetLastError());
		ret_val = MUTEX_CREATE_FAILED;
		goto EXIT;
	}
EXIT:
	return ret_val;
}

ErrorCode_t TryCreateSessionFile(bool *created)
{
	int ret_val = SUCCESS;
	FILE *fp_gamesession = NULL;

	ret_val = WaitForMutex(create_session_mtx);
	GO_TO_EXIT_ON_FAILURE(ret_val, "WaitForMutex() failed!\n");
	
	/* critical code start*/
	if (false == IsFileExists(GS_NAME))
	{
		if (NULL == (fp_gamesession = fopen(GS_NAME, "w")))
		{
			printf("File ERROR\n");
			ret_val = FILE_ERROR;
			goto EXIT;
		}
		*created = true;
	}
	
	/* critical code end */

	ret_val = ReleaseMutexSimp(create_session_mtx);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ReleaseMutexSimp() failed!\n");
EXIT:
	if (fp_gamesession != NULL)
	{
		fclose(fp_gamesession);
	}
	return ret_val;
}

ErrorCode_t WaitForSecondPlayer() {
	DWORD wait_code = WaitForSingleObject(second_client_connected_event, INFINITE);
	if (wait_code != WAIT_OBJECT_0)
	{
		printf("Wair for event failed with %d\n", GetLastError());
		return EVENT_WAIT_ERROR;
	}
	return SUCCESS;
}
ErrorCode_t WaitForFirstPlayerToWriteMove() {
	DWORD wait_code = WaitForSingleObject(first_player_write_event, INFINITE);
	if (wait_code != WAIT_OBJECT_0)
	{
		printf("Wair for event failed with %d\n", GetLastError());
		return EVENT_WAIT_ERROR;
	}
	return SUCCESS;
}
ErrorCode_t WaitForSecondPlayerToWriteMove() {
	DWORD wait_code = WaitForSingleObject(second_player_write_event, INFINITE);
	if (wait_code != WAIT_OBJECT_0)
	{
		printf("Wair for event failed with %d\n", GetLastError());
		return EVENT_WAIT_ERROR;
	}
	return SUCCESS;
}
ErrorCode_t SignleSecondPlayerConnected() {
	DWORD ret_code = SetEvent(second_client_connected_event);
	if (ret_code == 0 )
	{
		printf("SetEvent failed with %d\n", GetLastError());
		return SET_EVENT_ERROR;
	}
	return SUCCESS;
}
ErrorCode_t SignleSecondPlayerWriteMove() {
	DWORD ret_code = SetEvent(second_player_write_event);
	if (ret_code == 0)
	{
		printf("SetEvent failed with %d\n", GetLastError());
		return SET_EVENT_ERROR;
	}
	return SUCCESS;
}
ErrorCode_t SignleFirstPlayerWriteMove() {
	DWORD ret_code = SetEvent(first_player_write_event);
	if (ret_code == 0)
	{
		printf("SetEvent failed with %d\n", GetLastError());
		return SET_EVENT_ERROR;
	}
	return SUCCESS;
}
ErrorCode_t ResetSecondPlayerEvent() {
	return MyResetEvent(second_client_connected_event);
}

ErrorCode_t ResetPlayersWriteEvents() {
	ErrorCode_t ret_val = MyResetEvent(first_player_write_event);
	GO_TO_EXIT_ON_FAILURE(ret_val, "MyResetEvent() failed!\n");
	ret_val = MyResetEvent(second_player_write_event);
	GO_TO_EXIT_ON_FAILURE(ret_val, "MyResetEvent() failed!\n");
EXIT:
	return ret_val;
}

ErrorCode_t WriteAndReadMoves(MOVES_ENUM my_move, MOVES_ENUM *oppent_move, bool first_player) {
	
	ErrorCode_t ret_val = SUCCESS;
	
	if (false == first_player) {
		/* wait for first player to write his move */
		ret_val = WaitForFirstPlayerToWriteMove();
		GO_TO_EXIT_ON_FAILURE(ret_val, "WaitForFirstPlayerToWriteMove() failed.\n");

		/* second player reading first player move */
		ret_val = ReadMove(oppent_move);
		GO_TO_EXIT_ON_FAILURE(ret_val, "ReadMove() failed.\n");
		/* second player writing his move */
		ret_val = WriteMove(MOVES_STRINGS[my_move]);
		GO_TO_EXIT_ON_FAILURE(ret_val, "WriteMove() failed.\n");
		ret_val = SignleSecondPlayerWriteMove();
		GO_TO_EXIT_ON_FAILURE(ret_val, "SignleSecondPlayerWriteMove() failed.\n");
	}
	else {
		/* first player write his move */
		ret_val = WriteMove(MOVES_STRINGS[my_move]);
		GO_TO_EXIT_ON_FAILURE(ret_val, "WriteMove() failed.\n");
		ret_val = SignleFirstPlayerWriteMove();
		GO_TO_EXIT_ON_FAILURE(ret_val, "SignleFirstPlayerWriteMove() failed.\n");
		/*wait for second player to write his move */
		ret_val = WaitForSecondPlayerToWriteMove();
		GO_TO_EXIT_ON_FAILURE(ret_val, "WaitForFirstPlayerToWriteMove() failed.\n");
		/* first player reading first player move */
		ret_val = ReadMove(oppent_move);
		GO_TO_EXIT_ON_FAILURE(ret_val, "ReadMove() failed.\n");
	}
	
	ret_val = ResetPlayersWriteEvents();
	GO_TO_EXIT_ON_FAILURE(ret_val, "ResetPlayersWriteEvents() failed.\n");
EXIT:
	return ret_val;
}
/*function to write the client move*/
ErrorCode_t WriteMove(char *move)
{
	ErrorCode_t ret_val = SUCCESS;
	FILE *fp_gamesession = NULL;

	if (NULL == (fp_gamesession = fopen(GS_NAME, "w")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		goto EXIT;
	}
	fprintf(fp_gamesession, "%s", move);

EXIT:
	if (fp_gamesession != NULL)
	{
		fclose(fp_gamesession);
	}
	return ret_val;
}

/*function to read the other client move*/
ErrorCode_t ReadMove(MOVES_ENUM *move_enum)
{
	ErrorCode_t ret_val = SUCCESS;
	*move_enum = UNDEFINED_MOVE;
	FILE *fp_gamesession = NULL;
	char move[MOVE_STRING_MAX_LEN] = "";
	if (NULL == (fp_gamesession = fopen(GS_NAME, "r")))
	{
		printf("File ERROR\n");
		ret_val = FILE_ERROR;
		goto EXIT;
	}
	fgets(move, MOVE_STRING_MAX_LEN, fp_gamesession);
	*move_enum = StringToEnum(move);
EXIT:
	if (fp_gamesession != NULL)
	{
		fclose(fp_gamesession);
	}
	return ret_val;
}

int DeleteGameSessionFile()
{
	if (remove(GS_NAME) == 0)
	{
		DEBUG_PRINT(printf("File %s deleted\n", GS_NAME));
	}
		
	else {
		DEBUG_PRINT(printf("Unable to delete the file %s\n", GS_NAME));
	}
		
	return 0;
}