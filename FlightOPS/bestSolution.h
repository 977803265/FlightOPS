/*************************************************
��Ȩ: �μ�˶
����: �μ�˶
����: 2016-10-16
����: ����ͣ��λ��Դ�����Ż�_���Ž���
�޸���ʷ:

**************************************************/

/*<remark>=========================================================
/// <summary>
/// ����ͣ��λ��Դ�����Ż�_���Ž���
/// </summary>
===========================================================</remark>*/

#pragma once

#include "basic.h"
#include "flight.h"

#pragma region �½�����
////////////////////////////////////////////////////////////////////////
class BestSolution;// ���Ž���
////////////////////////////////////////////////////////////////////////
#pragma endregion

class BestSolution
{
#pragma region Variables

public:// ���Ž����
	static vector<string>			m_flightNo;						// �����
	static vector<string>			m_inTime;							// ����λʱ�䣬�ַ���
	static vector<string>			m_outTime;						// ����λʱ�䣬�ַ���
	static vector<string>			m_gatePositionNo;					// ��ѡ��λ����
	static double					m_objValue;						// ����Ŀ�꺯��ֵ
	static int					m_distributionFlightCount;			// ��û�λ�ĺ�����
	static int					m_distanceGatePositionCount;		// ���ź�����
	static int					m_distanceGatePositionNumberCount;	// �����ÿ���
	static int					m_temporaryMeasuresSum;			// ִ����ʱ��ʩ������
	static int					m_conflictCount;					// ����ͨ����ͻ������
	static vector<vector<int>>		m_solution;						// cplex���Ž�
	static int					m_deleteNum;						// ���ʱɾ����ͻ��������
	static int					m_heuristicFlightNum;				// ����ʽ���ź�������

#pragma endregion

#pragma region Functions

public:
	//
	// ժҪ:
	//     ����cplex�Ľ�������ݡ�
	static void updateValueByCplexSolver();
	//
	// ժҪ:
	//     ɾ���ֶ�cplex�Ļ��⡣
	static void deleteWrongValue();
	//
	// ժҪ:
	//     ����ʽ��δ����ĺ������ͣ��λ��
	static void heuristicForUndistributed();
	//
	// ժҪ:
	//     ���������Ŀǰ���Ž⣬��������Ž⡣
	static void updateBestValue();
	//
	// ժҪ��
	//     ��ӡ�����
	static void printBestResult();
	//
	// ժҪ��
	//     ����Ŀ�꺯����
	static double getObjectiveFunction();
	//
	// ժҪ��
	//     ��������
	static bool less_first_time_t(const pair<time_t, Flight *> & m1, const pair<time_t, Flight *> & m2);

#pragma endregion
};