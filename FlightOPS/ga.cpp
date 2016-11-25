/*************************************************
��Ȩ: �μ�˶
����: �μ�˶
����: 2016-10-16
����: ����ͣ��λ��Դ�����Ż�_�Ŵ��㷨��
�޸���ʷ:

**************************************************/

/*<remark>=========================================================
/// <summary>
/// ����ͣ��λ��Դ�����Ż�_�Ŵ��㷨��
/// </summary>
===========================================================</remark>*/

#include "ga.h"
#include "random.h"
#include "bestSolution.h"

//�����������
vector<pair<Flight *, vector<GatePosition*>>> GeneticAlgorithm::GenerateRandomTour()
{
	vector<pair<Flight *, vector<GatePosition*>>> oneRandomTour;
	for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		Flight *flight = iter->second;
		vector<GatePosition*> possibleGatePositionVector(flight->possibleGatePosition.size(), NULL);
		vector<GatePosition*> possibleGatePositionVectorTemp;
		vector<int> possibleGatePositionCountNo = Random::GetRandomSequence(flight->possibleGatePosition.size());
		for (map<string, GatePosition*>::iterator iter2 = flight->possibleGatePosition.begin(); iter2 != flight->possibleGatePosition.end(); iter2++)
			possibleGatePositionVectorTemp.push_back(iter2->second);
		for (int i = 0; i < possibleGatePositionVectorTemp.size(); i++)
			possibleGatePositionVector[possibleGatePositionCountNo[i]] = possibleGatePositionVectorTemp[i];
		oneRandomTour.push_back(make_pair(flight, possibleGatePositionVector));
	}
	return oneRandomTour;
};

vector<int> GeneticAlgorithm::GenerateRandomTour2()
{
	vector<int> tour;
	int  numUnVisitedFlights = Flight::s_flightCount - 1;				//δ������к���ļ�����
	vector<int> unVisitedFlights;									//δ������к����б�		

	//δ����Ľ��еĺ����б��ʼ��
	for (int i = 0; i < Flight::s_flightCount; i++)
	{
		tour.push_back(0);
		unVisitedFlights.push_back(i);
	}

	for (int i = 0; i < Flight::s_flightCount; i++)						//���ܺ��������ѭ��
	{
		int index = Random::GetRandom(0, numUnVisitedFlights--);	//�������һ������
		tour[i] = unVisitedFlights[index];					//���ú�����ӵ���ǰ��ĵ�ǰλ��
		for (int j = index; j <= numUnVisitedFlights; j++)	//��δ����ĺ����б���ɾ���ú���
			unVisitedFlights[j] = unVisitedFlights[j + 1];
	}
	return tour;
}

//����Ŀ�꺯��ֵ
void GeneticAlgorithm::ComputeFitness()
{
	for (int i = 0; i < PopSize; i++)
	{
		GatePosition::clearResult();
		Road::clearResult();
		Flight::clearResult();
		for (int j = 0; j < individual[i].size(); j++)
		{
			Flight *flight = individual[i][j].first;
			time_t flightInTime_t = flight->m_inTime_t;		// ����λʱ�䣬ʱ���ʽ
			time_t flightOutTime_t = flight->m_outTime_t;		// ����λʱ�䣬ʱ���ʽ
			vector<GatePosition*> &possibleGatePositionVector = individual[i][j].second;
			for (vector<GatePosition*>::iterator iter2 = possibleGatePositionVector.begin(); iter2 != possibleGatePositionVector.end(); iter2++)
			{
				GatePosition *gatePosition = *iter2;
				if (gatePosition->m_mapOfStartTimeToFinishTime.find(flightInTime_t) != gatePosition->m_mapOfStartTimeToFinishTime.end())
					continue;
				gatePosition->m_mapOfStartTimeToFinishTime.insert(make_pair(flightInTime_t, flightOutTime_t));
				map<time_t, time_t>::iterator preiter3 = --gatePosition->m_mapOfStartTimeToFinishTime.find(flightInTime_t);
				map<time_t, time_t>::iterator iter3 = gatePosition->m_mapOfStartTimeToFinishTime.find(flightInTime_t);
				map<time_t, time_t>::iterator nextiter3 = ++gatePosition->m_mapOfStartTimeToFinishTime.find(flightInTime_t);
				if (preiter3 != --gatePosition->m_mapOfStartTimeToFinishTime.begin() && flightInTime_t < preiter3->second + 8 * 60)
				{
					gatePosition->m_mapOfStartTimeToFinishTime.erase(iter3);
					continue;
				}
				if (nextiter3 != gatePosition->m_mapOfStartTimeToFinishTime.end() && flightOutTime_t > nextiter3->first - 8 * 60)
				{
					gatePosition->m_mapOfStartTimeToFinishTime.erase(iter3);
					continue;
				}
				gatePosition->m_mapOfStartTimeToFlight.insert(make_pair(flightInTime_t, flight));
				Road *road = gatePosition->m_road;
				road->m_vectorOfStartTimeToFinishTime.push_back(make_pair(flightInTime_t - 5 * 60, flightInTime_t));
				road->m_vectorOfStartTimeToFinishTime.push_back(make_pair(flightOutTime_t, flightOutTime_t + 5 * 60));
				road->m_vectorOfStartTimeToFlight.push_back(make_pair(flightInTime_t - 5 * 60, flight));
				road->m_vectorOfStartTimeToFlight.push_back(make_pair(flightOutTime_t, flight));
				flight->gatePosition = gatePosition;
				break;
			}
		}
		fitness[i] = BestSolution::getObjectiveFunction();
		if (fitness[i] > BestSolution::m_objValue)
		{
			cout << "����Ŀ�꺯��ֵ�Ѹ��£�" << BestSolution::m_objValue << " -> " << fitness[i] << endl;
			BestSolution::m_objValue = fitness[i];
			BestSolution::m_flightNo.clear();
			BestSolution::m_inTime.clear();
			BestSolution::m_outTime.clear();
			BestSolution::m_gatePositionNo.clear();
			for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
			{
				Flight *flight = iter->second;
				if (flight->gatePosition != NULL)
				{
					BestSolution::m_flightNo.push_back(flight->m_flightNo);
					BestSolution::m_inTime.push_back(flight->m_inTime);
					BestSolution::m_outTime.push_back(flight->m_outTime);
					BestSolution::m_gatePositionNo.push_back(flight->gatePosition->m_gatePositionNo);
				}
				//else
				//{
				//	BestSolution::m_flightNo.push_back(flight->m_flightNo);
				//	BestSolution::m_inTime.push_back(flight->m_inTime);
				//	BestSolution::m_outTime.push_back(flight->m_outTime);
				//	BestSolution::m_gatePositionNo.push_back("δ����");
				//}
			}
		}
	}
};

//����Ŀ�꺯��ֵ2
void GeneticAlgorithm::ComputeFitness2()
{
	for (int i = 0; i < PopSize; i++)
	{
		GatePosition::clearResult();
		Road::clearResult();
		Flight::clearResult();
		for (int k = 0; k < individual[i].size(); k++)
		{
			int j = individual2[i][k];
			Flight *flight = individual[i][j].first;
			time_t flightInTime_t = flight->m_inTime_t;		// ����λʱ�䣬ʱ���ʽ
			time_t flightOutTime_t = flight->m_outTime_t;		// ����λʱ�䣬ʱ���ʽ
			vector<GatePosition*> &possibleGatePositionVector = individual[i][j].second;
			for (vector<GatePosition*>::iterator iter2 = possibleGatePositionVector.begin(); iter2 != possibleGatePositionVector.end(); iter2++)
			{
				GatePosition *gatePosition = *iter2;
				if (gatePosition->m_mapOfStartTimeToFinishTime.find(flightInTime_t) != gatePosition->m_mapOfStartTimeToFinishTime.end())
					continue;
				gatePosition->m_mapOfStartTimeToFinishTime.insert(make_pair(flightInTime_t, flightOutTime_t));
				map<time_t, time_t>::iterator preiter3 = --gatePosition->m_mapOfStartTimeToFinishTime.find(flightInTime_t);
				map<time_t, time_t>::iterator iter3 = gatePosition->m_mapOfStartTimeToFinishTime.find(flightInTime_t);
				map<time_t, time_t>::iterator nextiter3 = ++gatePosition->m_mapOfStartTimeToFinishTime.find(flightInTime_t);
				if (preiter3 != --gatePosition->m_mapOfStartTimeToFinishTime.begin() && flightInTime_t < preiter3->second + 8 * 60)
				{
					gatePosition->m_mapOfStartTimeToFinishTime.erase(iter3);
					continue;
				}
				if (nextiter3 != gatePosition->m_mapOfStartTimeToFinishTime.end() && flightOutTime_t > nextiter3->first - 8 * 60)
				{
					gatePosition->m_mapOfStartTimeToFinishTime.erase(iter3);
					continue;
				}
				gatePosition->m_mapOfStartTimeToFlight.insert(make_pair(flightInTime_t, flight));
				Road *road = gatePosition->m_road;
				road->m_vectorOfStartTimeToFinishTime.push_back(make_pair(flightInTime_t - 5 * 60, flightInTime_t));
				road->m_vectorOfStartTimeToFinishTime.push_back(make_pair(flightOutTime_t, flightOutTime_t + 5 * 60));
				road->m_vectorOfStartTimeToFlight.push_back(make_pair(flightInTime_t - 5 * 60, flight));
				road->m_vectorOfStartTimeToFlight.push_back(make_pair(flightOutTime_t, flight));
				flight->gatePosition = gatePosition;
				break;
			}
		}
		fitness[i] = BestSolution::getObjectiveFunction();
		if (fitness[i] > BestSolution::m_objValue)
		{
			cout << "����Ŀ�꺯��ֵ�Ѹ��£�" << BestSolution::m_objValue << " -> " << fitness[i] << endl;
			BestSolution::m_objValue = fitness[i];
			BestSolution::m_flightNo.clear();
			BestSolution::m_inTime.clear();
			BestSolution::m_outTime.clear();
			BestSolution::m_gatePositionNo.clear();
			for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
			{
				Flight *flight = iter->second;
				if (flight->gatePosition != NULL)
				{
					BestSolution::m_flightNo.push_back(flight->m_flightNo);
					BestSolution::m_inTime.push_back(flight->m_inTime);
					BestSolution::m_outTime.push_back(flight->m_outTime);
					BestSolution::m_gatePositionNo.push_back(flight->gatePosition->m_gatePositionNo);
				}
				//else
				//{
				//	BestSolution::m_flightNo.push_back(flight->m_flightNo);
				//	BestSolution::m_inTime.push_back(flight->m_inTime);
				//	BestSolution::m_outTime.push_back(flight->m_outTime);
				//	BestSolution::m_gatePositionNo.push_back("δ����");
				//}
			}
		}
	}
};

//�������̶ĵ�����
void GeneticAlgorithm::BuildWheel()
{
	double sum = 0;
	for (int i = 0; i < PopSize; i++)						//������Ⱥÿ������Ŀ�꺯��ֵ֮��
	{
		sum += fitness[i];
		prob.push_back(0);
	}
	prob[0] = (double)fitness[0] / (double)sum;				//��һ������ռ�ܺ͵ķ���
	for( int i = 1 ; i < PopSize ; i++ )					//����ÿ������ռ�ܺ͵��ۼƷ���
		prob[i] = prob[i-1] + fitness[i] / sum;
};

//���̶ķ�ʽѡȡһ������
int  GeneticAlgorithm::SelectIndividual()
{
	double p = Random::GetRandom();
	if( p < prob[0] )
		return 0;
	if( p >= prob[PopSize - 1] )
		return (PopSize - 1);
	for( int i = 1 ; i < PopSize ; i++ )
		if( (p >= prob[i-1]) && (p < prob[i]) )
			return i;
	return -1;
};

//��lo��hi������
void GeneticAlgorithm::PMX(int lo, int hi, vector<pair<Flight *, vector<GatePosition*>>> *tour1, vector<pair<Flight *, vector<GatePosition*>>> *tour2, vector<pair<Flight *, vector<GatePosition*>>> *newtour1, vector<pair<Flight *, vector<GatePosition*>>> *newtour2)
{
	int j;
	CopySolution(tour1, newtour1);
	CopySolution(tour2, newtour2);

	j = lo;
	while( j <= hi )
	{
		pair<Flight *, vector<GatePosition*>> temp = (*newtour1)[j];
		(*newtour1)[j] = (*newtour2)[j];
		(*newtour2)[j] = temp;
		j++;
	}
};

//��lo��hi������2
void GeneticAlgorithm::PMX2(int lo, int hi, vector<int> *tour1, vector<int> *tour2, vector<int> *newtour1, vector<int> *newtour2)
{
	int j, pos1, pos2;
	CopySolution2(tour1, newtour1);
	CopySolution2(tour2, newtour2);

	j = lo;
	while (j <= hi)
	{
		pos1 = FindFlight((*tour2)[j], newtour1);
		pos2 = FindFlight((*tour1)[j], newtour2);
		SwapOperator(j, pos1, newtour1);
		SwapOperator(j, pos2, newtour2);
		j++;
	}
};

//����
void GeneticAlgorithm::Mutation()
{
	for(int i = 0; i < PopSize; i++)					//����Ⱥ��ÿ��������ѭ��
	{
		vector<pair<Flight *, vector<GatePosition*>>>::iterator iter3 = individual[i].begin();
		if( Random::GetRandom() < Pm )							//�������
		{
			int lo = Random::GetRandom(0, Flight::s_flightCount - 1);	//�����������Ϊ����ε���ʼ�����ֹ��
			int hi = Random::GetRandom(0, Flight::s_flightCount - 1);
			while (lo == hi)
				hi = Random::GetRandom(0, Flight::s_flightCount - 1);
			int min = lo < hi ? lo : hi;
			int max = lo < hi ? hi : lo;
			lo = min;
			hi = max;
			vector<pair<Flight *, vector<GatePosition*>>> oneRandomTour;
			int j = 0;
			for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
			{
				if (j >= lo && j <= hi)
				{
					Flight *flight = iter->second;
					vector<GatePosition*> possibleGatePositionVector(flight->possibleGatePosition.size(), NULL);
					vector<GatePosition*> possibleGatePositionVectorTemp;
					vector<int> possibleGatePositionCountNo = Random::GetRandomSequence(flight->possibleGatePosition.size());
					for (map<string, GatePosition*>::iterator iter2 = flight->possibleGatePosition.begin(); iter2 != flight->possibleGatePosition.end(); iter2++)
						possibleGatePositionVectorTemp.push_back(iter2->second);
					for (int k = 0; k < possibleGatePositionVectorTemp.size(); k++)
						possibleGatePositionVector[possibleGatePositionCountNo[k]] = possibleGatePositionVectorTemp[k];
					oneRandomTour.push_back(make_pair(flight, possibleGatePositionVector));
				}
				else
				{
					oneRandomTour.push_back(iter3[j]);
				}
				j++;
			}
			individual[i] = oneRandomTour;
		}
	}
};

//����2
void GeneticAlgorithm::Mutation2()
{
	for (int i = 0; i < PopSize; i++)					//����Ⱥ��ÿ��������ѭ��
	{
		if (Random::GetRandom() < Pm)							//�������
		{
			if (Random::GetRandom() < Pm1or2)		// ��1�ֱ��� or ��2�ֱ���
			{
				vector<pair<Flight *, vector<GatePosition*>>>::iterator iter3 = individual[i].begin();
				int lo = Random::GetRandom(0, Flight::s_flightCount - 1);	//�����������Ϊ����ε���ʼ�����ֹ��
				int hi = Random::GetRandom(0, Flight::s_flightCount - 1);
				while (lo == hi)
					hi = Random::GetRandom(0, Flight::s_flightCount - 1);
				int min = lo < hi ? lo : hi;
				int max = lo < hi ? hi : lo;
				lo = min;
				hi = max;
				vector<pair<Flight *, vector<GatePosition*>>> oneRandomTour;
				int j = 0;
				for (map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
				{
					if (j >= lo && j <= hi)
					{
						Flight *flight = iter->second;
						vector<GatePosition*> possibleGatePositionVector(flight->possibleGatePosition.size(), NULL);
						vector<GatePosition*> possibleGatePositionVectorTemp;
						vector<int> possibleGatePositionCountNo = Random::GetRandomSequence(flight->possibleGatePosition.size());
						for (map<string, GatePosition*>::iterator iter2 = flight->possibleGatePosition.begin(); iter2 != flight->possibleGatePosition.end(); iter2++)
							possibleGatePositionVectorTemp.push_back(iter2->second);
						for (int k = 0; k < possibleGatePositionVectorTemp.size(); k++)
							possibleGatePositionVector[possibleGatePositionCountNo[k]] = possibleGatePositionVectorTemp[k];
						oneRandomTour.push_back(make_pair(flight, possibleGatePositionVector));
					}
					else
					{
						oneRandomTour.push_back(iter3[j]);
					}
					j++;
				}
				individual[i] = oneRandomTour;
			}
			else
			{
				int i1 = Random::GetRandom(0, Flight::s_flightCount - 1);	//�������������
				int i2 = Random::GetRandom(0, Flight::s_flightCount - 1);
				while (i1 == i2)
					i2 = Random::GetRandom(0, Flight::s_flightCount - 1);

				SwapOperator(i1, i2, &individual2[i]);		//�Խ���������λ�õ�������
			}
		}
	}
};

//��ʼ��
void GeneticAlgorithm::Initialize()
{
	for (int i = 0; i < PopSize / 2; i++)
	{
		individual.push_back(Flight::initPossibleGatePosition);
		fitness.push_back(0);
	}
	for (int i = PopSize / 2; i < PopSize; i++)		//����ʼ��Ⱥ����PopSize������
	{
		individual.push_back(GenerateRandomTour());
		fitness.push_back(0);
	}
	for (int i = PopSize; i < 2 * PopSize; i++)		//����ʼ��Ⱥ����2 * PopSize������
	{
		individual.push_back(individual[i - PopSize]);
		fitness.push_back(0);
	}
};

//��ʼ��2
void GeneticAlgorithm::Initialize2()
{
	for (int i = 0; i < PopSize / 2; i++)
	{
		individual.push_back(Flight::initPossibleGatePosition);
		individual2.push_back(GenerateRandomTour2());
		fitness.push_back(0);
	}
	for (int i = PopSize / 2; i < PopSize; i++)		//����ʼ��Ⱥ����PopSize������
	{
		individual.push_back(GenerateRandomTour());
		individual2.push_back(GenerateRandomTour2());
		fitness.push_back(0);
	}
	for (int i = PopSize; i < 2 * PopSize; i++)		//����ʼ��Ⱥ����2 * PopSize������
	{
		individual.push_back(individual[i - PopSize]);
		individual2.push_back(individual2[i - PopSize]);
		fitness.push_back(0);
	}
};

//ѡ��ͽ���
void GeneticAlgorithm::SelectionAndCrossover()
{
	BuildWheel();										//�������̶ĵ�����
	for(int i = PopSize; i < 2 * PopSize; i += 2)		
	{
		int father = SelectIndividual();				//���̶ķ�ʽѡȡһ������
		int mother = SelectIndividual();				//���̶ķ�ʽѡȡ��һ������
		
		if (Random::GetRandom() < Pc)							//������㽻�����
		{
			int lo = Random::GetRandom(0, Flight::s_flightCount - 1);	//�����������Ϊ����ε���ʼ�����ֹ��
			int hi = Random::GetRandom(0, Flight::s_flightCount - 1);
			while( lo == hi)
				hi = Random::GetRandom(0, Flight::s_flightCount - 1);
			int min = lo < hi ? lo : hi;
			int max = lo < hi ? hi : lo;
			lo = min;
			hi = max;
			//��lo��hi��γ��ȶ�father��mother������Ȼ��浽indevidual��
			PMX(lo, hi, &individual[father], &individual[mother], &individual[i], &individual[i+1]);
		}
		else		//��������㽻�����
		{
			CopySolution(&individual[father], &individual[i]  );
			CopySolution(&individual[mother], &individual[i+1]);
		}
	}

	for(int i = 0; i < PopSize; i++)					//����������Ⱥ
		CopySolution(&individual[PopSize+i], &individual[i]);
}

//ѡ��ͽ���2
void GeneticAlgorithm::SelectionAndCrossover2()
{
	BuildWheel();										//�������̶ĵ�����
	for (int i = PopSize; i < 2 * PopSize; i += 2)
	{
		int father = SelectIndividual();				//���̶ķ�ʽѡȡһ������
		int mother = SelectIndividual();				//���̶ķ�ʽѡȡ��һ������

		if (Random::GetRandom() < Pc)							//������㽻�����
		{
			int lo = Random::GetRandom(0, Flight::s_flightCount - 1);	//�����������Ϊ����ε���ʼ�����ֹ��
			int hi = Random::GetRandom(0, Flight::s_flightCount - 1);
			while (lo == hi)
				hi = Random::GetRandom(0, Flight::s_flightCount - 1);
			int min = lo < hi ? lo : hi;
			int max = lo < hi ? hi : lo;
			lo = min;
			hi = max;
			//��lo��hi��γ��ȶ�father��mother������Ȼ��浽indevidual��
			if (Random::GetRandom() < Pc1or2)
				PMX(lo, hi, &individual[father], &individual[mother], &individual[i], &individual[i + 1]);
			else
				PMX2(lo, hi, &individual2[father], &individual2[mother], &individual2[i], &individual2[i + 1]);
		}
		else		//��������㽻�����
		{
			CopySolution(&individual[father], &individual[i]);
			CopySolution2(&individual2[father], &individual2[i]);
			CopySolution(&individual[mother], &individual[i + 1]);
			CopySolution2(&individual2[mother], &individual2[i + 1]);
		}
	}

	for (int i = 0; i < PopSize; i++)					//����������Ⱥ
	{
		CopySolution(&individual[PopSize + i], &individual[i]);
		CopySolution2(&individual2[PopSize + i], &individual2[i]);
	}
}

void GeneticAlgorithm::CopySolution(vector<pair<Flight *, vector<GatePosition*>>> *from, vector<pair<Flight *, vector<GatePosition*>>> *to)//����from���Ƶ�to��
{
	for (int i = 0; i < Flight::s_flightCount; i++)
		(*to)[i] = (*from)[i];
}

void GeneticAlgorithm::CopySolution2(vector<int> *from, vector<int> *to)//����from���Ƶ�to��
{
	for (int i = 0; i < Flight::s_flightCount; i++)
		(*to)[i] = (*from)[i];
}

int GeneticAlgorithm::FindFlight(int flight, vector<int> *tour)//���ݳ��б���ڽ���Ѱ�ҳ��е�λ��
{
	int j = 0;
	while ((*tour)[j] != flight)
		j++;
	return j;
};

void GeneticAlgorithm::SwapOperator(int city1, int city2, vector<int> *tour)//���򽻻�
{
	int temp = (*tour)[city1];
	(*tour)[city1] = (*tour)[city2];
	(*tour)[city2] = temp;
};

//�Ŵ��㷨������
void GeneticAlgorithm::GeneticAlgorithmMain()
{
	Initialize2();				//��ʼ��
	ComputeFitness2();				//�����ʼ�����õ���Ⱥ��Ŀ�꺯��ֵ

	for(int g = 1; g <= Gen; g++)	//ѭ���ܴ���
	{
		cout << "�Ŵ��㷨�� " << g << " �ε����С���" << endl;
		SelectionAndCrossover2();	//ѡ��ͽ���
		Mutation2();				//����
		ComputeFitness2();			//��������Ⱥ��Ӧÿ�������Ŀ�꺯��ֵ
	}
}

int		GeneticAlgorithm::PopSize = 80;
int		GeneticAlgorithm::Gen = 300;
double	GeneticAlgorithm::Pc = 0.5;
double	GeneticAlgorithm::Pm = 0.2;

double	GeneticAlgorithm::Pc1or2 = 0.5;
double	GeneticAlgorithm::Pm1or2 = 0.5;