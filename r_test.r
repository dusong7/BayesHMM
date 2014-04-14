setwd( "~/Dropbox_Leo/Dropbox/HMM/ver0.3")

source("./wave_process.r")

train_data<-numeric()
N<-numeric()

for(i in 1:12){
  ip<-i
  if(i<10){ip<-paste(0,i,sep='')}  
  data1<-wave_process(file=paste('../audio/apple/apple',ip,'.wav',sep=''))
  N<-c(N,nrow(data1))
  train_data<-rbind(train_data,data1)  
}


source("./train_and_test.r")


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


  
test_margin<-comp_marginal(test,N, test_data,  3, 5000, 5000, 10)
hist(test_margin$trace_loglik)
              