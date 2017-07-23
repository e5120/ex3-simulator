#include "system.h"
#include <iostream>

int System::RandomPeripheral::fd =0;

System::RandomPeripheral::RandomPeripheral(const std::shared_ptr<CPU>& cpu0, const std::string n, int t)
		: name(n), type(t), interval(-1), fp(0)
{
	cpu = cpu0;
	std::cout << "system : " <<cpu.get() << std::endl;
}

System::RandomPeripheral::~RandomPeripheral()
{
	if (fp)
	{
		fclose(fp);
	}
#if SOCKET
    close(fd);
#endif
}


void System::RandomPeripheral::Open(FILE * fp0)
{
	fp = fp0;
	if (fp)
	{
		fprintf(fp, "/// RandomPeripheral trace (%s) :\n", name.c_str());
		fprintf(fp, "/// bit[17]   (port ID) : 0 = gpio, 1 = uart\n");
		fprintf(fp, "/// bit[16]   (trace type) : 0 = interval cycles, 1 = data\n");
		fprintf(fp, "/// bit[15:0] (value)\n\n");
	}
}

void System::RandomPeripheral::Close()
{
	if (type == 0 && IsPortReady())
	{
		AccessPort();
	}	///	access output ready port before closing...
	if (fp)
	{
		fclose(fp);
	}
	fp = 0;
}

int System::RandomPeripheral::GetPortID()
{
	return cpu->GetSelectedPortID();
}

const std::string System::RandomPeripheral::GetPortName()
{
	return (GetPortID()) ? "SIO" : "PIO";
}

void System::RandomPeripheral::RecordTrace(int rtype, unsigned char value)
{
	if (fp)
	{
		fprintf(fp, "%x%04x\n", (GetPortID() << 1) | rtype, value);
	}
}

int System::RandomPeripheral::GetInterval()
{
	return rand() % MAX_INTERVAL;
}
void System::RandomPeripheral::SetRandomInterval()
{
	interval = GetInterval();
	EMIT_MESSAGE_4(fprintf, cpu->fplog,
		"-------------------------------------\n"
		"%s[%s].interval = %d\n"
		"-------------------------------------\n"
		, name.c_str(), GetPortName().c_str(), interval);
	RecordTrace(TT_Interval, interval);
}

int System::RandomPeripheral::Execute(void(*accessPortHook)(int))
{
	if (interval < 0 && IsPortReady())
	{
		SetRandomInterval();
	}
	if (interval == 0)
	{
		(accessPortHook && AccessHookEnabled()) ? accessPortHook(type) : AccessPort();
	}
	if (interval >= 0)
	{
		interval--;
	}
    return 0;
}

#if SOCKET
int System::RandomPeripheral::SocketStart() {
	// サーバと接続
	if ((fd = Connect("127.0.0.1", PORT)) < 0) {
		return 1;
	}
	return 0;
}

void System::RandomPeripheral::SendMsg(int &fd, char* buf) {
	buf[strlen(buf)] = '\0';
	send(fd, buf, strlen(buf),0);
}

std::string System::RandomPeripheral::RecvMsg(int &fd) {
    std::string str = "";
    char buf[BUF_SIZE];
    int no = 0;

    while(true){
        if ((no = recv(fd, buf, sizeof(buf), MSG_DONTWAIT)) < 1) {
            if (errno == EAGAIN) {
                break;
            } else {
                printf("受信エラー\n");
            }
        }
        buf[no] = '\0';
        str += buf;
    }
    return str;
}


int System::RandomPeripheral::Connect(const std::string host, int port) {
	int                fd;
	struct sockaddr_in addr;
	struct hostent     *hp;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}
	if ((hp = gethostbyname(host.c_str())) == NULL) {
		fprintf(stderr, "gethost error %s\n", host.c_str());
		close(fd);
		return -1;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);
	if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("connect");
		return -1;
	}
	return fd;
}
#endif
void System::TerminalViewer::Reset()
{
	str = "";
}

System::TerminalViewer::TerminalViewer()
{
}

System::TerminalViewer::~TerminalViewer()
{
}

void System::TerminalViewer::PrintView()
{
#if 1
	printf("------------ TERMINAL VIEWER -------------\n");
	printf("%s\n", str.c_str());
	printf("------------------------------------------\n");
	Reset();
#else
    printf("------------ TERMINAL VIEWER -------------\n");
    //printf("length : %lu\t%s\n", str.size(), str.c_str());
    for (int i = 1; i < str.size(); i++) {
        if (str[i - 1] == 'X')printf("X: %d\n", str[i]);
        if (str[i - 1] == 'Y')printf("Y: %d\n", str[i]);
        if (str[i - 1] == 'B')printf("B: %d\n", str[i]);
    }
    int width = 47; //# 0~44 #
    int height = 33; // 0 ~ 30 + 2
    if (str[1] != 'X') {
        system("clear");
        printf("\033[40m");
        printf("                                               \n");
        for (int i = 0; i <= 30; i++) {
            printf("\033[40m ");
            printf("\033[49m");
            printf("                                             ");
            printf("\033[40m \n");
            printf("\033[49m");
        }
        printf("\033[40m");
        printf("                                               \n");
        printf("\033[49m");
        printf("\033[30m");
        printf("\033[%d;%dH=====", 2, str[9] + 1);
        printf("\033[%d;%dH=====", height - 1, str[10] + 1);
        printf("\033[%d;%dHO", str[8] + 1, str[7] + 1);
        printf("\033[%d;1H",height + 1);
        printf("\033[39m");
        printf("BX:%d  BY:%d  B2:%d  B1:%d\n", str[7], str[8], str[9], str[10]);
        for (int i = 1; i < str.size(); i++) {
            printf("%d\n",str[i]);
        }
    }
    printf("------------------------------------------\n");
    Reset();
#endif
}

System::InputTerminal::InputTerminal(const std::shared_ptr<CPU>& cpu0) : RandomPeripheral(cpu0, "in", 1), termView()
{
}

bool System::InputTerminal::IsPortReady()
{
	return cpu->IsWaitingForInput();
}

void System::InputTerminal::AccessPort()
{
	termView->PrintView();
	printf("IN-term [%s] > ", GetPortName().c_str());
	int value = _getche();
	printf("\n");
	switch (value)
	{
	case 0x03:	/// ctrl-C
		ABORT_PROGRAM(printf("Program terminated on Input terminal (key = %02x)\n", value););
	case 0x1B:	/// ESC
		cpu->dbg->Command();
		return;
	}
	PutInput(value);
}

void System::InputTerminal::PutInput(int value) {
#if SOCKET
    char tmp[3];
    tmp[0] = (unsigned char)value;
    tmp[1] = '\n';

    cpu->SetInput((unsigned char)value);
    if(!GetPortID()){
        SendMsg(fd, tmp);
    //    termView->str += RecvMsg(fd);
    }
	EMIT_MESSAGE_5(fprintf, cpu->fplog,
		"-------------------------------------\n"
		"cpu->input[%s] = 0x%02x (%2d : '%c')\n"
		"-------------------------------------\n"
		, GetPortName(), value, value, value);

    RecordTrace(TT_Data, value);
#else
    termView->str += (unsigned char)value;
    cpu->SetInput((unsigned char) value);
    EMIT_MESSAGE_5(fprintf, cpu->fplog,
                   "-------------------------------------\n"
                           "cpu->input[%s] = 0x%02x (%2d : '%c')\n"
                           "-------------------------------------\n"
    , GetPortName().c_str(), value, value, value);
    RecordTrace(TT_Data, value);

#endif
}

bool System::InputTerminal::AccessHookEnabled()
{
	return true;
}

System::OutputTerminal::OutputTerminal(const std::shared_ptr<CPU>& cpu0) : RandomPeripheral(cpu0, "out", 0), termView()
{
}

int System::OutputTerminal::GetInterval()
{
	return 0;
}

bool System::OutputTerminal::IsPortReady()
{
	return cpu->IsOutputReady();
}

void System::OutputTerminal::AccessPort()
{
#if SOCKET
    int value = GetOutput();
    if(GetPortID()){
        //printf("called\n");
        RecordTrace(TT_Data, value);
        termView->str += value;
        termView->PrintView();
    }
    else{
        termView->str += RecvMsg(fd);
        if(!termView->str.empty()){
            termView->PrintView();
        }
    }
#else
    int value = GetOutput();
    RecordTrace(TT_Data, value);
    termView->str += value;
    //termView->PrintView();
#endif
 }

int System::OutputTerminal::GetOutput()
{
	int value = cpu->GetOutput();
	EMIT_MESSAGE_5(fprintf, cpu->fplog,
		"-------------------------------------\n"
		"cpu->output[%s] = 0x%02x (%2d : '%c')\n"
		"-------------------------------------\n"
		, GetPortName().c_str(), value, value, value);
	return value;
}

bool System::OutputTerminal::AccessHookEnabled()
{
	return cpu->IsOutputReady();
}
