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

#include "flight.h"

#pragma region ��������
//////////////////////////////////////////////////////////////////////////

Flight::Flight(string	companyNo, string flightNo, string inTime, string outTime, string internationalOrDomestic, string flyingCommission, string model, int number)
{
	m_companyNo=companyNo;
	m_flightNo=flightNo;
	m_inTime=inTime;
	m_inTime_t = Time::StringToDatetime(inTime);
	m_outTime=outTime;
	m_outTime_t = Time::StringToDatetime(outTime);
	m_internationalOrDomestic=internationalOrDomestic;
	m_flyingCommission=flyingCommission;
	m_model=model;
	if (flyingCommission == "I")
		m_number = 0;
	else
		m_number=number;
	if (internationalOrDomestic == "����")
		m_internationalOrDomesticInt = 1;
	else if (internationalOrDomestic == "����" || internationalOrDomestic == "���")
		m_internationalOrDomesticInt = 2;
	possibleGatePosition = map<string, GatePosition*>();
	gatePosition = NULL;
	// �򺽿չ�˾���Ϻͷ��мƻ������������δ���ڵ�ֵ
	if (find(s_vectorOfCompanyNo.begin(), s_vectorOfCompanyNo.end(), companyNo) == s_vectorOfCompanyNo.end())
		s_vectorOfCompanyNo.push_back(companyNo);
	if (find(s_vectorOfFlyingCommission.begin(), s_vectorOfFlyingCommission.end(), flyingCommission) == s_vectorOfFlyingCommission.end())
		s_vectorOfFlyingCommission.push_back(flyingCommission);
}

void Flight::readData()
{
	ifstream file("��ʷ����.csv");
	while (file.good())
	{
		//����洢����
		string value1;
		string value2;
		string value3;
		string value4;
		string value5;
		string value6;
		string value7;
		string value8;
		//��ȡֵ
		getline(file, value1, ',');
		getline(file, value2, ',');
		getline(file, value3, ',');
		getline(file, value4, ',');
		getline(file, value5, ',');
		getline(file, value6, ',');
		getline(file, value7, ',');
		getline(file, value8);
		if (value1 == "" || value1 == "���չ�˾")
			continue;
		if (value8 == "")
			value8 = "0";
		//ת�����洢
		Flight *newFlight = new Flight(value1, value2, value3, value4, value5, value6, value7, atoi(value8.c_str()));
		if (newFlight->m_inTime_t >= newFlight->m_outTime_t)
		{
			s_mapSetOfwrongFlight.insert(make_pair(value2, newFlight));
			s_wrongFlightCount++;
		}
		else
		{
			s_mapSetOfFlight.insert(make_pair(value2, newFlight));
			s_flightCount++;
		}
	}
	cout << "��ȡ����������ɣ�����ʱ�̴��ڳ���ʱ�̵ĺ�������Ϊ " << s_wrongFlightCount << " �ܣ��ڱ��㷨�в��迼�ǡ�" << endl;
}

void Flight::initNumberCount()
{
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		Flight::s_nemberCount = Flight::s_nemberCount + flight->m_number;
	}
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfwrongFlight.begin(); iter != Flight::s_mapSetOfwrongFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		Flight::s_nemberCount = Flight::s_nemberCount + flight->m_number;
	}
}

void Flight::printTime()
{
	int min = Flight::s_mapSetOfFlight.begin()->second->m_inTime_t - 60 * 5;
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		int inTime = flight->m_inTime_t - 60 * 5;
		int outTime = flight->m_outTime_t + 60 * 5;
		if (inTime < min)
			min = inTime;
		if (outTime < min)
			min = outTime;
	}
	string result = "flightTime.txt";
	ofstream fout(result);
	//cout << "[";
	// ��˳������������ʱ�� - 5min
	fout << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end();)
	{
		Flight *flight = iter->second;
		int inTime = (flight->m_inTime_t - 60 * 5 - min) / 10;
		//cout << inTime;
		fout << inTime;
		iter++;
		if (iter != Flight::s_mapSetOfFlight.end())
		{
			//cout << ",";
			fout << ",";
		}
	}
	//cout << "]" << endl << "[";
	// ��˳������������ʱ�� + 5min
	fout << "]" << endl << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end();)
	{
		Flight *flight = iter->second;
		int outTime = (flight->m_outTime_t + 60 * 5 - min) / 10;
		//cout << outTime;
		fout << outTime;
		iter++;
		if (iter != Flight::s_mapSetOfFlight.end())
		{
			//cout << ",";
			fout << ",";
		}
	}
	//cout << "]";
	fout << "]";
}

void Flight::printRoadTime()
{
	int min = Flight::s_mapSetOfFlight.begin()->second->m_inTime_t - 60 * 5;
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		int inTime = flight->m_inTime_t - 60 * 5;
		int outTime = flight->m_outTime_t + 60 * 5;
		if (inTime < min)
			min = inTime;
		if (outTime < min)
			min = outTime;
	}
	string result = "flightRoadTime.txt";
	ofstream fout(result);
	// ��˳������������ʱ���뻬��ʱ��
	fout << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end();)
	{
		Flight *flight = iter->second;
		int inTime = (flight->m_inTime_t - 60 * 5 - min) / 10;
		fout << inTime;
		iter++;
		if (iter != Flight::s_mapSetOfFlight.end())
		{
			fout << ",";
		}
	}
	// ��˳������������ʱ�뿪����ʱ��
	fout << "]" << endl << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end();)
	{
		Flight *flight = iter->second;
		int inTime = (flight->m_inTime_t - min) / 10;
		fout << inTime;
		iter++;
		if (iter != Flight::s_mapSetOfFlight.end())
		{
			fout << ",";
		}
	}
	// ��˳������������ʱ���뻬��ʱ��
	fout << "]" << endl << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end();)
	{
		Flight *flight = iter->second;
		int outTime = (flight->m_outTime_t - min) / 10;
		fout << outTime;
		iter++;
		if (iter != Flight::s_mapSetOfFlight.end())
		{
			fout << ",";
		}
	}
	// ��˳������������ʱ�뿪����ʱ��
	fout << "]" << endl << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end();)
	{
		Flight *flight = iter->second;
		int outTime = (flight->m_outTime_t + 60 * 5 - min) / 10;
		fout << outTime;
		iter++;
		if (iter != Flight::s_mapSetOfFlight.end())
		{
			fout << ",";
		}
	}
	fout << "]";
}

void Flight::printConflictFlight()
{
	string result = "conflictFlight.txt";
	ofstream fout(result);
	// ����A�뺽��B���뻬����ͻ
	int i = 0;
	fout << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		time_t tin1 = flight->m_inTime_t;
		time_t tout1 = flight->m_outTime_t;
		int j = 0;
		fout << "[";
		for (map<string, Flight *>::iterator iter2 = Flight::s_mapSetOfFlight.begin(); iter2 != Flight::s_mapSetOfFlight.end(); iter2++)
		{
			Flight *flight2 = iter2->second;
			time_t tin2 = flight2->m_inTime_t;
			time_t tout2 = flight2->m_outTime_t;
			if (abs(tin1 - tin2) < 5 * 60)
				fout << 1;
			else
				fout << 0;
			j++;
			if (j == Flight::s_flightCount)
				fout << "]";
			else
				fout << ",";
		}
		i++;
		if (i == Flight::s_flightCount)
			fout << "]";
		else
			fout << ",";
	}
	fout << endl;
	// ����A�뺽��B�Ƴ�������ͻ
	i = 0;
	fout << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		time_t tin1 = flight->m_inTime_t;
		time_t tout1 = flight->m_outTime_t;
		int j = 0;
		fout << "[";
		for (map<string, Flight *>::iterator iter2 = Flight::s_mapSetOfFlight.begin(); iter2 != Flight::s_mapSetOfFlight.end(); iter2++)
		{
			Flight *flight2 = iter2->second;
			time_t tin2 = flight2->m_inTime_t;
			time_t tout2 = flight2->m_outTime_t;
			if (abs(tout1 - tout2) < 5 * 60)
				fout << 1;
			else
				fout << 0;
			j++;
			if (j == Flight::s_flightCount)
				fout << "]";
			else
				fout << ",";
		}
		i++;
		if (i == Flight::s_flightCount)
			fout << "]";
		else
			fout << ",";
	}
	// ����A���뻬���뺽��B�Ƴ�������ͻ
	i = 0;
	fout << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		time_t tin1 = flight->m_inTime_t;
		time_t tout1 = flight->m_outTime_t;
		int j = 0;
		fout << "[";
		for (map<string, Flight *>::iterator iter2 = Flight::s_mapSetOfFlight.begin(); iter2 != Flight::s_mapSetOfFlight.end(); iter2++)
		{
			Flight *flight2 = iter2->second;
			time_t tin2 = flight2->m_inTime_t;
			time_t tout2 = flight2->m_outTime_t;
			if (tin1 - tout2 < 10 * 60 && tin1 - tout2 > 0)
				fout << 1;
			else
				fout << 0;
			j++;
			if (j == Flight::s_flightCount)
				fout << "]";
			else
				fout << ",";
		}
		i++;
		if (i == Flight::s_flightCount)
			fout << "]";
		else
			fout << ",";
	}
	fout << endl;
	// ����A�Ƴ������뺽��B���뻬����ͻ
	i = 0;
	fout << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		time_t tin1 = flight->m_inTime_t;
		time_t tout1 = flight->m_outTime_t;
		int j = 0;
		fout << "[";
		for (map<string, Flight *>::iterator iter2 = Flight::s_mapSetOfFlight.begin(); iter2 != Flight::s_mapSetOfFlight.end(); iter2++)
		{
			Flight *flight2 = iter2->second;
			time_t tin2 = flight2->m_inTime_t;
			time_t tout2 = flight2->m_outTime_t;
			if (tin2 - tout1 < 10 * 60 && tin2 - tout1 > 0)
				fout << 1;
			else
				fout << 0;
			j++;
			if (j == Flight::s_flightCount)
				fout << "]";
			else
				fout << ",";
		}
		i++;
		if (i == Flight::s_flightCount)
			fout << "]";
		else
			fout << ",";
	}
}

void Flight::printNumberOfFlight()
{
	string result = "numberOfFlight.txt";
	ofstream fout(result);
	fout << "[";
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end();)
	{
		Flight *flight = iter->second;
		int number = flight->m_number;
		fout << number;
		iter++;
		if (iter != Flight::s_mapSetOfFlight.end())
		{
			fout << ",";
		}
	}
	fout << "]" << endl;
}

void Flight::printResult()
{
	string result = "result.csv";
	ofstream fout(result);
	fout << "���ۺ����,����ʱ��,����ʱ��,ͣ��λ" << endl;
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		if (flight->gatePosition != NULL)
			fout << flight->m_flightNo + "," + flight->m_inTime << "," << flight->m_outTime << "," << flight->gatePosition->m_gatePositionNo << endl;
		else
			fout << flight->m_flightNo + "," + flight->m_inTime << "," << flight->m_outTime << "," << "δ����" << endl;
	}
}

void Flight::clearResult()
{
	for (map<string, Flight *>::iterator iter = s_mapSetOfFlight.begin(); iter != s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		flight->gatePosition = NULL;
	}
}

void Flight::clearOneFlight(Flight *flight)
{
	time_t inTime = flight->m_inTime_t;
	time_t outTime = flight->m_outTime_t;
	GatePosition *gatePosition = flight->gatePosition;
	Road *inRoad = gatePosition->m_inRoad;
	Road *outRoad = gatePosition->m_outRoad;
	int deleteInIter = find(inRoad->m_vectorOfStartTimeToFlight.begin(), inRoad->m_vectorOfStartTimeToFlight.end(), make_pair(inTime - 5 * 60, flight)) - inRoad->m_vectorOfStartTimeToFlight.begin();
	inRoad->m_vectorOfStartTimeToFinishTime.erase(inRoad->m_vectorOfStartTimeToFinishTime.begin() + deleteInIter);
	inRoad->m_vectorOfStartTimeToFlight.erase(remove(inRoad->m_vectorOfStartTimeToFlight.begin(), inRoad->m_vectorOfStartTimeToFlight.end(), make_pair(inTime - 5 * 60, flight)), inRoad->m_vectorOfStartTimeToFlight.end());
	int deleteOutIter = find(outRoad->m_vectorOfStartTimeToFlight.begin(), outRoad->m_vectorOfStartTimeToFlight.end(), make_pair(outTime, flight)) - outRoad->m_vectorOfStartTimeToFlight.begin();
	outRoad->m_vectorOfStartTimeToFinishTime.erase(outRoad->m_vectorOfStartTimeToFinishTime.begin() + deleteOutIter);
	outRoad->m_vectorOfStartTimeToFlight.erase(remove(outRoad->m_vectorOfStartTimeToFlight.begin(), outRoad->m_vectorOfStartTimeToFlight.end(), make_pair(outTime, flight)), outRoad->m_vectorOfStartTimeToFlight.end());
	gatePosition->m_mapOfStartTimeToFinishTime.erase(inTime);
	gatePosition->m_mapOfStartTimeToFlight.erase(inTime);
	flight->gatePosition = NULL;
}

void Flight::GetFeasibleGatePosition()
{
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		string companyNo = flight->m_companyNo;
		int internationalOrDomesticInt = flight->m_internationalOrDomesticInt;
		string flyingCommission = flight->m_flyingCommission;
		string model = flight->m_model;
		for (map<string, GatePosition *>::iterator iter2 = GatePosition::s_mapSetOfGatePosition.begin(); iter2 != GatePosition::s_mapSetOfGatePosition.end(); iter2++)
		{
			string gatePositionNo = iter2->first;
			GatePosition *gatePosition = iter2->second;
			vector<string> &companyNo2 = gatePosition->m_companyNo;
			int internationalOrDomesticInt2 = gatePosition->m_internationalOrDomesticInt;
			vector<string> &flyingCommission2 = gatePosition->m_flyingCommission;
			vector<string> &model2 = gatePosition->m_model;
			if (find(companyNo2.begin(), companyNo2.end(), companyNo) == companyNo2.end())
				continue;
			if (internationalOrDomesticInt2 != 3 && internationalOrDomesticInt2 != internationalOrDomesticInt)
				continue;
			if (find(flyingCommission2.begin(), flyingCommission2.end(), flyingCommission) == flyingCommission2.end())
				continue;
			if (find(model2.begin(), model2.end(), model) == model2.end())
				continue;
			flight->possibleGatePosition.insert(make_pair(gatePositionNo, gatePosition));
		}
	}
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		vector<GatePosition*> gatePositions;
		for (map<string, GatePosition*>::iterator iter2 = flight->possibleGatePosition.begin(); iter2 != flight->possibleGatePosition.end(); iter2++)
		{
			GatePosition* gatePosition = iter2->second;
			gatePositions.push_back(gatePosition);
		}
		Flight::initPossibleGatePosition.push_back(make_pair(flight, gatePositions));
	}
}

//////////////////////////////////////////////////////////////////////////
#pragma endregion

#pragma region ��ͬ���Ϻ���
//////////////////////////////////////////////////////////////////////////
map<string, Flight *> Flight::s_mapSetOfFlight = map<string, Flight *>();
map<string, Flight *> Flight::s_mapSetOfwrongFlight = map<string, Flight *>();
int Flight::s_flightCount = 0;
int Flight::s_wrongFlightCount = 0;
int Flight::s_nemberCount = 0;
vector<pair<Flight *, vector<GatePosition*>>>	Flight::initPossibleGatePosition = vector<pair<Flight *, vector<GatePosition*>>>();
vector<string> Flight::s_vectorOfCompanyNo = vector<string>();
vector<string> Flight::s_vectorOfFlyingCommission = vector<string>();
//////////////////////////////////////////////////////////////////////////
#pragma endregion