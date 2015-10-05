#pragma once

#include "Stdafx.h"
#include "WinsockEx.h"
#include "Overlap.h"

namespace SXN
{
	namespace Net
	{
		public class TcpConnection final
		{
			public:

			#pragma region Fields

			WinsockEx* pWinsockEx;

			SOCKET listenSocket;

			SOCKET socket;

			RIO_RQ rioRequestQueue;

			PVOID addrBuf;

			WSAOVERLAPPEDPLUS* acceptOverlapped;

			WSAOVERLAPPEDPLUS* disconnectOverlaped;

			ULONG id;

			PRIO_BUF receiveBuffer;

			PRIO_BUF sendBuffer;

			#pragma endregion

			public:

			#pragma region Constructor & Destructor

			/// <summary>
			/// Initializes a new instance of the <see cref="TcpConnection" /> class.
			/// </summary>
			inline TcpConnection(SOCKET listenSocket, WinsockEx* pWinsockEx, int id, SOCKET socket, RIO_RQ requestQueue, HANDLE complitionPort)
			{
				this->listenSocket = listenSocket;

				this->pWinsockEx = pWinsockEx;

				this->id = id;

				this->socket = socket;

				this->rioRequestQueue = requestQueue;

				this->addrBuf = new char[(sizeof(sockaddr_in) + 16) * 2];

				{
					this->acceptOverlapped = new WSAOVERLAPPEDPLUS();

					memset(acceptOverlapped, 0, sizeof(WSAOVERLAPPEDPLUS));

					acceptOverlapped->action = SOCK_ACTION_ACCEPT;

					acceptOverlapped->connectionId = id;

					acceptOverlapped->connectionSocket = socket;

					acceptOverlapped->completionPort = complitionPort;
				}

				{
					this->disconnectOverlaped = new WSAOVERLAPPEDPLUS();

					memset(disconnectOverlaped, 0, sizeof(WSAOVERLAPPEDPLUS));

					disconnectOverlaped->action = SOCK_ACTION_DISCONNECT;

					disconnectOverlaped->connectionId = id;

					disconnectOverlaped->connectionSocket = socket;

					disconnectOverlaped->completionPort = complitionPort;
				}
			}

			inline ~TcpConnection()
			{
				delete addrBuf;
			}

			#pragma endregion

			#pragma region Methods

			inline BOOL StartAccept()
			{
				DWORD dwBytes;

				return pWinsockEx->AcceptEx(listenSocket, socket, addrBuf, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, &dwBytes, acceptOverlapped);
			}

			inline BOOL StartRecieve()
			{
				return pWinsockEx->RIOReceive(rioRequestQueue, receiveBuffer, 1, 0, (LPVOID)id);
			}

			inline BOOL StartSend(DWORD dataLength)
			{
				sendBuffer->Length = dataLength;

				return pWinsockEx->RIOSend(rioRequestQueue, sendBuffer, 1, 0, (LPVOID)id);
			}

			inline BOOL StartDisconnect()
			{
				return pWinsockEx->DisconnectEx(socket, disconnectOverlaped, TF_REUSE_SOCKET, 0);
			}

			#pragma endregion
		};
	}
}