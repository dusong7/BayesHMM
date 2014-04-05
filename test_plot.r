#######################
# posterior
########################
setwd("~/Dropbox_Leo/Dropbox/HMM/ver0.2/")

mu0<-read.table("output/mu_trace_0.csv",header=F,sep=',')
mu0<-(mu0[,-ncol(mu0)])

require(lattice)


xyhist_data<-numeric()
for(i in 1:18){
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}

pdf('apple_state1.pdf',10,6)
histogram(~xyhist_data[,2]|as.factor(xyhist_data[,1]),as.table=T,
          breaks=10,col='blue',xlab='Emission Distribution')
dev.off()



mu0<-read.table("output/mu_trace_1.csv",header=F,sep=',')
mu0<-(mu0[,-ncol(mu0)])

require(lattice)


xyhist_data<-numeric()
for(i in 1:18){
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}

pdf('apple_state2.pdf',10,6)
histogram(~xyhist_data[,2]|as.factor(xyhist_data[,1]),as.table=T,
          breaks=10,col='blue',xlab='Emission Distribution')
dev.off()


##############################3
# markov kernel
##############################

mu0<-read.table("output/q_trace_0.csv",header=F,sep=',')
mu0<-(mu0[,-ncol(mu0)])



xyhist_data<-data.frame()
for(i in 1:ncol(mu0)){
  #start<-ceiling((i)/3)
  #label<-paste(start,'to',(i-(start-1)*3))
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}


pdf('q_dist.pdf',8,6)
histogram(~xyhist_data[,2]|as.factor(xyhist_data[,1]),as.table=T,
          breaks=10,col='blue',xlab='Transition Distribution')
dev.off()

###########################
#sigma
#####################3

mu0<-read.table("output/sigma_trace_0.csv",header=F,sep=',')
mu0<-(mu0[,-ncol(mu0)])

require(lattice)


xyhist_data<-numeric()
for(i in 1:18){
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}

pdf('apple_sigma.pdf',10,6)
histogram(~xyhist_data[,2]|as.factor(xyhist_data[,1]),as.table=T,
          breaks=10,col='blue',xlab='Emission Distribution')
dev.off()
##################################


###########################
#loglik
#####################3

mu0<-read.table("output/loglik_trace_0.csv",header=F,sep=',')

pdf('apple_loglik.pdf',10,6)
ts.plot(mu0$V1,ylab='loglik')
dev.off()

pdf('apple_acf.pdf',10,6)
acf(mu0$V1,ylab='loglik')
dev.off()
##################################

######
#state
#######
s0<-read.table("output/s_trace_0.csv",header=F,sep=',')
s0<-s0[,-ncol(s0)]

pdf('apple_s.pdf',10,6)
plot((as.numeric(s0[5000,])),type='p',axes=F,xlab='time index',ylab='state')
box()
axis(2,at=c(0,1,2),labels=c(0,1,2))
axis(1,at=seq(0,600,by=100),labels=seq(0,600,by=100))
dev.off()

