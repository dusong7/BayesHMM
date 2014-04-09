

std::uniform_real_distribution<> rUnif_rng(0, 1); //generate uniform by rUnif(gen)
//std::gamma_distribution<> rGamma_rng(1., 1.);
std::normal_distribution<> rNormal_rng(0., 1.); 

std::uniform_int_distribution<> rUnif_int_rng(0, 6);


double rUnif(shared_ptr<std::mt19937>  gen){
  return rUnif_rng(*gen);
}

double  rGamma(shared_ptr<std::mt19937>  gen, double alpha,
double beta){
	std::gamma_distribution<> rGamma_rng(alpha, 1./beta);
  	return rGamma_rng(*gen);
}

double rNormal(shared_ptr<std::mt19937>  gen, double mu,
double sigma){
  	return rNormal_rng(*gen)*sqrt(sigma)+mu;
}


vector<double>  rDirichlet(shared_ptr<std::mt19937>  gen,vector<int> alpha ){

  int N=alpha.size();

  std::vector<double> vD(N);
  for(int i=0;i<N;i++){
  		  vD[i]= rGamma(gen,(double)alpha[i], 1.);
  }
  double sum1=sum(vD);
  vD=scale(vD,1./sum1);

  return vD;

}