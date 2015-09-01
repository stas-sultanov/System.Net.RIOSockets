﻿using System;
using System.Net.RIOSockets;

namespace Test
{
	class Program
	{
		static void Main(String[] args)
		{
			// 0 try initialize server
			var tryInitialize = TcpSocketServer.TryInitialize(10202);

			if (!tryInitialize.Success)
			{
				Console.WriteLine($"error initializing server socket:: {tryInitialize.ErrorCode}");
			}

			var socketServer = tryInitialize.Result;

			// 1 try accept request
			var tryAccept = socketServer.TryAccept();

			if (!tryAccept.Success)
			{
				Console.WriteLine($"error accepting request :: {tryInitialize.ErrorCode}");
			}

		}
	}
}