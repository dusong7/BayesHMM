test_loglik<-read.table("output/test_loglik.csv",header=F,sep=',')
ts.plot(test_loglik$V1)
hist(test_loglik$V1)