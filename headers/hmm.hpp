//in the test case K=3, m=6

class HMM{
public:
	shared_ptr<std::mt19937> gen;
	int N_sub; //No. of samples
	int K;  int p;   //p: data dimension
	vector<int> N;

    vector<vector<vector<double>>> data; // N_sub samples x N_i frames* m
    vector<vector<double>> mu; //K states * m
    vector<vector<double>> sigma; //K states *m
    vector<vector<double>> q; //K*K transition matrix: from row idx to col idx
    vector<vector<int>> q_count; //K*K transition counts matrix: from row idx to col idx
   // vector<double> p_gaussian; //Gaussian prob: N frames
    vector<vector<int>>	s;  //N_sub of membership N frame s=1...K
    vector<int>	s_count;  // K, state counts
    vector<double> pi0; //initial state density


    //record of MCMC chain:
    vector<vector<double>>	mu_trace;
	vector<vector<double>> sigma_trace;
	vector<vector<double>> q_trace;
	vector<vector<double>> pi0_trace;
	vector<vector<int>> s_trace;
	vector<double> loglik_trace;


	std::vector<vector<double>> prob_gaussian;
	std::vector<vector<double>> prob_q;


    HMM(int _K, vector<vector<vector<double>>> _data);
    void Upd_Counts();
    void Upd_Gaussian();
double Comp_Normal_aux(double x, double mu, double sigma);
double Comp_Normal(vector<double> x, vector<double> mu,
							vector<double> sigma);
    void Upd_S_aux(int, int);
    void Upd_S();
    void Upd_Q();
    void Upd_pi0();
    void Update();
    void Burnin(int);
    void Run(int step, int thin);

    double loglik();




};

HMM::HMM(int _K, vector<vector<vector<double>>> _data){

	std::random_device rd;
	gen.reset(new std::mt19937(rd()));


	K=_K;
	data=_data;
	N_sub=data.size();
	p=data[0][0].size();
	//cout<<p<<endl;
	//N=data[0].size();
	mat_resize(mu,K,p);
	mat_resize(sigma,K,p);
	mat_resize(q,K,K);
	mat_resize(q_count,K,K);
	s_count.resize(K);
	//p_gaussian.resize(N);
	N.resize(N_sub);
	s.resize(N_sub);
	for (int sub_idx = 0; sub_idx < N_sub; ++sub_idx)
	{
		N[sub_idx]=data[sub_idx].size();
		s[sub_idx].resize(N[sub_idx]);
		for (int j = 0; j < N[sub_idx]; ++j)
		{
		s[sub_idx][j]=j%K;
		}
		s[sub_idx]=shuffle(s[sub_idx],gen);
	}

	pi0.resize(K);
	fill(pi0,1./(double)K);
	fill(q,1./(double)K);
	fill(mu,0.);
	fill(sigma,1.);
	fill(q_count,0);

	prob_gaussian.resize(N_sub);
	prob_q.resize(N_sub);


	for (int i = 0; i < N_sub; ++i)
	{
		prob_gaussian[i].resize(N[i]);
		prob_q[i].resize(N[i]-1);
	}


}

void HMM::Upd_Counts(){
	fill(s_count,0);
	for(int i=0;i<K;i++){
		s_count[i]=count(s,i);
	}
	fill(q_count,0);
	for(int sub_idx=0;sub_idx<N_sub;sub_idx++){
		for(int i=1;i<N[sub_idx];i++){
		q_count[s[sub_idx][i-1]][s[sub_idx][i]]+=1;
		}
	}

}


void HMM::Upd_Gaussian(){
	//update mu
	vector<vector<double>> mu_mean;
	vector<vector<double>> mu_var;
	mat_resize(mu_mean,K,p);
	mat_resize(mu_var,K,p);

	fill(mu_mean,0.);
	fill(mu_var,0.);


	for(int j=0;j<p;j++){
		for(int sub_idx=0;sub_idx<N_sub;sub_idx++){
			for(int i=0;i<N[sub_idx];i++){
				mu_mean[s[sub_idx][i]][j]+=data[sub_idx][i][j];
			}
		}
	}



	for(int i=0;i<K;i++){
		if(s_count[i]>0){
		mu_mean[i]=scale(mu_mean[i],1./s_count[i]);
		mu_var[i]=scale(sigma[i],1./s_count[i]);}
	}

	fill(mu,0.);
	for(int i=0;i<K;i++){
		for(int j=0;j<p;j++){
			if(mu_var[i][j]>0){
				mu[i][j]=rNormal(gen,mu_mean[i][j],mu_var[i][j]);
			}
		}
	}


	//update sigma
	vector<vector<double>> sigma_alpha;
	vector<vector<double>> sigma_beta;
	mat_resize(sigma_alpha,K,p);
	mat_resize(sigma_beta,K,p);

	for (int i = 0; i < K; ++i)
	{
		fill(sigma_alpha[i],((double)s_count[i])/2.);	
	}

	fill(sigma_beta,0.);

	for(int sub_idx=0;sub_idx<N_sub;sub_idx++){
		for(int j=0;j<p;j++){
			for(int i=0;i<N[sub_idx];i++){
				double temp=data[sub_idx][i][j]-mu[s[sub_idx][i]][j];
				sigma_beta[s[sub_idx][i]][j]+= (temp*temp)/2.;
			}
		}
	}

	fill(sigma,1.);
	for(int i=0;i<K;i++){
		for(int j=0;j<p;j++){
			if(sigma_beta[i][j]>0){
				sigma[i][j]=//sigma_beta[i][j]/sigma_alpha[i][j];
				1./rGamma(gen,sigma_alpha[i][j],sigma_beta[i][j]);
			}
		}
	}
	//cout<<mu<<endl;
	//cout<<sigma<<endl;
}

double HMM::Comp_Normal_aux(double x, double mu, double sigma){

	double temp=0;

	if(sigma>0){
	 temp=- 0.5 *(x-mu)*(x-mu)/sigma - 0.5*log(sigma);
	}
	return temp;
}

double HMM::Comp_Normal(vector<double> x, vector<double> mu,
							vector<double> sigma){
	double temp;
	temp=0.;
	for (int i = 0; i < x.size(); ++i)
	{
		temp+=Comp_Normal_aux(x[i],mu[i],sigma[i]);
	}

	return temp;
}

void HMM::Upd_S_aux(int idx, int sub_idx){

	std::vector<double> prob_multinominal;
	prob_multinominal.resize(K);
	
	if(idx==0){
		for (int k = 0; k < K; ++k)
			{
			prob_multinominal[k]= log(pi0[k])+
			+log( q[k][s[sub_idx][1]])+Comp_Normal(data[sub_idx][0],mu[k],sigma[k]);
			}
		}
	if(idx==N[sub_idx]-1){
		for (int k = 0; k < K; ++k)
			{
			prob_multinominal[k]= log(q[s[sub_idx][idx-1]][k])+
			+Comp_Normal(data[sub_idx][idx],mu[k],sigma[k]);
			}
	}
	if(idx<(N[sub_idx]-1)&&idx>0){
		for (int k = 0; k < K; ++k)
			{
			prob_multinominal[k]= log(q[s[sub_idx][idx-1]][k])+
			log(q[k][s[sub_idx][idx+1]])+
			+Comp_Normal(data[sub_idx][idx],mu[k],sigma[k]);
			}
	}
	//prob_multinominal=cumsum(normalize(prob_multinominal));

	prob_multinominal=cumsum(normalize(exp(plus_v(prob_multinominal,-min(prob_multinominal)))));

	//cout<<prob_multinominal<<endl;


	double U1=rUnif(gen);
	for(int k=0;k<K;++k){
		if(U1<prob_multinominal[k]){s[sub_idx][idx]=k;break;}
	}
}

void HMM::Upd_S(){
	for (int sub_idx = 0; sub_idx < N_sub; ++sub_idx){

		for (int i = 0; i < N[sub_idx]; ++i)
		{
			Upd_S_aux(i,sub_idx);
		}
	}
		Upd_Counts();
}


void HMM::Upd_Q(){
	

	fill(q[K-1],-1.);

	while(!compare_ge(q[K-1],0.)){
			for(int k=0;k<K-1;k++){
				q[k]=rDirichlet(gen,plus_v(q_count[k],1));
			}

			std::vector<double> temp=rep(0.,K);

			for(int k=0;k<K-1;k++){
				temp=plus_v(temp,q[k]);
			}

			q[K-1]=minus_v(rep(1.,K),temp);
		}
}


void HMM::Upd_pi0(){
	vector<int> pi0_alpha(K);
	fill(pi0_alpha,1);
	
	for(int sub_idx=0;sub_idx<N_sub;sub_idx++){
	pi0_alpha[s[sub_idx][0]]+=1;
	}
	//cout<<pi0_alpha<<endl;
	pi0=rDirichlet(gen,pi0_alpha);
}

void HMM::Update(){
  	Upd_Gaussian();
	Upd_S();
  	Upd_Q();
  	Upd_pi0();
}

void HMM::Burnin(int step){
	Upd_Counts();

	for (int i = 0; i < step; ++i)
	{
		Update();
	}
}


void HMM::Run(int step, int thin){
	for (int i = 0; i < step; ++i)
	{
		for (int j = 0; j < thin; ++j)
		{
			Update();
		}
		mu_trace.push_back(flatten(mu));
		sigma_trace.push_back(flatten(sigma));
		q_trace.push_back(flatten(q));
		pi0_trace.push_back(pi0);
		s_trace.push_back(flatten(s));
		loglik_trace.push_back(loglik());
	}
}

double HMM::loglik(){

	double pi0_sum=0.;


	for (int sub_idx = 0; sub_idx < N_sub; ++sub_idx){

		for(int i=0;i<N[sub_idx];i++){
			prob_gaussian[sub_idx][i]=Comp_Normal(data[sub_idx][i],mu[s[sub_idx][i]],sigma[s[sub_idx][i]]);
			if(i<N[sub_idx]-1){
			prob_q[sub_idx][i]=log(q[s[sub_idx][i]][s[sub_idx][i+1]]);
			}
		}

		pi0_sum+=log(pi0[s[sub_idx][0]]);
	}

	double loglik=sum(prob_gaussian)+sum(prob_q)+pi0_sum
					-sum(log(flatten(sigma)));

	return loglik;
}
