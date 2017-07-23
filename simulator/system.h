#pragma once

#include <cstdio>
#include <string>
#include <stdio.h>
#if SOCKET
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#endif

#include "cpu.h"
#include "utils.h"

#define PORT      30000  // 変更 0-65535
#define BUF_SIZE  1024   // バッファのサイズ

class System
{
public:
#define MAX_INTERVAL 50
	class RandomPeripheral
	{
	public:
		CPU * cpu;
		const char * name;
		int type;	/// type = 0 : output, type = 1 : input
		int interval;

        // サーバ用
        static int fd;

		FILE * fp;
		RandomPeripheral(CPU * cpu0, const char * n, int t);
		virtual ~RandomPeripheral();
		enum TraceType
		{
			TT_Interval = 0, TT_Data = 1
		};
		void Open(FILE * fp0);
		void Close();

		int GetPortID();
		const char * GetPortName();
		void RecordTrace(int rtype, unsigned char value);
		virtual int GetInterval();
		void SetRandomInterval();
		int Execute(void(*accessPortHook)(int) = 0);
        //static void SocketStart(int prt);
		///	virtual abstract functions : must be defined in actual class
		virtual bool IsPortReady() = 0;
		virtual void AccessPort() = 0;
		virtual bool AccessHookEnabled() = 0;
#if SOCKET
        //サーバ用
        static int SocketStart();
        static int Connect(const std::string host, int port);
        static void SendMsg(int& fd, char* buf);
        static std::string RecvMsg(int& fd);
#endif
	};

	class TerminalViewer
	{
		void Reset();
	public:
		std::string str;
		TerminalViewer();
		~TerminalViewer();
		void PrintView();
	};

	class InputTerminal : public RandomPeripheral
	{
	public:
		TerminalViewer * termView;
		InputTerminal(CPU * cpu0);

		bool IsPortReady();
		virtual void AccessPort();
		void PutInput(int value);
		virtual bool AccessHookEnabled();
	};

	class OutputTerminal : public RandomPeripheral
	{
	public:
		TerminalViewer * termView;
		OutputTerminal(CPU * cpu0);

		virtual int GetInterval();
		bool IsPortReady();
		virtual void AccessPort();
		int GetOutput();
		virtual bool AccessHookEnabled();
	};
};
