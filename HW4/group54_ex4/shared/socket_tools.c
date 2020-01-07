#include "socket_tools.h"
#include <stdio.h>
#include <string.h>

void InitSockets(SOCKET *sockets, int size) {
	int Ind;
	for (Ind = 0; Ind < size; Ind++)
	{
		*sockets = INVALID_SOCKET;
	}
}

ErrorCode_t RecvData(SOCKET *t_socket, protocol_t * prtcl_msg) {
	TransferResult_t RecvRes;
	ErrorCode_t ret_val = SUCCESS;
	char *AcceptedStr = NULL;
	DEBUG_PRINT(printf("Waiting to recive something...\n"));
	RecvRes = ReceiveString(&AcceptedStr, *t_socket);
	if (RecvRes == TRNS_FAILED)
	{
		printf("Service socket error while reading.\n");
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
	ret_val = AllocateString(&send_str, PROTOCOL_MESSAGE_MAX_LEN);
	GO_TO_EXIT_ON_FAILURE(ret_val, "AllocateString failed!");
	ret_val = ProtocolToString(prtcl_msg, &send_str);
	GO_TO_EXIT_ON_FAILURE(ret_val, "ProtocolToString() failed.\n");
	SendRes = SendString(send_str, *t_socket);
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

ErrorCode_t SendProtcolMsg(SOCKET *t_socket, PROTOCOL_ENUM type) {
	ErrorCode_t ret_val = SUCCESS;
	protocol_t prtcl_msg;
	SetProtocol(&prtcl_msg, type, NULL, 0);
	ret_val = SendData(t_socket, &prtcl_msg);
	GO_TO_EXIT_ON_FAILURE(ret_val, "SendData() failed!");
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

TransferResult_t SendString( const char *Str, SOCKET sd )
{
	/* Send the the request to the server on socket sd */
	int TotalStringSizeInBytes;
	TransferResult_t SendRes;

	/* The request is sent in two parts. First the Length of the string (stored in 
	   an int variable ), then the string itself. */
		
	TotalStringSizeInBytes = (int)( strlen(Str) + 1 ); // terminating zero also sent	

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
