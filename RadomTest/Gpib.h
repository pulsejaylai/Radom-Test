extern "C" __declspec(dllimport)long __stdcall  viOpenDefaultRM(long*);
extern "C" __declspec(dllimport)long __stdcall  viStatusDesc(long,long,char*);
extern "C" __declspec(dllimport)long __stdcall  viFindRsrc(long,char*,long*,long*,char*);
extern "C" __declspec(dllimport)long __stdcall  viFindNext(long,char*);
extern "C" __declspec(dllimport)long __stdcall  viOpen(long,char*,long,unsigned long,long*);
extern "C" __declspec(dllimport)long __stdcall  viSetAttribute(long,long,long);
extern "C" __declspec(dllimport)long __stdcall  viWrite(long,char*,unsigned long,unsigned long*);
extern "C" __declspec(dllimport)long __stdcall  viRead(long,char*,unsigned long,unsigned long*);
extern "C" __declspec(dllimport)long __stdcall  viClose(long);

void Gpwrite(char Address[256],char cmd[256]);
void Gpread(char Address[256],char cmd[256],char buf[256]);
void Gpread2(char Address[256],char cmd[256],char buf[1024]);
void Gpread3(char Address[256],char cmd[256],char buf[43]);
void Gpib_Init();
void delay(int Time);
char * Gpiblist();