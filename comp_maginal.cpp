extern "C" {
#include <R.h>
#include <Rmath.h>
};

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <random>
#include <numeric>
#include <algorithm> 
#include <functional> 
#include <memory>

#include <io.hpp>
#include <vector_function.hpp>
#include <random_num.hpp>

#include <hmm.hpp>
#include <serialization.hpp>

using namespace std;

template <class T>
void copy(vector<T> v, T* arr){
  for (int i = 0; i <v.size() ; ++i){
    arr[i]=v[i];
  }
}

template <class T>
void copy_back(vector<T>& v, T* arr){
	for (int i = 0; i <v.size() ; ++i){
		v[i]=arr[i];
	}
}

extern "C" void test_HMM(
  double* X_arr,
  int* N_sub,
  int* N,
  int* p,
  int* K,
  int* burnin,
  int* step,
  int* thin,
  /*,int* sub_idx,
  int* var_idx,
  int* K, int* step, int* thin,
  */
  //double* trace_mu,
  //double* trace_sigma,
  //double* trace_q,
  //double* trace_pi0,

  double* best_mu,
  double* best_sigma,
  double* best_q,
  double* best_pi0,
  
  double* trace_loglik,
  double* marginal_loglik
  ){

  Posterior_Mode hmm_est;

  hmm_est.mu.resize((*K)*(*p));
  hmm_est.sigma.resize((*K)*(*p));
  hmm_est.q.resize((*K)*(*K));
  hmm_est.pi0.resize((*K));

  copy_back(hmm_est.mu, best_mu);
  copy_back(hmm_est.sigma, best_sigma);
  copy_back(hmm_est.q, best_q);
  copy_back(hmm_est.pi0, best_pi0);


  ///////////////////////////////////////
  vector< vector<vector<double>>> X_full;
  X_full.resize(0);

  int offset=0;
  for (int i = 0; i < *N_sub; ++i)
  {	
  	vector<vector<double>> X_1sub(0);
  	for (int j = 0; j < N[i]; ++j)
  	{
  		vector<double> X_1obs(*p);
  		X_1obs.assign(&X_arr[offset+j*(*p)],&X_arr[offset+j*(*p)]+(*p));
  		X_1sub.push_back(X_1obs);
  	}
  	offset+=(*p)*N[i];
  	X_full.push_back(X_1sub);
  }

  ///////////////////////////////////////

  HMM hmm_test(*K, X_full);

  UpdateHMM_w_Est(hmm_test, hmm_est);

  ///////////////////////////////////////

  hmm_test.Run_S_only(*step, *burnin, *thin);
  
  /////////////////////////////////////


	copy(hmm_test.loglik_trace,trace_loglik);

  *marginal_loglik=sum(hmm_test.loglik_trace)/(*step);

}

/*
int main(int argc, char** argv)
  {

//std::mt19937 gen(rd());



    for(int i=0;i<12;i++)
	{
    string temp_file = "../test_data/apple/data" + to_string(i+1) +".csv";
    X_full.push_back(readCSV2(temp_file.c_str()));
	}




    const int N_chain=1;
    vector<shared_ptr<HMM>> hmm_chains(N_chain);
    for(int i=0;i<N_chain;i++){
    	hmm_chains[i].reset(new HMM(3, X_full));
    }

    #pragma omp parallel for
    for(int i=0;i<N_chain;i++){
   	hmm_chains[i]->Burnin(5000);
  	hmm_chains[i]->Run(10000,10);
  	}


	Posterior_Mode hmm_est(*hmm_chains[0]);
 	save(hmm_est, "output/apple.hmm");

 	//cout<<hmm_est.q<<endl;


    vector< vector<vector<double>>> X_test; 

    for(int i=0;i<1;i++)
	{
    string temp_file = "../test_data/apple/data" + to_string(i+1) +".csv";
    X_test.push_back(readCSV2(temp_file.c_str()));
	}



	HMM hmm1(3, X_test);

	UpdateHMM_w_Est(hmm1, hmm_est);

	hmm1.Run_S_only(1000, 500, 10);
	{
	string temp_file = "./output/test_loglik.csv";
	std::ofstream ofs_pred(temp_file.c_str());
	ofs_pred<<hmm1.loglik_trace<<endl;
	ofs_pred.close();
	}

}
*/