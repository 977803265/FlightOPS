/*************************************************
��Ȩ: �μ�˶
����: �μ�˶
����: 2015-06-20
����: ����ͣ��λ��Դ�����Ż�_�������
�޸���ʷ:

**************************************************/

/*<remark>=========================================================
/// <summary>
/// ����ͣ��λ��Դ�����Ż�_�������
/// </summary>
===========================================================</remark>*/

#pragma once

#include "random.h"

void Random::Initialize()
{
	srand(unsigned(time(0)));//�Ե�ǰʱ����Ϊ���������	
	GetRandom();//��������ɵ�ʱ�򣬵�һ����Ϊ���㣬�����
}

int Random::GetRandom(int min, int max)
{
	//rand(���Ǻ������еĺ����������ǲ���һ��1��RAND_MAX֮�������
	return (int)(min + (max - min + 1) * rand() / (RAND_MAX + 1.0));
}

double Random::GetRandom()
{
	return (rand() / (RAND_MAX + 0.0));
}

vector<int> Random::GetRandomSequence(int lengthOfSeq)
{
	vector<int> randomSeq(lengthOfSeq);

	//��ʼ��
	for (int i = 0; i < lengthOfSeq; i++)
	{
		randomSeq[i] = i;
	}

	//��������
	for (int i = 0; i < lengthOfSeq; i++)
	{
		int firstPos = GetRandom(0, lengthOfSeq - 1);
		int secondPos = GetRandom(0, lengthOfSeq - 1);
		int temp = randomSeq[firstPos];
		randomSeq[firstPos] = randomSeq[secondPos];
		randomSeq[secondPos] = temp;
	}

	return randomSeq;
}