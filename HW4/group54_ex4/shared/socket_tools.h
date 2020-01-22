#ifndef SOCKET_TOOLS_H
#define SOCKET_TOOLS_H

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "utils.h"
#include "msg_protocol.h"

void InitSockets(SOCKET * sockets, int size);
ErrorCode_t ShutDownAndCloseSocket(SOCKET * t_socket);
ErrorCode_t CreateSocket(SOCKET * s);
ErrorCode_t RecvData_WithTimeout(SOCKET * t_socket, protocol_t * prtcl_msg, int timeout_in_seconds);
ErrorCode_t RecvData(SOCKET * t_socket, protocol_t * prtcl_msg);
ErrorCode_t SendData(SOCKET * t_socket, protocol_t * prtcl_msg);
ErrorCode_t SendProtcolMsgNoParams(SOCKET * t_socket, PROTOCOL_ENUM type);
ErrorCode_t SendProtcolMsgWithParamsList(SOCKET * t_socket, PROTOCOL_ENUM type, param_node ** param_list);
ErrorCode_t SendProtcolMsgWithParams(SOCKET * t_socket, PROTOCOL_ENUM type, char ** param_list, int param_list_size);

typedef enum { TRNS_FAILED, TRNS_DISCONNECTED, TRNS_SUCCEEDED } TransferResult_t;

/**
 * SendBuffer() uses a socket to send a buffer.
 *
 * Accepts:
 * -------
 * Buffer - the buffer containing the data to be sent.
 * BytesToSend - the number of bytes from the Buffer to send.
 * sd - the socket used for communication.
 *
 * Returns:
 * -------
 * TRNS_SUCCEEDED - if sending succeeded
 * TRNS_FAILED - otherwise
 */
	
 TransferResult_t SendBuffer( const char* Buffer, int BytesToSend, SOCKET sd );

/**
 * SendString() uses a socket to send a string.
 * Str - the string to send. 
 * sd - the socket used for communication.
 */ 
TransferResult_t SendString( const char *Str, SOCKET sd );

/**
 * Accepts:
 * -------
 * ReceiveBuffer() uses a socket to receive a buffer.
 * OutputBuffer - pointer to a buffer into which data will be written
 * OutputBufferSize - size in bytes of Output Buffer
 * BytesReceivedPtr - output parameter. if function returns TRNS_SUCCEEDED, then this 
 *					  will point at an int containing the number of bytes received.
 * sd - the socket used for communication.
 *
 * Returns:
 * -------
 * TRNS_SUCCEEDED - if receiving succeeded
 * TRNS_DISCONNECTED - if the socket was disconnected
 * TRNS_FAILED - otherwise
 */ 
TransferResult_t ReceiveBuffer( char* OutputBuffer, int RemainingBytesToReceive, SOCKET sd );

/**
 * ReceiveString() uses a socket to receive a string, and stores it in dynamic memory.
 * 
 * Accepts:
 * -------
 * OutputStrPtr - a pointer to a char-pointer that is initialized to NULL, as in:
 *
 *		char *Buffer = NULL;
 *		ReceiveString( &Buffer, ___ );
 *
 * a dynamically allocated string will be created, and (*OutputStrPtr) will point to it.
 * 
 * sd - the socket used for communication.
 * 
 * Returns:
 * -------
 * TRNS_SUCCEEDED - if receiving and memory allocation succeeded
 * TRNS_DISCONNECTED - if the socket was disconnected
 * TRNS_FAILED - otherwise
 */ 
TransferResult_t ReceiveString( char** OutputStrPtr, SOCKET sd );

#endif // SOCKET_SEND_RECV_TOOLS_H