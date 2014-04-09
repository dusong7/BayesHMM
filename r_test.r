setwd( "/media/data/Dropbox_Leo/Dropbox/HMM/ver0.3")

source("./wave_process.r")

train_data<-numeric()
N<-numeric()

for(i in 1:12){
  ip<-i
  if(i<10){ip<-paste(0,i,sep='')}  
  data1<-wave_process(file=paste('../hmm-speech-recognition-0.1/audio/apple/apple',ip,'.wav',sep=''))
  N<-c(N,nrow(data1))
  train_data<-rbind(train_data,data1)  
}




train<-function(train_data, N, R_K=3, R_burnin, R_step, R_thin){

R_N_sub=length(N)
R_p=ncol(train_data)
R_N=N

dyn.load('./train.so')

train_HMM<-.C("train_HMM",
  X_arr=as.numeric(t(train_data)),
  N_sub=as.integer(R_N_sub),
  N=as.integer(R_N),
  p=as.integer(R_p),
  K=as.integer(R_K),
  burin=as.integer(R_burin),
  step=as.integer(R_step),
  thin=as.integer(R_thin),
  
  trace_mu=as.double( vector("numeric",R_step*R_K*R_p)),
  trace_sigma=as.double( vector("numeric",R_step*R_K*R_p)),
  trace_q=as.double( vector("numeric",R_step*R_K*R_K)),
  trace_pi0=as.double( vector("numeric",R_step*R_K)),
  trace_loglik=as.double( vector("numeric",R_step)),
  
  best_mu=as.double( vector("numeric",R_K*R_p)),
  best_sigma=as.double( vector("numeric",R_K*R_p)),
  best_q=as.double( vector("numeric",R_K*R_K)),
  best_pi0=as.double( vector("numeric",R_K)),
  best_loglik=as.double( vector("numeric",1))
 )

trace_mu<-matrix(train_HMM$trace_mu,ncol=R_K*R_p,byrow=T)
trace_sigma<-matrix(train_HMM$trace_sigma,ncol=R_K*R_p,byrow=T)
trace_q<-matrix(train_HMM$trace_q,ncol=R_K*R_K,byrow=T)
trace_pi0<-matrix(train_HMM$trace_pi0,ncol=R_K,byrow=T)
trace_loglik<-train_HMM$trace_loglik

mu<-colMeans(trace_mu)
sigma<-colMeans(trace_sigma)
q<-colMeans(trace_q)
pi0<-colMeans(trace_pi0)


result<-list("trace"=list("mu"=trace_mu,"sigma"=trace_sigma,"q"=trace_q,"pi0"=trace_pi0,"loglik"=trace_loglik),"Posterior_Mean"=list("mu"=mu,"sigma"=sigma,"q"=q,"pi0"=pi0))

return(result)

}

test<-train(train_data, N, 3, 5000, 5000, 10)



######################################3

test_data<-numeric()
N<-numeric()

for(i in 1:1){
  ip<-i
  if(i<10){ip<-paste(0,i,sep='')}  
  data1<-wave_process(file=paste('../hmm-speech-recognition-0.1/audio/apple/apple',ip,'.wav',sep=''))
  N<-c(N,nrow(data1))
  test_data<-rbind(test_data,data1)  
}


  
  
comp_marginal<-function(HMMobj,N,test_data, R_K,R_burnin, R_step, R_thin){
  
  mu=HMMobj$Posterior_Mean$mu
  sigma=HMMobj$Posterior_Mean$sigma
  q=HMMobj$Posterior_Mean$q
  pi0=HMMobj$Posterior_Mean$pi0
  
  R_N_sub=length(N)
  R_p=ncol(test_data)
  R_N=N
  
dyn.load('./comp_maginal.so')

test_HMM<-.C("test_HMM",
              X_arr=as.numeric(t(test_data)),
              N_sub=as.integer(R_N_sub),
              N=as.integer(R_N),
              p=as.integer(R_p),
              K=as.integer(R_K),
              burin=as.integer(R_burin),
              step=as.integer(R_step),
              thin=as.integer(R_thin),
              
              
              best_mu=as.numeric(mu),
              best_sigma=as.numeric(sigma),
              best_q=as.numeric(q),
              best_pi0=as.numeric(pi0),
              trace_loglik=as.double( vector("numeric",R_step)),
              marginal_loglik=as.double( vector("numeric",1)))

result<-list("trace_loglik"=test_HMM$trace_loglik,"marginal_loglik"=test_HMM$marginal_loglik)

return(result)

}

test_margin<-comp_marginal(test,N, test_data,  3, 5000, 5000, 10)
hist(test_margin$trace_loglik)
              