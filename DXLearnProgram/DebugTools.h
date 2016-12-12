#pragma once 

#include <fstream>
#include <string>



class DeBugTool
{

public:
	static DeBugTool* GetInstance();
	static void ShutDown();
	void LogMessage(std::string Level,std::string Message);
private:
	DeBugTool();
	virtual~DeBugTool();

private:
	static DeBugTool* Single;
	std::ofstream LogFile;
};

#define DX_LOG(LogLevel,Message)\
	{	\
		DeBugTool::GetInstance()->LogMessage(LogLevel,Message); \
	}