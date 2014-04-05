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

using namespace std;



int main(int argc, char** argv)
  {

//std::mt19937 gen(rd());


    vector< vector<vector<double>>> X_full; 

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
  	hmm_chains[i]->Run(5000,10);
  	}

	#pragma omp parallel for
    for(int i=0;i<N_chain;i++){

		{ 
	    string temp_file = "./output/mu_trace_" + to_string(i) +".csv";
		std::ofstream ofs_pred(temp_file.c_str());
		ofs_pred<<hmm_chains[i]->mu_trace<<endl;
		ofs_pred.close();
		}

		{ 
	    string temp_file = "./output/sigma_trace_" + to_string(i) +".csv";
		std::ofstream ofs_pred(temp_file.c_str());
		ofs_pred<<hmm_chains[i]->sigma_trace<<endl;
		ofs_pred.close();
		}

		{ 
	    string temp_file = "./output/q_trace_" + to_string(i) +".csv";
		std::ofstream ofs_pred(temp_file.c_str());
		ofs_pred<<hmm_chains[i]->q_trace<<endl;
		ofs_pred.close();
		}

		{ 
	    string temp_file = "./output/pi0_trace_" + to_string(i) +".csv";
		std::ofstream ofs_pred(temp_file.c_str());
		ofs_pred<<hmm_chains[i]->pi0_trace<<endl;
		ofs_pred.close();
		}

		{ 
	    string temp_file = "./output/s_trace_" + to_string(i) +".csv";
		std::ofstream ofs_pred(temp_file.c_str());
		ofs_pred<<hmm_chains[i]->s_trace<<endl;
		ofs_pred.close();
		}

		{ 
		string temp_file = "./output/loglik_trace_" + to_string(i) +".csv";
		std::ofstream ofs_pred(temp_file.c_str());
		ofs_pred<<hmm_chains[i]->loglik_trace<<endl;
		ofs_pred.close();
		}
	}
	
}
