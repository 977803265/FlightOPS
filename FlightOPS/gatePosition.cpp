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

#include "gatePosition.h"

#pragma region ��������
//////////////////////////////////////////////////////////////////////////

GatePosition::GatePosition(string summary, string gatePositionNo, string internationalOrDomestic, string distance, string inRoadNo, string outRoadNo, vector<string> flyingCommission, vector<string> companyNo, vector<string> model, vector<string> conflictGatePosition, vector<string> influenceGatePositionIn, vector<string> influenceGatePositionOut)
{
	m_summary = summary;
	//if (summary == "�������ӹ����ƺ" || summary == "��Զ��ƺ" || summary == "��ʱ��λ")
	//	temporaryMeasures = 1;
	if (internationalOrDomestic == "")
		temporaryMeasures = 1;
	else
		temporaryMeasures = 0;
	m_gatePositionNo = gatePositionNo;
	m_internationalOrDomestic = internationalOrDomestic;
	m_distance = distance;
	m_inRoadNo = inRoadNo;
	m_outRoadNo = outRoadNo;
	if (flyingCommission.empty())
		m_flyingCommission.insert(m_flyingCommission.end(), Flight::s_vectorOfFlyingCommission.begin(), Flight::s_vectorOfFlyingCommission.end());
	else
		m_flyingCommission = flyingCommission;
	if (companyNo.empty())
		m_companyNo.insert(m_companyNo.end(), Flight::s_vectorOfCompanyNo.begin(), Flight::s_vectorOfCompanyNo.end());
	else
		m_companyNo = companyNo;
	m_model = model;
	if (internationalOrDomestic == "����")
		m_internationalOrDomesticInt = 1;
	else if (internationalOrDomestic == "����")
		m_internationalOrDomesticInt = 2;
	else
		m_internationalOrDomesticInt = 3;
	m_mapOfStartTimeToFinishTime = map<time_t, time_t>();
	m_mapOfStartTimeToFlight = map<time_t, Flight *>();
	m_conflictGatePosition = conflictGatePosition;
	m_influenceGatePositionIn = influenceGatePositionIn;
	m_influenceGatePositionOut = influenceGatePositionOut;
}

void GatePosition::readData()
{
	ifstream file("��λ���Ա�.csv");
	while (file.good())
	{
		//����洢����
		string value0;	// ����
		string value1;	// ͣ��λ
		string value2;	// ���ʹ�������
		string value3;	// Զ����λ����
		string value4;	// ���뻬�е�
		string value5;	// �Ƴ����е�
		string value6;	// ��������
		string value7;	// ���չ�˾
		string value8;	// ����
		string value9;	// ��ͻ��λ
		string value10;	// Ӱ�컬���λ
		string value11;	// Ӱ�컬����λ
		//��ȡֵ
		getline(file, value0, ',');
		getline(file, value1, ',');
		getline(file, value2, ',');
		getline(file, value3, ',');
		getline(file, value4, ',');
		getline(file, value5, ',');
		getline(file, value6, ',');
		getline(file, value7, ',');
		getline(file, value8, ',');
		getline(file, value9, ',');
		getline(file, value10, ',');
		getline(file, value11);
		if (value1 == "" || value1 == "ͣ��λ")
			continue;
		//ת�����洢
		vector<string> flyingCommission = stringToVector(value6);
		vector<string> companyNo = stringToVector(value7);
		vector<string> model = stringToVector(value8);
		vector<string> conflictGatePosition = stringToVector(value9);
		vector<string> influenceGatePositionIn = stringToVector(value10);
		vector<string> influenceGatePositionOut = stringToVector(value11);
		GatePosition *newGatePosition = new GatePosition(value0, value1, value2, value3, value4, value5, flyingCommission, companyNo, model, conflictGatePosition, influenceGatePositionIn, influenceGatePositionOut);
		if (s_mapSetOfGatePosition.find(value1) == s_mapSetOfGatePosition.end())
		{
			s_mapSetOfGatePosition.insert(make_pair(value1, newGatePosition));
			s_gatePositionCount++;
		}
		else
			cout << endl << "ͣ��λ�����ظ�" << endl;
	}
}

void GatePosition::initBeConflict()
{
	for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
	{
		string gatePositionNo1 = iter->first;
		GatePosition *gatePosition1 = iter->second;
		vector<string> &conflictGatePosition = gatePosition1->m_conflictGatePosition;
		vector<string> &influenceGatePositionIn = gatePosition1->m_influenceGatePositionIn;
		vector<string> &influenceGatePositionOut = gatePosition1->m_influenceGatePositionOut;
		for (map<string, GatePosition *>::iterator iter2 = GatePosition::s_mapSetOfGatePosition.begin(); iter2 != GatePosition::s_mapSetOfGatePosition.end(); iter2++)
		{
			string gatePositionNo2 = iter2->first;
			GatePosition *gatePosition2 = iter2->second;
			if (find(conflictGatePosition.begin(), conflictGatePosition.end(), gatePositionNo2) != conflictGatePosition.end())
				gatePosition2->m_beConflictGatePosition.push_back(gatePosition1);
			if (find(influenceGatePositionIn.begin(), influenceGatePositionIn.end(), gatePositionNo2) != influenceGatePositionIn.end())
				gatePosition2->m_beInfluenceGatePositionIn.push_back(gatePosition1);
			if (find(influenceGatePositionOut.begin(), influenceGatePositionOut.end(), gatePositionNo2) != influenceGatePositionOut.end())
				gatePosition2->m_beInfluenceGatePositionOut.push_back(gatePosition1);
		}
	}
}

vector<string> GatePosition::stringToVector(string sourceStr)
{
	vector<string> result;

	string subStr = "";
	int strLen = sourceStr.size();
	for (int i = 0; i < strLen; i++)
	{
		char theChar = sourceStr[i];

		if (theChar == '|')// �����ָ���
		{
			if (subStr != "")
			{
				result.push_back(subStr);
				subStr = "";
			}
		}
		else// û�������ָ���
		{
			subStr += theChar;
		}
	}
	if (subStr != "")// �������һ���ַ���
		result.push_back(subStr);

	return result;
}

void GatePosition::GetFeasibleFlight()
{
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		string flightNo = iter->first;
		Flight *flight = iter->second;
		for (map<string, GatePosition*>::iterator iter2 = flight->possibleGatePosition.begin(); iter2 != flight->possibleGatePosition.end();iter2++)
		{
			GatePosition* gatePosition = iter2->second;
			gatePosition->possibleFlight.insert(make_pair(flightNo, flight));
		}
	}
}

void GatePosition::PrintFeasibleFlight()
{
	string result = "feasibleFlight.txt";
	ofstream fout(result);
	int i = 0;
	fout << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		string flightNo = iter->first;
		Flight *flight = iter->second;
		int j = 0;
		fout << "[";
		for (map<string, GatePosition *>::iterator iter2 = GatePosition::s_mapSetOfGatePosition.begin(); iter2 != GatePosition::s_mapSetOfGatePosition.end(); iter2++)
		{
			string gatePositionNo = iter2->first;
			GatePosition* gatePosition = iter2->second;
			if (flight->possibleGatePosition.find(gatePositionNo) == flight->possibleGatePosition.end())
				fout << 0;
			else 
				fout << 1; 
			j++;
			if (j != GatePosition::s_gatePositionCount)
				fout << ",";
			else
				fout << "]";
		}
		i++;
		if (i != Flight::s_flightCount)
			fout << ",";
		else
			fout << "]";
	}
}

void GatePosition::PrintDistance()
{
	string result = "distance.txt";
	ofstream fout(result);
	fout << "[";
	int i = 0;
	for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
	{
		GatePosition *gatePosition = iter->second;
		if (gatePosition->m_distance == "����λ")
			fout << 1;
		else
			fout << 0;
		i++;
		if (i == GatePosition::s_gatePositionCount)
			fout << "]";
		else
			fout << ",";
	}
}

void GatePosition::PrintTemporaryMeasures()
{
	string result = "temporaryMeasures.txt";
	ofstream fout(result);
	fout << "[";
	int i = 0;
	for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
	{
		GatePosition *gatePosition = iter->second;
		fout << gatePosition->temporaryMeasures;
		i++;
		if (i == GatePosition::s_gatePositionCount)
			fout << "]";
		else
			fout << ",";
	}
}

void GatePosition::PrintConflictGatePosition()
{
	string result = "conflictGatePosition.txt";
	ofstream fout(result);
	fout << "[";
	int i = 0;
	for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
	{
		GatePosition *gatePosition1 = iter->second;
		fout << "[";
		int j = 0;
		for (map<string, GatePosition *>::iterator iter2 = GatePosition::s_mapSetOfGatePosition.begin(); iter2 != GatePosition::s_mapSetOfGatePosition.end(); iter2++)
		{
			GatePosition *gatePosition2 = iter2->second;
			if (find(gatePosition1->m_conflictGatePosition.begin(), gatePosition1->m_conflictGatePosition.end(), gatePosition2->m_gatePositionNo) == gatePosition1->m_conflictGatePosition.end())
				fout << 0;
			else
				fout << 1;
			j++;
			if (j == GatePosition::s_gatePositionCount)
				fout << "]";
			else
				fout << ",";
		}
		i++;
		if (i == GatePosition::s_gatePositionCount)
			fout << "]";
		else
			fout << ",";
	}
}

void GatePosition::PrintInfluenceGatePosition()
{
	string result = "influenceGatePosition.txt";
	ofstream fout(result);
	// ��ӡ��Ӱ�컬��Ļ�λ
	fout << "[";
	int i = 0;
	for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
	{
		GatePosition *gatePosition1 = iter->second;
		fout << "[";
		int j = 0;
		for (map<string, GatePosition *>::iterator iter2 = GatePosition::s_mapSetOfGatePosition.begin(); iter2 != GatePosition::s_mapSetOfGatePosition.end(); iter2++)
		{
			GatePosition *gatePosition2 = iter2->second;
			if (find(gatePosition1->m_influenceGatePositionIn.begin(), gatePosition1->m_influenceGatePositionIn.end(), gatePosition2->m_gatePositionNo) == gatePosition1->m_influenceGatePositionIn.end())
				fout << 0;
			else
				fout << 1;
			j++;
			if (j == GatePosition::s_gatePositionCount)
				fout << "]";
			else
				fout << ",";
		}
		i++;
		if (i == GatePosition::s_gatePositionCount)
			fout << "]";
		else
			fout << ",";
	}
	// ��ӡ��Ӱ�컬���Ļ�λ
	fout << endl;
	fout << "[";
	i = 0;
	for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
	{
		GatePosition *gatePosition1 = iter->second;
		fout << "[";
		int j = 0;
		for (map<string, GatePosition *>::iterator iter2 = GatePosition::s_mapSetOfGatePosition.begin(); iter2 != GatePosition::s_mapSetOfGatePosition.end(); iter2++)
		{
			GatePosition *gatePosition2 = iter2->second;
			if (find(gatePosition1->m_influenceGatePositionOut.begin(), gatePosition1->m_influenceGatePositionOut.end(), gatePosition2->m_gatePositionNo) == gatePosition1->m_influenceGatePositionOut.end())
				fout << 0;
			else
				fout << 1;
			j++;
			if (j == GatePosition::s_gatePositionCount)
				fout << "]";
			else
				fout << ",";
		}
		i++;
		if (i == GatePosition::s_gatePositionCount)
			fout << "]";
		else
			fout << ",";
	}
}

void GatePosition::clearResult()
{
	for (map<string, GatePosition *>::iterator iter = s_mapSetOfGatePosition.begin(); iter != s_mapSetOfGatePosition.end(); iter++)
	{
		GatePosition *gatePosition = iter->second;
		gatePosition->m_mapOfStartTimeToFinishTime.clear();
		gatePosition->m_mapOfStartTimeToFlight.clear();
	}
}

//////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region ��ͬ���Ϻ���
//////////////////////////////////////////////////////////////////////////
map<string, GatePosition *> GatePosition::s_mapSetOfGatePosition = map<string, GatePosition *>();
int GatePosition::s_gatePositionCount = 0;
//////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region ���е��ศ������
//////////////////////////////////////////////////////////////////////////

Road::Road(string roadNo, string gatePositionOfRoadNo)
{
	m_roadNo = roadNo;
	m_gatePositionOfRoadNo.push_back(gatePositionOfRoadNo);
	m_vectorOfStartTimeToFinishTime = vector<pair<time_t, time_t>>();
	m_vectorOfStartTimeToFlight = vector<pair<time_t, Flight *>>();
}

void Road::initRoad()
{
	for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
	{
		string gatePoditionNo = iter->first;
		GatePosition *gatePosition = iter->second;
		string inRoadNo = gatePosition->m_inRoadNo;
		string outRoadNo = gatePosition->m_outRoadNo;
		map<string, Road *>::iterator iter2 = s_mapSetOfRoad.find(inRoadNo);
		if (iter2 == s_mapSetOfRoad.end())
		{
			Road *newInRoad = new Road(inRoadNo, gatePoditionNo);
			gatePosition->m_inRoad = newInRoad;
			s_mapSetOfRoad.insert(make_pair(inRoadNo, newInRoad));
			s_roadCount++;

		}
		else
		{
			Road *inRoad = iter2->second;
			gatePosition->m_inRoad = inRoad;
			inRoad->m_gatePositionOfRoadNo.push_back(gatePoditionNo);
		}
		if (inRoadNo != outRoadNo)
		{
			map<string, Road *>::iterator iter3 = s_mapSetOfRoad.find(outRoadNo);
			if (iter3 == s_mapSetOfRoad.end())
			{
				Road *newOutRoad = new Road(outRoadNo, gatePoditionNo);
				gatePosition->m_outRoad = newOutRoad;
				s_mapSetOfRoad.insert(make_pair(outRoadNo, newOutRoad));
				s_roadCount++;

			}
			else
			{
				Road *outRoad = iter3->second;
				gatePosition->m_outRoad = outRoad;
				outRoad->m_gatePositionOfRoadNo.push_back(gatePoditionNo);
			}
		}
		else
			gatePosition->m_outRoad = gatePosition->m_inRoad;
	}
}

void Road::printGatePositionToRoad()
{
	string result = "gatePositionToRoad.txt";
	ofstream fout(result);

	#pragma region ͣ��λ���뻬����Ӧ��ϵ
	//////////////////////////////////////////////////////////////////////////
	{
		fout << "[";
		int i = 0;
		for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
		{
			string gatePositionNo = iter->first;
			GatePosition *gatePosition = iter->second;
			fout << "[";
			int j = 0;
			for (map<string, Road *>::iterator iter2 = Road::s_mapSetOfRoad.begin(); iter2 != Road::s_mapSetOfRoad.end(); iter2++)
			{
				string roadNo = iter2->first;
				Road *road = iter2->second;
				if (gatePosition->m_inRoadNo == roadNo)
					fout << 1;
				else
					fout << 0;
				j++;
				if (j == Road::s_roadCount)
					fout << "]";
				else
					fout << ",";
			}
			i++;
			if (i == GatePosition::s_gatePositionCount)
				fout << "]";
			else
				fout << ",";
		}
	}
	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	fout << endl;

	#pragma region ͣ��λ�Ƴ�������Ӧ��ϵ
	//////////////////////////////////////////////////////////////////////////
	{
		fout << "[";
		int i = 0;
		for (map<string, GatePosition *>::iterator iter = GatePosition::s_mapSetOfGatePosition.begin(); iter != GatePosition::s_mapSetOfGatePosition.end(); iter++)
		{
			string gatePositionNo = iter->first;
			GatePosition *gatePosition = iter->second;
			fout << "[";
			int j = 0;
			for (map<string, Road *>::iterator iter2 = Road::s_mapSetOfRoad.begin(); iter2 != Road::s_mapSetOfRoad.end(); iter2++)
			{
				string roadNo = iter2->first;
				Road *road = iter2->second;
				if (gatePosition->m_outRoadNo == roadNo)
					fout << 1;
				else
					fout << 0;
				j++;
				if (j == Road::s_roadCount)
					fout << "]";
				else
					fout << ",";
			}
			i++;
			if (i == GatePosition::s_gatePositionCount)
				fout << "]";
			else
				fout << ",";
		}
	}
	////////////////////////////////////////////////////////////////////////
	#pragma endregion
}

void Road::clearResult()
{
	for (map<string, Road *>::iterator iter = s_mapSetOfRoad.begin(); iter != s_mapSetOfRoad.end(); iter++)
	{
		Road *road = iter->second;
		road->m_vectorOfStartTimeToFinishTime.clear();
		road->m_vectorOfStartTimeToFlight.clear();
	}
}

////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region ���е����Ϻ���
//////////////////////////////////////////////////////////////////////////
map<string, Road *> Road::s_mapSetOfRoad = map<string, Road *>();
int Road::s_roadCount = 0;
//////////////////////////////////////////////////////////////////////////
#pragma endregion