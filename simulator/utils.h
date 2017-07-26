#pragma once
#include <memory>

#define NEED_ECHO (Debugger::echo)
#define NEED_FDMP (Debugger::flog)

#define EMIT_MESSAGE_0(func, fp)							if(NEED_FDMP) func(fp);							if(NEED_ECHO) func(stdout)
#define EMIT_MESSAGE_1(func, fp, arg0)						if(NEED_FDMP) func(fp, arg0);					if(NEED_ECHO) func(stdout, arg0)
#define EMIT_MESSAGE_2(func, fp, arg0, arg1)				if(NEED_FDMP) func(fp, arg0, arg1);				if(NEED_ECHO) func(stdout, arg0, arg1)
#define EMIT_MESSAGE_3(func, fp, arg0, arg1, arg2)			if(NEED_FDMP) func(fp, arg0, arg1, arg2);		if(NEED_ECHO) func(stdout, arg0, arg1, arg2)
#define EMIT_MESSAGE_4(func, fp, arg0, arg1, arg2, arg3)	if(NEED_FDMP) func(fp, arg0, arg1, arg2, arg3);	if(NEED_ECHO) func(stdout, arg0, arg1, arg2, arg3)
#define EMIT_MESSAGE_5(func, fp, arg0, arg1, arg2, arg3, arg4)	\
	if(NEED_FDMP) func(fp, arg0, arg1, arg2, arg3, arg4);	if(NEED_ECHO) func(stdout, arg0, arg1, arg2, arg3, arg4)

#define ABORT_PROGRAM(msg)	msg; printf("hit return : "); getchar(); exit(-1)

#ifdef WIN32
#include <conio.h>
#else	/// LINUX
#include <termios.h>
#endif


#if !defined(WIN32)
class GetChSetting
{
public:
	termios oldTermios, newTermios;
	GetChSetting()
	{
		tcgetattr(0, &oldTermios);
		newTermios = oldTermios;
		newTermios.c_lflag &= ~ICANON;	/// disable buffered i/o
		tcsetattr(0, TCSANOW, &newTermios);
	}
	~GetChSetting()
	{
		tcsetattr(0, TCSANOW, &oldTermios);
	}
};
extern int _getche(void);
#endif
