#pragma once


// ��ȷ����㷨����ʱ�����(��������)����

class CTimeCount
{
private:

	// �㷨����ʱ�� (��λ: ms)
	double use_time;

	// ����ֵ
	LARGE_INTEGER new_time, old_time, frequency;


public:

	// ��ʱ��ʼ
	void Start(void);

	// ��ʱ����
	void End(void);

	// ����㷨����ʱ��(��λ:��)
	double GetUseTime(void);
};
