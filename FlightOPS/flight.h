/*************************************************
��Ȩ: �μ�˶
����: �μ�˶
����: 2016-10-15
����: ����ͣ��λ��Դ�����Ż�_������
�޸���ʷ:

**************************************************/

/*<remark>=========================================================
/// <summary>
/// ����ͣ��λ��Դ�����Ż�_������
/// </summary>
===========================================================</remark>*/

#pragma once

#include "basic.h"
#include "gatePosition.h"
#include "time.h"

#pragma region �½�����
////////////////////////////////////////////////////////////////////////
class Flight;// ������
class GatePosition;	// ͣ��λ��
////////////////////////////////////////////////////////////////////////
#pragma endregion

class Flight
{

#pragma region Variables

public:// �������
	string					m_companyNo;						// ���չ�˾
	string					m_flightNo;						// �����
	string					m_inTime;							// ����λʱ�䣬�ַ���
	string					m_outTime;						// ����λʱ�䣬�ַ���
	time_t					m_inTime_t;						// ����λʱ�䣬ʱ���ʽ
	time_t					m_outTime_t;						// ����λʱ�䣬ʱ���ʽ
	string					m_internationalOrDomestic;			// ���ʹ�������
	int						m_internationalOrDomesticInt;		// ���ʹ������ԣ�1Ϊ���ڣ�2λ����
	string					m_flyingCommission;				// ��������
	string					m_model;							// ����
	int						m_number;							// ����ÿ���
	map<string, GatePosition*>	possibleGatePosition;				// ��ǰ�����ѡ��λ��map���ϣ�firstΪ��λ���룬secondΪ��λ
	GatePosition *			gatePosition;						// ��ѡ��λ

public:// ���༯��
	static map<string, Flight *>		s_mapSetOfFlight;				// �����map���ϣ�key.firstΪ����ţ�key.valueΪ����λʱ�䣬valueΪ����
	static int						s_flightCount;				// �������
	static map<string, Flight *>		s_mapSetOfwrongFlight;			// ���󺽰��map���ϣ�keyΪ����ţ�valueΪ����
	static int						s_wrongFlightCount;			// ����λʱ��ȳ���λʱ���ĺ������
	static int						s_nemberCount;				// �ÿ�������
	static vector<pair<Flight *, vector<GatePosition*>>>	initPossibleGatePosition;	// ��ʼ�����ѡ��λ��map���ϣ�firstΪ��λ���룬secondΪ��λ

public:// ���Լ���
	static vector<string>		s_vectorOfCompanyNo;					// ���к��չ�˾����
	static vector<string>		s_vectorOfFlyingCommission;				// ���з������񼯺�

#pragma endregion

#pragma region Functions

public:

	#pragma region ��������
	//////////////////////////////////////////////////////////////////////////

	//
	// ժҪ:
	//     ���캯����
	Flight(string	 companyNo, string flightNo, string inTime, string outTime, string internationalOrDomestic, string flyingCommission, string model, int number);
	//
	// ժҪ��
	//     ��ȡ���ݡ�
	static void readData();
	//
	// ժҪ��
	//     ͳ���ÿ���������
	static void initNumberCount();
	//
	// ժҪ��
	//     ��ӡ����ʱ�䡣
	static void printTime();
	//
	// ժҪ��
	//     ��ӡ����ռ�û���ʱ�䡣
	static void printRoadTime();
	//
	// ժҪ:
	//     ��ӡ���ܻ�����ͻ����(N*N��������ʵ��ʹ�û���������ͻΪ1)���ֱ��ӡ����A�뺽��B���뻬����ͻ���Ƴ�������ͻ���������Ƴ����Ƴ��뻬�롣
	static void printConflictFlight();
	//
	// ժҪ:
	//     ��ӡÿ�ܺ�����ÿ�������
	static void printNumberOfFlight();
	//
	// ժҪ��
	//     ��ӡ�����
	static void printResult();
	//																											
	// ժҪ��																									 
	//     ��������
	static void clearResult();
	//																											
	// ժҪ��																									 
	//     ���һ������ļƻ���Ϣ��
	static void clearOneFlight(Flight *flight);

	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region �������
	//////////////////////////////////////////////////////////////////////////

	//
	// ժҪ��
	//     ��ú������ʹ�õ����л�λ��
	static void GetFeasibleGatePosition();

	////////////////////////////////////////////////////////////////////////
	#pragma endregion

#pragma endregion

};