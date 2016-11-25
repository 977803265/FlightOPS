#include "flight.h"
#include "gatePosition.h"
#include "bestSolution.h"

#pragma once

void solveByCplex();

int main()
{
	#pragma region ����ʹ��
	//////////////////////////////////////////////////////////////////////////
	{
		Flight::s_mapSetOfFlight;
		GatePosition::s_mapSetOfGatePosition;
		Road::s_mapSetOfRoad;
		BestSolution::m_solution;
	}
	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region ��ʼ������
	////////////////////////////////////////////////////////////////////////
	clock_t start, finish, time1, time2;
	double duration1, duration2, duration3, duration4, duration5, duration6, duration7;
	start = clock();							//��ʼ��ʱ(��)
	time1 = clock();
	Flight::readData();
	GatePosition::readData();
	Flight::initNumberCount();
	Road::initRoad();
	GatePosition::initBeConflict();
	time2 = clock();
	duration1 = (time2 - time1) / (double)1000;
	cout << "��ʼ��������ʱ : " << duration1 << " ��" << endl;	//��ʾ����ʱ��
	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	for (static map<string, Flight *>::iterator iter = Flight::s_mapSetOfFlight.begin(); iter != Flight::s_mapSetOfFlight.end(); iter++)
	{
		if (iter->second->m_inTime_t == -1 || iter->second->m_outTime_t == -1)
		{
			cout << "error";
		}
	}
	for (static map<string, Flight *>::iterator iter = Flight::s_mapSetOfwrongFlight.begin(); iter != Flight::s_mapSetOfwrongFlight.end(); iter++)
	{
		if (iter->second->m_inTime_t == -1 || iter->second->m_outTime_t == -1)
		{
			cout << "error";
		}
	}

	#pragma region ��ú������ʹ�õ����л�λ
	////////////////////////////////////////////////////////////////////////
	time1 = clock();
	Flight::GetFeasibleGatePosition();
	time2 = clock();
	duration2 = (time2 - time1) / (double)1000;
	cout << "������к�����û�λ��ʱ : " << duration2 << " ��" << endl;	//��ʾ����ʱ��
	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region ��û�λ����ͣ�ŵ����к���
	////////////////////////////////////////////////////////////////////////
	time1 = clock();
	GatePosition::GetFeasibleFlight();
	time2 = clock();
	duration3 = (time2 - time1) / (double)1000;
	cout << "��û�λ����ͣ�ŵ����к�����ʱ : " << duration3 << " ��" << endl;	//��ʾ����ʱ��
	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region ���cplex�����������
	//////////////////////////////////////////////////////////////////////////;
	{
		time1 = clock();
		string str = "NMR.txt";
		ofstream NM(str);
		NM << Flight::s_flightCount << " " << GatePosition::s_gatePositionCount << " " << Road::s_roadCount << " " << Flight::s_nemberCount << endl;
		NM << "[";
		int i = 0;
		for (map<string, Road *>::iterator iter = Road::s_mapSetOfRoad.begin(); iter != Road::s_mapSetOfRoad.end(); iter++)
		{
			if (iter->first == "������" || iter->first == "")
				NM << 0;
			else
				NM << 1;
			i++;
			if (i == Road::s_roadCount)
				NM << "]";
			else
				NM << ",";
		}
		NM.close();
		Flight::printTime();
		GatePosition::PrintFeasibleFlight();
		GatePosition::PrintDistance();
		Flight::printRoadTime();
		Road::printGatePositionToRoad();
		Flight::printConflictFlight();
		Flight::printNumberOfFlight();
		GatePosition::PrintTemporaryMeasures();
		GatePosition::PrintConflictGatePosition();
		GatePosition::PrintInfluenceGatePosition();
		time2 = clock();
		duration4 = (time2 - time1) / (double)1000;
		cout << "���cplex�������������ʱ : " << duration4 << " ��" << endl;	//��ʾ����ʱ��
	}
	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region CPLEX���
	//////////////////////////////////////////////////////////////////////////
	time1 = clock();
	solveByCplex();
	time2 = clock();
	duration5 = (time2 - time1) / (double)1000;
	cout << "CPLEX�����ʱ : " << duration5 << " ��" << endl;	//��ʾ����ʱ��
	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region ��ӡ���
	//////////////////////////////////////////////////////////////////////////
	time1 = clock();
	BestSolution::updateValueByCplexSolver();
	BestSolution::deleteWrongValue();
	BestSolution::heuristicForUndistributed();
	BestSolution::updateBestValue();
	BestSolution::printBestResult();
	string str = "";
	ostringstream oss;
	oss << BestSolution::m_objValue;
	str = oss.str();
	string result = "usedTimeWhenBestResult=" + str + ".txt";
	ofstream fout(result);
	time2 = clock();
	duration6 = (time2 - time1) / (double)1000;
	cout << "�洢�����csv�ļ���ʱ : " << duration6 << " ��" << endl;	//��ʾ����ʱ��
	//////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region ͳ�ƻ���ʱ��
	//////////////////////////////////////////////////////////////////////////
	finish = clock();								//��ֹ��ʱ(��)
	duration7 = duration1 + duration2 + duration3 + duration4 + duration5 + duration6;
	cout << "��������ʱ : " << duration7 << " ��" << endl;	//��ʾ����ʱ��
	//////////////////////////////////////////////////////////////////////////
	#pragma endregion

	#pragma region �������ʱ�䵽�ļ�
	//////////////////////////////////////////////////////////////////////////
	fout << "��ʼ��������ʱ : " << duration1 << " ��" << endl;	//��ʾ����ʱ��
	fout << "������к�����û�λ��ʱ : " << duration2 << " ��" << endl;	//��ʾ����ʱ��
	fout << "��û�λ����ͣ�ŵ����к�����ʱ : " << duration3 << " ��" << endl;	//��ʾ����ʱ��
	fout << "���cplex�������������ʱ : " << duration4 << " ��" << endl;	//��ʾ����ʱ��
	fout << "CPLEX�����ʱ : " << duration5 << " ��" << endl;	//��ʾ����ʱ��
	fout << "�洢�����csv�ļ���ʱ : " << duration6 << " ��" << endl;	//��ʾ����ʱ��
	fout << "��������ʱ : " << duration7 << " ��" << endl;	//��ʾ����ʱ��
	////////////////////////////////////////////////////////////////////////
	#pragma endregion

	system("pause");
	return 0;
}

void solveByCplex()
{
	// ����CPLEX����
	IloEnv env;

	try {
		int i, j, k;
		int parameter1 = 10;
		int parameter2 = 3;
		int parameter3 = 3;
		int parameter4 = 1;
		int parameter5 = 1;

		// ���ļ��ж�ȡ����
		ifstream f("NMR.txt");									// ��������(N)��ͣ��λ����(M)����������(R)�������Ƿ�Ϊ������(1*R)
		ifstream flightTime("flightTime.txt");					// �������ʱ��(1*N)������ʱ��(1*N)
		ifstream feasibleFlightFile("feasibleFlight.txt");			// ����-��λ�ɷ�ͣ��(N*M)
		ifstream distanceFile("distance.txt");					// ��λΪ����λ1��Զ��λ0(1*M)
		ifstream flightRoadTime("flightRoadTime.txt");			// �������ʱ���뻬��ʱ��(1*N)������ʱ�뿪����ʱ��(1*N)������ʱ���뻬��ʱ��(1*N)������ʱ�뿪����ʱ��(1*N)
		ifstream gatePositionToRoadFile("gatePositionToRoad.txt");	// ��λ-���뻬����Ӧ(M*R)����λ-�Ƴ�������Ӧ(M*R)
		ifstream conflictFlightFile("conflictFlight.txt");			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ���ֱ��ӡ����A�뺽��B�����뻬����ͻ���Ƴ�������ͻ���������Ƴ����Ƴ��뻬��
		ifstream numberOfFlightFile("numberOfFlight.txt");			// ÿ�ܺ�����ÿ�����(1*N)
		ifstream temporaryMeasuresFile("temporaryMeasures.txt");	// ��λ�Ƿ�Ϊ��ʱ��ʩ������ʱ��ʩΪ1������Ϊ0(1*M)
		ifstream conflictGatePositionFile("conflictGatePosition.txt");	// ����ͬʱʹ�õĻ�λ(M*M)��������ͬʱʹ����Ϊ1
		ifstream influenceGatePositionFile("influenceGatePosition.txt");	// ��Ӱ�컬��Ļ�λ(M*M)����Ӱ�컬���Ļ�λ(M*M)����Ӱ��Ϊ1

		// ������֪����
		IloInt N, M, R;										// ��������(N)��ͣ��λ����(M)����������(R)
		IloInt allNumber;										// ���ÿ�����
		IloIntArray _tin(env);									// �������ʱ��(1*N)
		IloIntArray _tout(env);								// �������ʱ��(1*N)
		IloArray<IloIntArray> _feasibleFlight(env);				// ����-��λ�ɷ�ͣ��(N*M)
		IloIntArray _distance(env);								// ��λΪ����λ1��Զ��λ0(1*M)
		IloIntArray _tComeBegin(env);							// �������ʱ���뻬��ʱ��(1*N)
		IloIntArray _tComeEnd(env);								// �������ʱ�뿪����ʱ��(1*N)
		IloIntArray _tGoBegin(env);								// �������ʱ���뻬��ʱ��(1*N)
		IloIntArray _tGoEnd(env);								// �������ʱ�뿪����ʱ��(1*N)
		IloArray<IloIntArray> _gatePositionToInRoad(env);			// ��λ-���뻬����Ӧ(M*R)
		IloArray<IloIntArray> _gatePositionToOutRoad(env);			// ��λ-�Ƴ�������Ӧ(M*R)
		IloArray<IloIntArray> _conflictFlightInIn(env);			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ�����뻬����ͻ
		IloArray<IloIntArray> _conflictFlightOutOut(env);			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ���Ƴ�������ͻ
		IloArray<IloIntArray> _conflictFlightInOut(env);			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ���������Ƴ���ͻ
		IloArray<IloIntArray> _conflictFlightOutIn(env);			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ���Ƴ��뻬���ͻ
		IloIntArray _noLimit(env);								// �����Ƿ�Ϊ������(1*R)
		IloIntArray _numberOfFlight(env);						// ÿ�ܺ�����ÿ�����(1*N)
		IloIntArray _temporaryMeasures(env);						// ��λ�Ƿ�Ϊ��ʱ��ʩ������ʱ��ʩΪ1������Ϊ0(1*M)
		IloArray<IloIntArray> _conflictGatePosition(env);			// ����ͬʱʹ�õĻ�λ(M*M)��������ͬʱʹ����Ϊ1
		IloArray<IloIntArray> _influenceGatePositionIn(env);		// ��Ӱ�컬��Ļ�λ(M*M)����Ӱ����Ϊ1
		IloArray<IloIntArray> _influenceGatePositionOut(env);		// ��Ӱ�컬���Ļ�λ(M*M)����Ӱ����Ϊ1

		f >> N >> M >> R >> allNumber >> _noLimit;
		flightTime >> _tin >> _tout;
		feasibleFlightFile >> _feasibleFlight;
		distanceFile >> _distance;
		flightRoadTime >> _tComeBegin >> _tComeEnd >> _tGoBegin >> _tGoEnd;
		gatePositionToRoadFile >> _gatePositionToInRoad >> _gatePositionToOutRoad;
		conflictFlightFile >> _conflictFlightInIn >> _conflictFlightOutOut >> _conflictFlightInOut >> _conflictFlightOutIn;
		numberOfFlightFile >> _numberOfFlight;
		temporaryMeasuresFile >> _temporaryMeasures;
		conflictGatePositionFile >> _conflictGatePosition;
		influenceGatePositionFile >> _influenceGatePositionIn >> _influenceGatePositionOut;

		// ������߱������
		IloArray<IloBoolArray> X(env, N);
		for (i = 0; i < N; i++)
		{
			X[i] = IloBoolArray(env, M);
			for (j = 0; j < M; j++)
			{
				X[i][j] = 0;
			}
		}
		IloArray<IloBoolArray> YIN(env, N);
		for (i = 0; i < N; i++)
		{
			YIN[i] = IloBoolArray(env, R);
			for (k = 0; k < R; k++)
			{
				YIN[i][k] = 0;
			}
		}
		IloArray<IloBoolArray> YOUT(env, N);
		for (i = 0; i < N; i++)
		{
			YOUT[i] = IloBoolArray(env, R);
			for (k = 0; k < R; k++)
			{
				YOUT[i][k] = 0;
			}
		}

		int N_once = 700;
		int N_once_2 = 300;
		int N_once_1 = N_once - N_once_2;
		bool runTimes =  true;
		int N_min = 0, N_max = 0;

		while (runTimes)
		{
			IloEnv env2;
			N_max += N_once_1;
			if (N_max >= N)
			{
				N_max = N;
				runTimes = false;
			}
			N_min = N_max - N_once;
			if (N_min < 0)
				N_min = 0;

			cout << "��ʼ����� " << N_min + 1 << " �ܵ��� " << N_max << " �ܺ���" << endl;

			// ������֪����
			IloIntArray tin(env2, N_max - N_min);									// �������ʱ��(1*N)
			IloIntArray tout(env2, N_max - N_min);								// �������ʱ��(1*N)
			IloArray<IloIntArray> feasibleFlight(env2, N_max - N_min);				// ����-��λ�ɷ�ͣ��(N*M)
			IloIntArray distance(env2, M);								// ��λΪ����λ1��Զ��λ0(1*M)
			IloIntArray tComeBegin(env2, N_max - N_min);							// �������ʱ���뻬��ʱ��(1*N)
			IloIntArray tComeEnd(env2, N_max - N_min);								// �������ʱ�뿪����ʱ��(1*N)
			IloIntArray tGoBegin(env2, N_max - N_min);								// �������ʱ���뻬��ʱ��(1*N)
			IloIntArray tGoEnd(env2, N_max - N_min);								// �������ʱ�뿪����ʱ��(1*N)
			IloArray<IloIntArray> gatePositionToInRoad(env2);			// ��λ-���뻬����Ӧ(M*R)
			IloArray<IloIntArray> gatePositionToOutRoad(env2);			// ��λ-�Ƴ�������Ӧ(M*R)
			IloArray<IloIntArray> conflictFlightInIn(env2, N_max - N_min);			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ�����뻬����ͻ
			IloArray<IloIntArray> conflictFlightOutOut(env2, N_max - N_min);			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ���Ƴ�������ͻ
			IloArray<IloIntArray> conflictFlightInOut(env2, N_max - N_min);			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ���������Ƴ���ͻ
			IloArray<IloIntArray> conflictFlightOutIn(env2, N_max - N_min);			// ����ʱ���ϳ�ͻ�ĺ���(N*N)�������ǰ��ŵĻ�������λ���Ƴ��뻬���ͻ
			IloIntArray noLimit(env2);								// �����Ƿ�Ϊ������(1*R)
			IloIntArray numberOfFlight(env2, N_max - N_min);						// ÿ�ܺ�����ÿ�����(1*N)
			IloIntArray temporaryMeasures(env2);						// ��λ�Ƿ�Ϊ��ʱ��ʩ������ʱ��ʩΪ1������Ϊ0(1*M)
			IloArray<IloIntArray> conflictGatePosition(env2);			// ����ͬʱʹ�õĻ�λ(M*M)��������ͬʱʹ����Ϊ1
			IloArray<IloIntArray> influenceGatePositionIn(env2);		// ��Ӱ�컬��Ļ�λ(M*M)����Ӱ����Ϊ1
			IloArray<IloIntArray> influenceGatePositionOut(env2);		// ��Ӱ�컬���Ļ�λ(M*M)����Ӱ����Ϊ1

			for (int i1 = 0; i1 < N_max - N_min; i1++)
			{
				tin[i1] = _tin[i1 + N_min];
				tout[i1] = _tout[i1 + N_min];
				tComeBegin[i1] = _tComeBegin[i1 + N_min];
				tComeEnd[i1] = _tComeEnd[i1 + N_min];
				tGoBegin[i1] = _tGoBegin[i1 + N_min];
				tGoEnd[i1] = _tGoEnd[i1 + N_min];
				numberOfFlight[i1] = _numberOfFlight[i1 + N_min];
				feasibleFlight[i1] = IloIntArray(env2, M);
				for (j = 0; j < M; j++)
				{
					feasibleFlight[i1][j] = _feasibleFlight[i1 + N_min][j];
				}
				conflictFlightInIn[i1] = IloIntArray(env2, N_max - N_min);
				conflictFlightOutOut[i1] = IloIntArray(env2, N_max - N_min);
				conflictFlightInOut[i1] = IloIntArray(env2, N_max - N_min);
				conflictFlightOutIn[i1] = IloIntArray(env2, N_max - N_min);
				for (int i2 = 0; i2 < N_max - N_min; i2++)
				{
					conflictFlightInIn[i1][i2] = _conflictFlightInIn[i1 + N_min][i2 + N_min];
					conflictFlightOutOut[i1][i2] = _conflictFlightOutOut[i1 + N_min][i2 + N_min];
					conflictFlightInOut[i1][i2] = _conflictFlightInOut[i1 + N_min][i2 + N_min];
					conflictFlightOutIn[i1][i2] = _conflictFlightOutIn[i1 + N_min][i2 + N_min];
				}
			}
			distance = _distance;
			gatePositionToInRoad = _gatePositionToInRoad;
			gatePositionToOutRoad = _gatePositionToOutRoad;
			noLimit = _noLimit;
			temporaryMeasures = _temporaryMeasures;
			conflictGatePosition = _conflictGatePosition;
			influenceGatePositionIn = _influenceGatePositionIn;
			influenceGatePositionOut = _influenceGatePositionOut;

			// ����ģ��
			IloModel model(env2);

			// ������߱���
			IloArray<IloBoolVarArray> x(env2, N_max - N_min);	// ����iռ��ͣ��λj
			for (i = 0; i < N_max - N_min; i++)
			{
				x[i] = IloBoolVarArray(env2, M);
			}
			IloArray<IloBoolVarArray> yin(env2, N_max - N_min);	// ����i����ʹ�û���k
			for (i = 0; i < N_max - N_min; i++)
			{
				yin[i] = IloBoolVarArray(env2, R);
			}
			IloArray<IloBoolVarArray> yout(env2, N_max - N_min);	// ����i����ʹ�û���k
			for (i = 0; i < N_max - N_min; i++)
			{
				yout[i] = IloBoolVarArray(env2, R);
			}
			IloBoolVarArray z2(env2, N_max - N_min);// ����i�Ƿ������������ͻ

			if (N_max - N_min > N_once_1)
			{
				int N_readNum = 0;
				int N_middle = N_max - N_min;
				if (N_middle < N_once)
					N_readNum = N_max - N_once_1;
				else
					N_readNum = N_once_2;
				// ��ȡ��һ�εĽ�
				for (i = 0; i < N_readNum; i++)
				{
					for (j = 0; j < M; j++)
					{
						model.add(x[i][j] == X[i + N_min][j]);
					}
				}
				for (i = 0; i < N_readNum; i++)
				{
					for (k = 0; k < R; k++)
					{
						model.add(yin[i][k] == YIN[i + N_min][k]);
					}
				}
				for (i = 0; i < N_readNum; i++)
				{
					for (k = 0; k < R; k++)
					{
						model.add(yout[i][k] == YOUT[i + N_min][k]);
					}
				}
			}

			// Ŀ�꺯��
			// ��һ����
			IloNumExprArray distributionFlightCountTemp(env2, N_max - N_min);
			for (i = 0; i < N_max - N_min; i++)
			{
				distributionFlightCountTemp[i] = IloNumExpr(env2);
				for (j = 0; j < M; j++)
				{
					distributionFlightCountTemp[i] = distributionFlightCountTemp[i] + x[i][j];
				}
			}
			IloNumExpr distributionFlightCount = IloSum(distributionFlightCountTemp) / (double)(N_max - N_min);
			//IloNumExpr distributionFlightCount = IloSum(distributionFlightCountTemp) / (double)(Flight::s_flightCount + Flight::s_wrongFlightCount);
			// �ڶ�����
			IloNumExprArray distanceGatePositionCountTemp(env2, M);
			for (j = 0; j < M; j++)
			{
				distanceGatePositionCountTemp[j] = IloNumExpr(env2);
				if (distance[j] == 1)
				{
					for (i = 0; i < N_max - N_min; i++)
					{
						distanceGatePositionCountTemp[j] = distanceGatePositionCountTemp[j] + x[i][j];
					}
				}
			}
			IloNumExpr distanceGatePositionCount = IloSum(distanceGatePositionCountTemp) / (double)(N_max - N_min);
			//IloNumExpr distanceGatePositionCount = IloSum(distanceGatePositionCountTemp) / (double)(Flight::s_flightCount + Flight::s_wrongFlightCount);
			// ��������
			IloNumExprArray distanceGatePositionNumberCountTemp(env2, M);
			for (j = 0; j < M; j++)
			{
				distanceGatePositionNumberCountTemp[j] = IloNumExpr(env2);
				if (distance[j] == 1)
				{
					for (i = 0; i < N_max - N_min; i++)
					{
						distanceGatePositionNumberCountTemp[j] = distanceGatePositionNumberCountTemp[j] + x[i][j] * numberOfFlight[i];
					}
				}
			}
			IloNumExpr distanceGatePositionNumberCount = IloSum(distanceGatePositionNumberCountTemp) / (double)(IloSum(numberOfFlight));
			// ���Ĳ���
			IloNumExprArray temporaryMeasuresTemp(env2, M);
			for (j = 0; j < M; j++)
			{
				temporaryMeasuresTemp[j] = IloNumExpr(env2);
				if (temporaryMeasures[j] == 1)
				{
					for (i = 0; i < N_max - N_min; i++)
					{
						temporaryMeasuresTemp[j] = temporaryMeasuresTemp[j] + x[i][j];
					}
				}
			}
			IloNumExpr temporaryMeasuresSum = IloSum(temporaryMeasuresTemp) / (double)(N_max - N_min);
			// ���岿��
			for (i = 0; i < N_max - N_min - 1; i++)
			{
				for (j = i + 1; j < N_max - N_min; j++)
				{
					if (conflictFlightInIn[i][j] == 1)
					{
						for (k = 0; k < R; k++)
						{
							if (noLimit[k] == 1)
							{
								// �����޸�
								model.add(z2[i] >= yin[i][k] + yin[j][k] - 1.1);
								model.add(z2[j] >= yin[i][k] + yin[j][k] - 1.1);
							}
						}
					}
					if (conflictFlightOutOut[i][j] == 1)
					{
						for (k = 0; k < R; k++)
						{
							if (noLimit[k] == 1)
							{
								// �����޸�
								model.add(z2[i] >= yout[i][k] + yout[j][k] - 1.1);
								model.add(z2[j] >= yout[i][k] + yout[j][k] - 1.1);
							}
						}
					}
					if (conflictFlightInOut[i][j] == 1)
					{
						for (k = 0; k < R; k++)
						{
							if (noLimit[k] == 1)
							{
								// �����޸�
								model.add(z2[i] >= yin[i][k] + yout[j][k] - 1.1);
								model.add(z2[j] >= yin[i][k] + yout[j][k] - 1.1);
							}
						}
					}
					if (conflictFlightOutIn[i][j] == 1)
					{
						for (k = 0; k < R; k++)
						{
							if (noLimit[k] == 1)
							{
								// �����޸�
								model.add(z2[i] >= yout[i][k] + yin[j][k] - 1.1);
								model.add(z2[j] >= yout[i][k] + yin[j][k] - 1.1);
							}
						}
					}
				}
			}
			IloNumExpr conflictCount = IloSum(z2) / (double)(N_max - N_min);

			// ���Լ������
			model.add(IloMaximize(env2, parameter1 * distributionFlightCount + parameter2 * distanceGatePositionCount + parameter3 * distanceGatePositionNumberCount - parameter4 * temporaryMeasuresSum - parameter5 * conflictCount));

			// Լ������1 ÿ�ܺ���ֻ��ѡ��һ����λ
			IloNumExprArray xTemp1(env2, N_max - N_min);
			for (i = 0; i < N_max - N_min; i++)
			{
				xTemp1[i] = IloNumExpr(env2);
				for (j = 0; j < M; j++)
				{
					if (feasibleFlight[i][j] == 1)
						xTemp1[i] = xTemp1[i] + x[i][j];
					else
					{
						model.add(x[i][j] == 0);
					}
				}
				// �����޸�
				model.add(xTemp1[i] <= 1.1);
			}

			//for (i = 0; i < N_max - N_min; i++)
			//{
			//	for (j = 0; j < M; j++)
			//	{
			//		if (feasibleFlight[i][j] == 0)
			//			model.add(x[i][j] == 0);
			//	}
			//	// �����޸�
			//	model.add(IloSum(x[i]) <= 1.1);
			//}

			// Լ������2 ͬһ��λͬһʱ��ֻ��ͣһ�ܺ���
			for (j = 0; j < M; j++)
			{
				for (int i1 = 0; i1 < N_max - N_min - 1; i1++)
				{
					for (int i2 = i1 + 1; i2 < N_max - N_min; i2++)
					{
						if ((feasibleFlight[i1][j] == 1) && (feasibleFlight[i2][j] == 1) && ((tin[i1] - tout[i2]) * (tin[i2] - tout[i1]) > 0))
						{
							// �����޸�
							model.add(x[i1][j] + x[i2][j] <= 1.1);
						}
					}
				}
			}

			// Լ������3��������ʹ�û�����Լ��
			for (i = 0; i < N_max - N_min; i++)
			{
				if (IloSum(feasibleFlight[i]) == 0)
				{
					model.add(IloSum(yin[i]) == 0);
					model.add(IloSum(yout[i]) == 0);
					//for (k = 0; k < R; k++)
					//{
					//	model.add(yin[i][k] == 0);
					//	model.add(yout[i][k] == 0);
					//}
					continue;
				}
				model.add(IloSum(yin[i]) == IloSum(x[i]));
				model.add(IloSum(yout[i]) == IloSum(x[i]));
				for (j = 0; j < M; j++)
				{
					if (feasibleFlight[i][j] == 1)
					{
						for (k = 0; k < R; k++)
						{
							if (gatePositionToInRoad[j][k] == 1)
							{
								model.add(yin[i][k] >= x[i][j]);
							}
							if (gatePositionToOutRoad[j][k] == 1)
							{
								model.add(yout[i][k] >= x[i][j]);
							}
						}
					}
				}
			}

			// Լ������4 ��ͻ��λ����ͬʱʹ��
			for (int j1 = 0; j1 < M; j1++)
			{
				for (int j2 = 0; j2 < M; j2++)
				{
					if (conflictGatePosition[j1][j2] == 1)
					{
						for (int i1 = 0; i1 < N_max - N_min; i1++)
						{
							for (int i2 = 0; i2 < N_max - N_min; i2++)
							{
								if ((feasibleFlight[i1][j1] == 1) && (feasibleFlight[i2][j2] == 1) && ((tin[i1] - tout[i2]) * (tin[i2] - tout[i1]) > 0))
								{
									// �����޸�
									model.add(x[i1][j1] + x[i2][j2] <= 1.1);
								}
							}
						}
					}
				}
			}

			// Լ������5 �ض���λ��Ӱ��ĳЩ��λ�Ļ���򻬳�
			for (int j1 = 0; j1 < M; j1++)
			{
				for (int j2 = 0; j2 < M; j2++)
				{
					if (influenceGatePositionIn[j1][j2] == 1)
					{
						for (int i1 = 0; i1 < N_max - N_min; i1++)
						{
							for (int i2 = 0; i2 < N_max - N_min; i2++)
							{
								if ((feasibleFlight[i1][j1] == 1) && (feasibleFlight[i2][j2] == 1) && (tin[i1] < tin[i2]) && (tin[i2] < tout[i1]))
								{
									// �����޸�
									model.add(x[i1][j1] + x[i2][j2] <= 1.1);
								}
							}
						}
					}
					if (influenceGatePositionOut[j1][j2] == 1)
					{
						for (int i1 = 0; i1 < N_max - N_min; i1++)
						{
							for (int i2 = 0; i2 < N_max - N_min; i2++)
							{
								if ((feasibleFlight[i1][j1] == 1) && (feasibleFlight[i2][j2] == 1) && (tin[i1] < tout[i2]) && (tout[i2] < tout[i1]))
								{
									// �����޸�
									model.add(x[i1][j1] + x[i2][j2] <= 1.1);
								}
							}
						}
					}
				}
			}

			//��ģ�Ͷ��嵽CPLEX��
			IloCplex cplex2(model);

			//���ʱ������
			//cplex2.setParam(IloCplex::TiLim, 600);

			//����Gap
			cplex2.setParam(IloCplex::EpGap, 0.002);
			
			//ִ�����
			cplex2.solve();

			// ���߱������
			for (i = 0; i < N_max - N_min; i++)
			{
				for (j = 0; j < M; j++)
				{
					X[i + N_min][j] = (int)((double)cplex2.getValue(x[i][j]) + 0.5);
				}
			}
			for (i = 0; i < N_max - N_min; i++)
			{
				for (k = 0; k < R; k++)
				{
					YIN[i + N_min][k] = (int)((double)cplex2.getValue(yin[i][k]) + 0.5);
				}
			}
			for (i = 0; i < N_max - N_min; i++)
			{
				for (k = 0; k < R; k++)
				{
					YOUT[i + N_min][k] = (int)((double)cplex2.getValue(yout[i][k]) + 0.5);
				}
			}

			//// ������
			//char N_min_char[5];
			//_itoa(N_min, N_min_char, 10);
			//string N_min_str(N_min_char);
			//char N_max_char[5];
			//_itoa(N_max, N_max_char, 10);
			//string N_max_str(N_max_char);
			//string str = "X[" + N_min_str + "] -- X[" + N_max_str + "].dat";
			//ofstream Xfileout(str);
			//for (i = 0; i < N_max - N_min; i++)
			//{
			//	int Xsum = 0;
			//	for (j = 0; j < M; j++)
			//	{
			//		Xfileout << X[i + N_min][j] << "\t";
			//		Xsum += X[i + N_min][j];
			//	}
			//	Xfileout << "Xsum = " << Xsum << "\t";
			//	Xfileout << "xTemp1 = " << cplex2.getValue(IloSum(x[i])) << endl;
			//}


			cout << "��û�λ��������" << cplex2.getValue(IloSum(distributionFlightCountTemp)) << endl;
			cout << "���ź�������" << cplex2.getValue(IloSum(distanceGatePositionCountTemp)) << endl;
			cout << "�����ÿ�������" << cplex2.getValue(IloSum(distanceGatePositionNumberCountTemp)) << endl;
			cout << "ִ����ʱ��ʩ�ĺ�������" << cplex2.getValue(IloSum(temporaryMeasuresTemp)) << endl;
			cout << "����ͨ����ͻ��������" << cplex2.getValue(IloSum(z2)) << endl;
			cout << "getObjValue��" << cplex2.getObjValue() << endl;
			cout << "getBestObjValue��" << cplex2.getBestObjValue() << endl;
			
			env2.end();
		}

		//cout << "��û�λ��������" << cplex.getValue(IloSum(distributionFlightCountTemp)) << endl;
		//cout << "���ź�������" << cplex.getValue(IloSum(distanceGatePositionCountTemp)) << endl;
		//cout << "�����ÿ�������" << cplex.getValue(IloSum(distanceGatePositionNumberCountTemp)) << endl;
		//cout << "ִ����ʱ��ʩ�ĺ�������" << cplex.getValue(IloSum(temporaryMeasuresTemp)) << endl;
		//cout << "����ͨ����ͻ��������" << cplex.getValue(IloSum(z2)) << endl;
		//cout << "getObjValue��" << cplex.getObjValue() << endl;
		//cout << "getBestObjValue��" << cplex.getBestObjValue() << endl;

		IloInt X_SUM;
		IloIntArray X_SUMI(env,N);
		for (i = 0; i < N; i++)
		{
			X_SUMI[i] = IloSum(X[i]);
			for (j = 0; j < M; j++)
			{
				X_SUM = IloSum(X_SUMI);
			}
		}
		cout << "X_SUM��" << X_SUM << endl;

		for (i = 0; i < N; i++)
		{
			vector<int> resultTemp;
			for (j = 0; j < M; j++)
			{
				resultTemp.push_back(X[i][j]);
			}
			BestSolution::m_solution.push_back(resultTemp);
		}

		cout << endl;
	}

	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
	catch (...) {
		cerr << "Error" << endl;
	}

	env.end();
}
