
#include "DebugTools.h"
#include <ctime>

DeBugTool* DeBugTool::Single = nullptr;
DeBugTool::DeBugTool()
	:LogFile("./Log.txt",std::ios::out)
{

}

DeBugTool::~DeBugTool()
{
	if (LogFile.is_open())
		LogFile.close();
}

DeBugTool* DeBugTool::GetInstance()
{
	if (!Single)
		Single = new DeBugTool();
	return Single;
}

void DeBugTool::ShutDown()
{
	if (Single)
	{
		delete Single;
		Single = nullptr;
	}
		
}

void DeBugTool::LogMessage(std::string Level, std::string Message)
{
	time_t NowTime;
	tm LC;
	NowTime = std::time(&NowTime);
	localtime_s(&LC,&NowTime);
	LogFile << "[" << LC.tm_year<<"/"<<LC.tm_mon<<"/"<<LC.tm_yday<<":"<<LC.tm_hour <<"] \t"<<Level<<":\t";
	LogFile << Message << '\n';
}

