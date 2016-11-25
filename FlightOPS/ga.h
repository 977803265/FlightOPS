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

#pragma once

#include "basic.h"
#include "flight.h"

#pragma region �½�����
////////////////////////////////////////////////////////////////////////
class GeneticAlgorithm;// �Ŵ��㷨��
////////////////////////////////////////////////////////////////////////
#pragma endregion

class GeneticAlgorithm
{

#pragma region Variables

public:// �㷨����
	static int		PopSize;									// ��Ⱥ��С
	static int		Gen;										// ѭ������
	static double		Pc;										// �������
	static double		Pm;										// �������

	static double		Pc1or2;									// �����������
	static double		Pm1or2;									// �����������

public:// �㷨����
	vector<vector<pair<Flight *, vector<GatePosition*>>>>		individual;	// ��Ⱥ����1
	vector<vector<int>>									individual2;	// ��Ⱥ����2
	vector<double>										fitness;		// ��ȺĿ�꺯��ֵ
	vector<double>										prob;		// ���̶�ת��

	//int    individual[2 * PopSize][MaxNumOfCities];				// ��Ⱥ
	//int    fitness[2 * PopSize];								// ��ȺĿ�꺯��ֵ
	//double prob[PopSize];

#pragma endregion

#pragma region Functions

void BuildWheel();
void ComputeFitness();
void ComputeFitness2();
vector<pair<Flight *, vector<GatePosition*>>> GenerateRandomTour();
vector<int> GenerateRandomTour2();
int  SelectIndividual();
void PMX(int lo, int hi, vector<pair<Flight *, vector<GatePosition*>>> *tour1, vector<pair<Flight *, vector<GatePosition*>>> *tour2, vector<pair<Flight *, vector<GatePosition*>>> *newtour1, vector<pair<Flight *, vector<GatePosition*>>> *newtour2);
void PMX2(int lo, int hi, vector<int> *tour1, vector<int> *tour2, vector<int> *newtour1, vector<int> *newtour2);
void Mutation();
void Mutation2();

void Initialize();
void Initialize2();

void SelectionAndCrossover();
void SelectionAndCrossover2();

void CopySolution(vector<pair<Flight *, vector<GatePosition*>>> *from, vector<pair<Flight *, vector<GatePosition*>>> *to);
void CopySolution2(vector<int> *from, vector<int> *to);

int FindFlight(int flight, vector<int> *tour);
void SwapOperator(int city1, int city2, vector<int> *tour);

void GeneticAlgorithmMain();

#pragma endregion

};