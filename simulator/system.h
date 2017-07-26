#pragma once

#include "cpu.h"
#include "utils.h"

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

#define PORT      30000  // 変更 0-65535
#define BUF_SIZE  1024   // バッファのサイズ

class System
{
public:
#define MAX_INTERVAL 50
	class RandomPeripheral
	{
	public:
		std::shared_ptr<CPU> cpu;
		const std::string name;
		int type;	/// type = 0 : output, type = 1 : input
		int interval;
		std::shared_ptr<FILE> fp;
		// サーバ用
        static int fd;

		RandomPeripheral(const std::shared_ptr<CPU>& cpu0, const std::string& n, int t);
		virtual ~RandomPeripheral();

		enum TraceType
		{
			TT_Interval = 0, TT_Data = 1
		};

		void Open(const std::shared_ptr<FILE>& fp0);
		void Close();

		int GetPortID();
		const std::string GetPortName();
		void RecordTrace(int rtype, unsigned char value);
		virtual int GetInterval();
		void SetRandomInterval();
		int Execute(std::function<void(int)> accessPortHook = nullptr);

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
		TerminalViewer(const TerminalViewer& rval);
		virtual ~TerminalViewer();
		void PrintView();
	};

	class InputTerminal : public RandomPeripheral
	{
	public:
		std::shared_ptr<TerminalViewer> termView;

		InputTerminal(const std::shared_ptr<CPU>& cpu0);
		virtual ~InputTerminal();

		bool IsPortReady();
		virtual void AccessPort();
		void PutInput(int value);
		virtual bool AccessHookEnabled();
	};

	class OutputTerminal : public RandomPeripheral
	{
	public:
		std::shared_ptr<TerminalViewer> termView;

		OutputTerminal(const std::shared_ptr<CPU>& cpu0);
		virtual ~OutputTerminal();

		virtual int GetInterval();
		bool IsPortReady();
		virtual void AccessPort();
		int GetOutput();
		virtual bool AccessHookEnabled();
	};
};
