#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <cstdlib>
#include <papi.h>
#include <omp.h>
#include <chrono>

using namespace std;

#define SYSTEMTIME clock_t

// Command to run papi -> g++ -O2 filename.cpp -o filename -lpapi

using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;

TimePoint Now(){
	return Clock::now();
}

double DurationInSeconds(const TimePoint& start, const TimePoint& end){
	return std::chrono::duration<double>(end - start).count();
}

long long values[6];
int EventSet = PAPI_NULL;
int ret;

// Command to see all events -> papi_avail -a
void PappiStart()
{

	ret = PAPI_library_init( PAPI_VER_CURRENT );
	if ( ret != PAPI_VER_CURRENT )
		std::cout << "FAIL" << endl;

	ret = PAPI_create_eventset(&EventSet);
	if (ret != PAPI_OK) cout << "ERROR: create eventset" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L1_ICM );
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L1_ICM" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L2_ICM);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L2_ICM" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L2_ICH);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L2_ICH" << endl;

	ret = PAPI_add_event(EventSet,PAPI_DP_OPS);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_DP_OPS" << endl;

	ret = PAPI_add_event(EventSet,PAPI_TOT_INS);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_TOT_INS" << endl;

	ret = PAPI_add_event(EventSet,PAPI_L1_DCM);
	if (ret != PAPI_OK) cout << "ERROR: PAPI_L1_DCM" << endl;

	// ret = PAPI_add_event(EventSet,PAPI_L2_DCM);
	// if (ret != PAPI_OK) cout << "ERROR: PAPI_L2_DCM" << endl;

	ret = PAPI_start(EventSet);
	if (ret != PAPI_OK) cout << "ERROR: Start PAPI" << endl;

}

void PappiEnd(){
	ret = PAPI_stop(EventSet, values);
	if (ret != PAPI_OK) cout << "ERROR: Stop PAPI" << endl;
	printf("L1 ICM: %lld \n",values[0]);
	printf("L2 ICM: %lld \n",values[1]);
	printf("L2 ICH: %lld \n",values[2]);
	printf("DP OPS: %lld \n",values[3]);
	printf("TOT INS: %lld \n",values[4]);
	printf("L1 DCM: %lld \n",values[5]);
	// printf("L2 DCM: %lld \n",values[6]);

	ret = PAPI_reset( EventSet );
	if ( ret != PAPI_OK )
		std::cout << "FAIL reset; " << endl;

	ret = PAPI_remove_event( EventSet, PAPI_L1_ICM );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl;

	ret = PAPI_remove_event( EventSet, PAPI_L2_ICM );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl;

	ret = PAPI_remove_event( EventSet, PAPI_L2_ICH );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl;

	ret = PAPI_remove_event( EventSet, PAPI_DP_OPS );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl;

	ret = PAPI_remove_event( EventSet, PAPI_TOT_INS );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl;

	ret = PAPI_remove_event( EventSet, PAPI_L1_DCM );
	if ( ret != PAPI_OK )
		std::cout << "FAIL remove event" << endl;
	
	// ret = PAPI_remove_event( EventSet, PAPI_L2_DCM );
	// if (ret != PAPI_OK)
	// 	std::cout << "FAIL remove event" << endl;

	ret = PAPI_destroy_eventset( &EventSet );
	if (ret != PAPI_OK){
		std::cout << "FAIL destroy eventset" << endl;
	}

}
 
void OnMult(int m_ar, int m_br) 
{	
	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;
	

		
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;



	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);



    TimePoint startTime = Now();
	PappiStart();
	for(i=0; i<m_ar; i++)
	{	for( j=0; j<m_br; j++)
		{	temp = 0;
			for( k=0; k<m_ar; k++)
			{	
				temp += pha[i*m_ar+k] * phb[k*m_br+j];
			}
			phc[i*m_ar+j]=temp;
		}
	}

	PappiEnd();
    TimePoint endTime = Now();

	double duration = DurationInSeconds(startTime, endTime);

	sprintf(st, "Time: %3.3f seconds\n", duration);
	cout << st;

	// display 10 elements of the result matrix tto verify correctness
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	for(j=0; j<min(10,m_br); j++)
			cout << phc[j] << " ";
	}
	cout << endl;

    free(pha);
    free(phb);
    free(phc);
	
	
}

// add code here for line x line matriz multiplication
void OnMultLine(int m_ar, int m_br)
{	
	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;
	

		
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;



	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phc[i*m_br + j] = (double)0.0;



	TimePoint startTime = Now();
	PappiStart();
	for(i=0; i<m_ar; i++)
		for(k=0; k<m_ar; k++)
			for( j=0; j<m_ar; j++)
				phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];

	PappiEnd();
	TimePoint endTime = Now();
	double duration = DurationInSeconds(startTime, endTime);
	sprintf(st, "Time: %3.3f seconds\n", duration);
    cout << st;

	// display 10 elements of the result matrix tto verify correctness
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	for(j=0; j<min(10,m_br); j++)
			cout << phc[j] << " ";
	}
	cout << endl;

    free(pha);
    free(phb);
    free(phc);
}

// add code here for line x line matriz multiplication with parallel for
void OnMultLineParFor(int m_ar, int m_br)
{
	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;
	

		
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;



	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phc[i*m_br + j] = (double)0.0;



	TimePoint startTime = Now();
	PappiStart();
	#pragma omp parallel for
	for(i=0; i<m_ar; i++)
		for(k=0; k<m_ar; k++)
			for( j=0; j<m_ar; j++)
				phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];

	PappiEnd();
	TimePoint endTime = Now();
	double duration = DurationInSeconds(startTime, endTime);
	sprintf(st, "Time: %3.3f seconds\n", duration);
    cout << st;

	// display 10 elements of the result matrix tto verify correctness
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	for(j=0; j<min(10,m_br); j++)
			cout << phc[j] << " ";
	}
	cout << endl;

    free(pha);
    free(phb);
    free(phc);
}

// add code here for line x line matriz multiplication parrallel for j
void OnMultLineParForJ(int m_ar, int m_br)
{
	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;
	

		
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;



	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phc[i*m_br + j] = (double)0.0;


	TimePoint startTime = Now();
	PappiStart();
	#pragma omp parallel
	for(i=0; i<m_ar; i++)
		for(k=0; k<m_ar; k++)
			#pragma omp for
			for( j=0; j<m_ar; j++)
				phc[i*m_ar+j] += pha[i*m_ar+k] * phb[k*m_br+j];

	PappiEnd();
	TimePoint endTime = Now();
	double duration = DurationInSeconds(startTime, endTime);
	sprintf(st, "Time: %3.3f seconds\n", duration);
    cout << st;

	// display 10 elements of the result matrix tto verify correctness
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	for(j=0; j<min(10,m_br); j++)
			cout << phc[j] << " ";
	}
	cout << endl;

    free(pha);
    free(phb);
    free(phc);
}

// add code here for block x block matriz multiplication
void OnMultBlock(int m_ar, int m_br, int bkSize)
{
	char st[100];
	double temp;
	int i, j, k;

	double *pha, *phb, *phc;
	

		
    pha = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phb = (double *)malloc((m_ar * m_ar) * sizeof(double));
	phc = (double *)malloc((m_ar * m_ar) * sizeof(double));

	for(i=0; i<m_ar; i++)
		for(j=0; j<m_ar; j++)
			pha[i*m_ar + j] = (double)1.0;



	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phb[i*m_br + j] = (double)(i+1);

	for(i=0; i<m_br; i++)
		for(j=0; j<m_br; j++)
			phc[i*m_br + j] = (double)0.0;

	TimePoint startTime = Now();
	PappiStart();
	for(int jj = 0; jj < m_ar; jj += bkSize)
		for(int kk = 0; kk < m_ar; kk += bkSize)
			for(int i = 0; i < m_ar; i++)
				for(int k = kk; k < min(kk + bkSize, m_ar); k++)
					for(int j = jj; j < min(jj + bkSize, m_br); j++)
						phc[i*m_br+j] += pha[i*m_ar+k] * phb[k*m_br+j];

	PappiEnd();
	TimePoint endTime = Now();
	double duration = DurationInSeconds(startTime, endTime);
	sprintf(st, "Time: %3.3f seconds\n", duration);
	cout << st;

	// display 10 elements of the result matrix tto verify correctness
	cout << "Result matrix: " << endl;
	for(i=0; i<1; i++)
	{	for(j=0; j<min(10,m_br); j++)
			cout << phc[j] << " ";
	}
	cout << endl;

    free(pha);
    free(phb);
    free(phc);
    
}

/*

	Flops, papisstart - papiend (ver configs no avaliable), tempos,  Cronos para o paralelismo

*/

void handle_error (int retval)
{
  printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
  exit(1);
}

void init_papi() {
  int retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT && retval < 0) {
    printf("PAPI library version mismatch!\n");
    exit(1);
  }
  if (retval < 0) handle_error(retval);

  std::cout << "PAPI Version Number: MAJOR: " << PAPI_VERSION_MAJOR(retval)
            << " MINOR: " << PAPI_VERSION_MINOR(retval)
            << " REVISION: " << PAPI_VERSION_REVISION(retval) << "\n";
}


int main (int argc, char *argv[])
{
	freopen("test.txt","w",stdout);

	char c;
	int lin, col, blockSize;
	int op;


	op=1;
	do {
		cout << endl << "1. Multiplication" << endl;
		cout << "2. Line Multiplication" << endl;
		cout << "3. Block Multiplication" << endl;
		cout << "Selection?: ";
		cin >>op;
		if (op == 0)
			break;
		// printf("Dimensions: lins=cols ? ");
   		// cin >> lin;
   		// col = lin;


		// Start counting

		switch (op){
			case 1: 
				for(int i = 600; i <= 3000; i+=400){
					cout << endl;
					cout << "Dimension: " << i << " x " << i << endl;
					OnMult(i, i);
					cout << endl;
				}
				break;
			case 2:
				for(int i = 600; i <= 3000; i+=400){
					cout << endl;
					cout << "Dimension: " << i << " x " << i << endl;
					OnMultLine(i, i);
					cout << endl;
				}
				// for (int i = 4096; i <= 10240; i+=2048){
				//  cout << endl;
				//	cout << "Dimension: " << i << " x " << i << endl;
				// 	OnMultLine(i, i);
				// 	cout << endl;
				// }
				// for (int i = 4096; i <= 10240; i+=2048){
				//  cout << endl;
				// 	cout << "Dimension: " << i << " x " << i << endl;
				// 	OnMultLineParFor(i, i);
				// 	cout << endl;
				// }
				// for (int i = 4096; i <= 10240; i+=2048){
				// 	cout << endl;
				// 	cout << "Dimension: " << i << " x " << i << endl;
				// 	OnMultLineParForJ(i, i);
				// 	cout << endl;
				// }
				break;
			case 3:
				for(int i = 4096; i <= 10240; i+=2048){
					cout << endl;
					int blockSize = 128; // 128, 256, 512
					cout << "Dimension: " << i << " x " << i << endl;
					cout << "Block size: " << blockSize << endl;
					OnMultBlock(i, i, blockSize);
					cout << endl;
				}
				break;

		}

	}while (op != 0);

} 