/*socket_tools module includes all the help function for handling socket operaitions */

#ifndef SOCKET_TOOLS_H
#define SOCKET_TOOLS_H

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "utils.h"
#include "msg_protocol.h"

/* initailize array of sockets with INVALID_SCOKET*/
void InitSockets(SOCKET * sockets, int size);

/* shutdown and close the given socket */
ErrorCode_t ShutDownAndCloseSocket(SOCKET * t_socket);

/* create a socket */
ErrorCode_t CreateSocket(SOCKET * s);

/* function for wait to socket recv data with timeout parameter! */
ErrorCode_t RecvData_WithTimeout(SOCKET * t_socket, protocol_t * prtcl_msg, int timeout_in_seconds);

/* function for sending protocol message via socket */
ErrorCode_t SendData(SOCKET * t_socket, protocol_t * prtcl_msg);

/* function for sending a protocol message without parameters */
ErrorCode_t SendProtcolMsgNoParams(SOCKET * t_socket, PROTOCOL_ENUM type);

/* function for sending a protocol message with parameters list as param linked list
	!! param_node ** param_list -> free responsable move to protocol_t object inside SOCKET * t_socket. !!
*/
ErrorCode_t SendProtcolMsgWithParamsList(SOCKET * t_socket, PROTOCOL_ENUM type, param_node ** param_list);

/* function for sending a protocol message with parameters list as array of strings */
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
 * prtcl_msg - An array of char terminate by '\n' char!
 * sd - the socket used for communication.
 */ 
TransferResult_t SendProtocolMsg( const char *prtcl_msg, SOCKET sd );

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

#endif 