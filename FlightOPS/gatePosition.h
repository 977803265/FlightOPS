/*************************************************
��Ȩ: �μ�˶
����: �μ�˶
����: 2016-10-15
����: ����ͣ��λ��Դ�����Ż�_ͣ��λ��
�޸���ʷ:

**************************************************/

/*<remark>=========================================================
/// <summary>
/// ����ͣ��λ��Դ�����Ż�_ͣ��λ��
/// </summary>
===========================================================</remark>*/

#pragma once

#include "basic.h"
#include "flight.h"

#pragma region �½�����
////////////////////////////////////////////////////////////////////////
class GatePosition;	// ͣ��λ��
class Road;			// ���е���
class Flight;			// ������
////////////////////////////////////////////////////////////////////////
#pragma endregion

class GatePosition
{

#pragma region Variables

public:// ͣ��λ����
	string					m_summary;						// ����
	string					m_gatePositionNo;					// ͣ��λ����
	string					m_internationalOrDomestic;			// ���ʹ�������
	string					m_distance;						// Զ����λ����
	string					m_inRoadNo;						// ���뻬�е�����
	string					m_outRoadNo;						// �Ƴ����е�����
	vector<string>			m_flyingCommission;				// ��������
	vector<string>			m_companyNo;						// ���չ�˾
	vector<string>			m_model;							// ����
	vector<string>			m_conflictGatePosition;			// Ӱ��Ļ�λ���뼯�ϣ���Ӱ����ˣ�
	vector<string>			m_influenceGatePositionIn;			// Ӱ�컬���λ���뼯�ϣ���Ӱ����ˣ�
	vector<string>			m_influenceGatePositionOut;			// Ӱ�컬����λ���뼯�ϣ���Ӱ����ˣ�
	vector<GatePosition*>		m_beConflictGatePosition;			// Ӱ��Ļ�λ���ϣ�����Ӱ���ң�
	vector<GatePosition*>		m_beInfluenceGatePositionIn;		// Ӱ�컬���λ���ϣ�����Ӱ���ң�
	vector<GatePosition*>		m_beInfluenceGatePositionOut;		// Ӱ�컬����λ���ϣ�����Ӱ���ң�
	map<string, Flight*>		possibleFlight;					// ��ǰ��λ��ѡ�����map���ϣ�firstΪ������룬secondΪ����
	int						temporaryMeasures;					// �Ƿ�Ϊ��ʱ��ʩ����Ϊ1������Ϊ0

public:// ͣ��λʱ�����
	map<time_t, time_t>		m_mapOfStartTimeToFinishTime;		// ͣ��λͣ��ʱ�̵ļ��ϣ�keyΪͣ��λͣ����ʼʱ�̣�valueΪͣ��λͣ������ʱ��
	map<time_t, Flight *>		m_mapOfStartTimeToFlight;			// ͣ��λͣ���ĺ���ļ��ϣ�keyΪͣ��λͣ����ʼʱ�̣�valueΪ�����

public:// ���ʹ�������
	int						m_internationalOrDomesticInt;		// 1Ϊ���ڣ�2Ϊ���⣬3Ϊ���ڹ������

public:// �������е�
	Road	*					m_inRoad;							// �������뻬�е�
	Road	*					m_outRoad;						// �����Ƴ����е�

public:// ͣ��λ����
	static map<string, GatePosition *>		s_mapSetOfGatePosition;	// ͣ��λ��map���ϣ�keyΪͣ��λ���룬valueΪͣ��λ
	static int							s_gatePositionCount;		// ͣ��λ����

#pragma endregion

#pragma region Functions

public:

#pragma region ��������
	//////////////////////////////////////////////////////////////////////////

	//
	// ժҪ:
	//     ���캯����
	GatePosition(string summary, string gatePositionNo, string internationalOrDomestic, string distance, string inRoadNo, string outRoadNo, vector<string> flyingCommission, vector<string> companyNo, vector<string> model, vector<string> conflictGatePosition, vector<string> influenceGatePositionIn, vector<string> influenceGatePositionOut);
	//																											
	// ժҪ��																									 
	//     ��ȡ���ݡ�
	static void readData();
	//																											
	// ժҪ��																									 
	//     ��ʼ��(����Ӱ����)��
	static void initBeConflict();
	//																											
	// ժҪ��																									 
	//     �ԣ��ָ��ַ�������stringת��Ϊvector<string>��
	static vector<string> stringToVector(string sourceStr);
	//																											
	// ժҪ��																									 
	//     ��û�λ����ͣ�ŵ����к��ࡣ
	static void GetFeasibleFlight();
	//																											
	// ժҪ��																									 
	//     ��ӡ��λ����ͣ�ŵ����к��ࡣ
	static void PrintFeasibleFlight();
	//																											
	// ժҪ��																									 
	//     ��ӡ��λ�������������Ϊ1��
	static void PrintDistance();
	//																											
	// ժҪ��																									 
	//     ��ӡ��λ�Ƿ�Ϊ��ʱ��ʩ������ʱ��ʩΪ1��������ʱ��ʩΪ0��
	static void PrintTemporaryMeasures();
	//																											
	// ժҪ��																									 
	//     ��ӡ����ͬʱʹ�õĻ�λ(M*M)��������ͬʱʹ����Ϊ1
	static void PrintConflictGatePosition();
	//																											
	// ժҪ��																									 
	//     ��ӡ��Ӱ�컬��Ļ�λ(M*M)����Ӱ�컬���Ļ�λ(M*M)����Ӱ��Ϊ1
	static void PrintInfluenceGatePosition();
	//																											
	// ժҪ��																									 
	//     ��������
	static void clearResult();

	////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma endregion

};

class Road
{

#pragma region Variables

public:// ���е�����
	string					m_roadNo;							// ���е�����
	vector<string>			m_gatePositionOfRoadNo;			// ���е�����ͣ��λ

public:// ���е�ʱ�����
	vector<pair<time_t, time_t>>		m_vectorOfStartTimeToFinishTime;		// ���е����们��ʱ�̵ļ��ϣ�keyΪ���е����们�п�ʼʱ�̣�valueΪ���е����们�н���ʱ��
	vector<pair<time_t, Flight *>>		m_vectorOfStartTimeToFlight;			// ���е����们�еĺ���ļ��ϣ�keyΪ���е����们�п�ʼʱ�̣�valueΪ�����

public:// ���е�����
	static map<string, Road *>		s_mapSetOfRoad;				// ���е���map���ϣ�keyΪ���е����룬valueΪ���е�
	static int					s_roadCount;					// ���е�����

#pragma endregion

#pragma region Functions

public:

#pragma region ��������
	//////////////////////////////////////////////////////////////////////////

	//
	// ժҪ:
	//     ���캯����
	Road(string roadNo, string gatePositionOfRoadNo);
	//
	// ժҪ:
	//     ��ʼ�����е���
	static void initRoad();
	//
	// ժҪ:
	//     ��ӡͣ��λ�뻬����Ӧ��ϵ��
	static void printGatePositionToRoad();
	//																											
	// ժҪ��																									 
	//     ��������
	static void clearResult();

	////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma endregion

};