#pragma once


// 精确获得算法处理时间的类(毫秒量级)

class CTimeCount
{
private:

	// 算法处理时间 (单位: ms)
	double use_time;

	// 计数值
	LARGE_INTEGER new_time, old_time, frequency;


public:

	// 计时开始
	void Start(void);

	// 计时结束
	void End(void);

	// 获得算法处理时间(单位:秒)
	double GetUseTime(void);
};
