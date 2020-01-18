#pragma once

class TimeTool
{
private:
	TimeTool() = delete;
	TimeTool& operator=(const TimeTool&) = delete;
	TimeTool(const TimeTool&) = delete;
	~TimeTool() = delete;

public:
	static double GetTime();
};