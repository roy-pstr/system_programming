#include "socket_tools.h"
#include "msg_protocol.h"
#include <stdio.h>
#include <string.h>

void InitSockets(SOCKET *sockets, int size) {
	int Ind;
	for (Ind = 0; Ind < size; Ind++)
	{
		*sockets = INVALID_SOCKET;
	}
}

ErrorCode_t ShutDownAndCloseSocket(SOCKET *t_socket) {
	ErrorCode_t ret_val = SUCCESS;
	/* CLOSE SOCKET */
	if (*t_socket != INVALID_SOCKET) {
		/*shutdown SOCKET*/
		if (shutdown(*t_socket, 2) == SOCKET_ERROR) {
			printf("Failed to shutdown one of the clients connection sockets, error %ld.\n", WSAGetLastError());
		}
		if (closesocket(*t_socket) == SOCKET_ERROR) {
			printf("Failed to close one of the clients connection sockets, error %ld.\n", WSAGetLastError());
		}
		*t_socket = INVALID_SOCKET;
	}
	return ret_val;
}

ErrorCode_t CreateSocket(SOCKET *s) {
	ErrorCode_t ret_val = SUCCESS;
	if (*s != INVALID_SOCKET) {
		closesocket(*s);
	}
	*s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*s == INVALID_SOCKET)
	{
		printf("Error at socket( ) in client: %ld\n", WSAGetLastError());
		ret_val = SOCKET_CREATE_ERROR;
		goto EXIT;
	}
EXIT:
	return ret_val;
}

ErrorCode_t RecvData_WithTimeout(SOCKET *t_socket, protocol_t * prtcl_msg, int timeout_in_seconds) {
	TransferResult_t RecvRes;
	ErrorCode_t ret_val = SUCCESS;
	char *AcceptedStr = NULL;

	int retval_select = 1;
	fd_set set;
	struct timeval timeout;
	FD_ZERO(&set); /* clear the set */
	FD_SET(*t_socket, &set); /* add our file descriptor to the set */
	timeout.tv_sec = timeout_in_seconds;
	timeout.tv_usec = 0;
	if (timeout_in_seconds != INFINITE) {
		retval_select = select(0, &set, NULL, NULL, &timeout);
	}
	if (retval_select == SOCKET_ERROR) {
		// select error...
		printf("Error while waiting in RecvData_WithTimeout\n");
		ret_val = SERVER_RECIVE_FAILED;
		goto EXIT;
	}
	if (retval_select == 0) {
		// timeout, socket does not have anything to read
		printf("TIMEOUT during RecvData_WithTimeout\n");
		ret_val = SERVER_RECV_TIMEDOUT;
		goto EXIT;
	}
	
	DEBUG_PRINT(printf("Waiting to recive something...\n"));
	InitProtocol(prtcl_msg);
	RecvRes = ReceiveString(&AcceptedStr, *t_socket);
	if (RecvRes == TRNS_FAILED)
	{
		printf("Socket error while reading.\n");
		ret_val = SOCKET_ERROR_RECV_DATA;
		goto EXIT;
	}
	else if (RecvRes == TRNS_DISCONNECTED)
	{
		printf("Connection closed while reading.\n");
		ret_val = SOCKET_ERROR_RECV_DATA;
		goto EXIT;
	}

	DEBUG_PRINT(printf("Recived message : %s\n", AcceptedStr));
	ret_val = ParseMessage(AcceptedStr, (int)strlen(AcceptedStr) + 1, prtcl_msg);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ParseMessage() failed!\n");

EXIT:
	if (NULL != AcceptedStr)
		free(AcceptedStr);
	return ret_val;
}

ErrorCode_t SendData(SOCKET *t_socket, protocol_t * prtcl_msg) {
	TransferResult_t SendRes;
	ErrorCode_t ret_val = SUCCESS;
	//DEBUG_PRINT(printf("Enter a message to send to the server:\n"));
	char *send_str;
	ret_val = AllocateString(&send_str, prtcl_msg->size_in_bytes);
	GO_TO_EXIT_ON_FAILURE(ret_val, "AllocateString failed!");
	ret_val = ProtocolToString(prtcl_msg, &send_str);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ProtocolToString() failed.\n");
	SendRes = SendProtocolMsg(send_str, *t_socket);
	if (SendRes == TRNS_FAILED)
	{
		printf("Socket error while trying to write data to socket\n");
		ret_val = SOCKET_ERROR_SEND_DATA;
		goto EXIT;
	}
	DEBUG_PRINT(printf("Message sent: %s\n", send_str));
EXIT:
	if (NULL != send_str)
		free(send_str);
	return ret_val;
}

ErrorCode_t SendProtcolMsgNoParams(SOCKET *t_socket, PROTOCOL_ENUM type) {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t prtcl_msg;
	CreateProtocol(&prtcl_msg, type, NULL, 0);
	ret_val = SendData(t_socket, &prtcl_msg);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SendData() failed!");
	FreeProtocol(&prtcl_msg);
EXIT:
	return ret_val;
}

ErrorCode_t SendProtcolMsgWithParamsList(SOCKET *t_socket, PROTOCOL_ENUM type, param_node **param_list) {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t prtcl_msg;
	CreateProtocolList(&prtcl_msg, type, param_list);
	ret_val = SendData(t_socket, &prtcl_msg);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SendData() failed!");
	FreeProtocol(&prtcl_msg);
EXIT:
	return ret_val;
}

ErrorCode_t SendProtcolMsgWithParams(SOCKET *t_socket, PROTOCOL_ENUM type, char **param_list, int param_list_size) {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t prtcl_msg;
	CreateProtocol(&prtcl_msg, type, param_list, param_list_size);
	ret_val = SendData(t_socket, &prtcl_msg);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SendData() failed!");
	FreeProtocol(&prtcl_msg);
EXIT:
	return ret_val;
}

TransferResult_t SendBuffer( const char* Buffer, int BytesToSend, SOCKET sd )
{
	const char* CurPlacePtr = Buffer;
	int BytesTransferred;
	int RemainingBytesToSend = BytesToSend;
	
	while ( RemainingBytesToSend > 0 )  
	{
		/* send does not guarantee that the entire message is sent */
		BytesTransferred = send (sd, CurPlacePtr, RemainingBytesToSend, 0);
		if ( BytesTransferred == SOCKET_ERROR ) 
		{
			printf("send() failed, error %d\n", WSAGetLastError() );
			return TRNS_FAILED;
		}
		
		RemainingBytesToSend -= BytesTransferred;
		CurPlacePtr += BytesTransferred; // <ISP> pointer arithmetic
	}

	return TRNS_SUCCEEDED;
}

/* return the length of the array of chars till the '\n' first occurance */
int CountNumberOfCharsInMsg(const char *str) {
	int size = 0;
	while (*str != '\n') {
		str++;
		size++;
	}
	size++; /*for the \n*/
	return size;
}

TransferResult_t SendProtocolMsg( const char *Str, SOCKET sd )
{
	/* Send the the request to the server on socket sd */
	int TotalStringSizeInBytes;
	TransferResult_t SendRes;

	/* The request is sent in two parts. First the Length of the string (stored in 
	   an int variable ), then the string itself. */
	TotalStringSizeInBytes = CountNumberOfCharsInMsg(Str);
	//TotalStringSizeInBytes = (int)( strlen(Str) + 1 ); // terminating zero also sent	

	SendRes = SendBuffer( 
		(const char *)( &TotalStringSizeInBytes ),
		(int)( sizeof(TotalStringSizeInBytes) ), // sizeof(int) 
		sd );

	if ( SendRes != TRNS_SUCCEEDED ) return SendRes ;

	SendRes = SendBuffer( 
		(const char *)( Str ),
		(int)( TotalStringSizeInBytes ), 
		sd );

	return SendRes;
}

TransferResult_t ReceiveBuffer( char* OutputBuffer, int BytesToReceive, SOCKET sd )
{
	char* CurPlacePtr = OutputBuffer;
	int BytesJustTransferred;
	int RemainingBytesToReceive = BytesToReceive;

	while ( RemainingBytesToReceive > 0 )  
	{
		/* send does not guarantee that the entire message is sent */
		BytesJustTransferred = recv(sd, CurPlacePtr, RemainingBytesToReceive, 0);
		if ( BytesJustTransferred == SOCKET_ERROR ) 
		{
			printf("recv() failed, error %d\n", WSAGetLastError() );
			return TRNS_FAILED;
		}		
		else if ( BytesJustTransferred == 0 )
			return TRNS_DISCONNECTED; // recv() returns zero if connection was gracefully disconnected.

		RemainingBytesToReceive -= BytesJustTransferred;
		CurPlacePtr += BytesJustTransferred; // <ISP> pointer arithmetic
	}

	return TRNS_SUCCEEDED;
}

TransferResult_t ReceiveString( char** OutputStrPtr, SOCKET sd )
{
	/* Recv the the request to the server on socket sd */
	int TotalStringSizeInBytes;
	TransferResult_t RecvRes;
	char* StrBuffer = NULL;

	if ( ( OutputStrPtr == NULL ) || ( *OutputStrPtr != NULL ) )
	{
		printf("The first input to ReceiveString() must be " 
			   "a pointer to a char pointer that is initialized to NULL. For example:\n"
			   "\tchar* Buffer = NULL;\n"
			   "\tReceiveString( &Buffer, ___ )\n" );
		return TRNS_FAILED;
	}
	
	/* The request is received in two parts. First the Length of the string (stored in 
	   an int variable ), then the string itself. */
		
	RecvRes = ReceiveBuffer( 
		(char *)( &TotalStringSizeInBytes ),
		(int)( sizeof(TotalStringSizeInBytes) ), // 4 bytes
		sd );

	if ( RecvRes != TRNS_SUCCEEDED ) return RecvRes;

	StrBuffer = (char*)malloc( TotalStringSizeInBytes * sizeof(char) );

	if ( StrBuffer == NULL )
		return TRNS_FAILED;

	RecvRes = ReceiveBuffer( 
		(char *)( StrBuffer ),
		(int)( TotalStringSizeInBytes), 
		sd );

	if ( RecvRes == TRNS_SUCCEEDED ) 
		{ *OutputStrPtr = StrBuffer; }
	else
	{
		free( StrBuffer );
	}
		
	return RecvRes;
}
