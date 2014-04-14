setwd( "~/Dropbox_Leo/Dropbox/HMM/ver0.3")

source("./wave_process.r")

fruit_name<-c("apple","banana","lime","orange","peach","pineapple")

data_list<-list()
N_list<-list()

if(FALSE){
          for(k in 1:length(fruit_name)){
          
          fruit<-fruit_name[k]
          train_data<-numeric()
          N<-numeric()
          
          for(i in 1:10){
            ip<-i
            if(i<10){ip<-paste(0,i,sep='')}  
            data1<-wave_process(file=paste('../audio/',fruit,"/",fruit,ip,'.wav',sep=''))
            N<-c(N,nrow(data1))
            train_data<-rbind(train_data,data1)  
          }
          
          data_list[[k]]<-train_data
          N_list[[k]]<-N
        }
}

#save(file='data_list.RDa',data_list)
#save(file='N_list.RDa',N_list)

load(file='data_list.RDa')
load(file='N_list.RDa')
source("./train_and_test.r")


###########################
#training
######################
sound_lib<-list()

for(k in 1:length(fruit_name)){
  sound_lib[[k]]<-train(data_list[[k]], N_list[[k]], 3, 5000, 1000, 10)
  print(paste(k,"finished"))
}

save(file='sound_lib.RDa',sound_lib)
#load(file='sound_lib.RDa')

######################################3

test_list<-list()
test_N_list<-list()

for(k in 1:length(fruit_name)){
  
  fruit<-fruit_name[k]
  test_data<-numeric()
  N<-numeric()
  
  for(i in 15:15){
    ip<-i
    if(i<10){ip<-paste(0,i,sep='')}  
    data1<-wave_process(file=paste('../audio/',fruit,"/",fruit,ip,'.wav',sep=''))
    N<-c(N,nrow(data1))
    test_data<-rbind(test_data,data1)  
  }
  
  test_list[[k]]<-test_data
  test_N_list[[k]]<-N
}
  

soundmatch<-function(j){
  marginal_list<-list()
  for(k in 1:length(fruit_name)){
  marginal_list[[k]]<-comp_marginal(sound_lib[[k]],test_N_list[[j]], test_list[[j]],  3, 5000, 1000, 10)  }
  
  bayes_factor<-numeric()
  for(k in 1:length(fruit_name)){
    bayes_factor<-c(bayes_factor,(marginal_list[[k]]$marginal_loglik-marginal_list[[1]]$marginal_loglik))
  }
return(bayes_factor)
}

fruit_bf<-numeric()

for(k in 1:length(fruit_name)){
fruit_bf<-rbind(fruit_bf,soundmatch(k))
}

print(fruit_bf)



########################3
test_data<-wave_process(file=paste('../test_data/orange.wav'))
N<-c(nrow(test_data))

marginal_list<-list()
for(k in 1:length(fruit_name)){
  marginal_list[[k]]<-comp_marginal(sound_lib[[k]],N, test_data,  3, 1000, 1000, 10)  }

for(k in 1:length(fruit_name)){
  print(marginal_list[[k]]$marginal_loglik)
}



#################################
require(lattice)

mu0<-sound_lib[[4]]$trace$mu

xyhist_data<-numeric()
for(i in 1:18){
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}

pdf('orange_mu.pdf',10,6)
histogram(~xyhist_data[,2]|as.factor(xyhist_data[,1]),as.table=T,
          breaks=10,col='blue',type="count",xlab='Emission Distribution')
dev.off()


#################################
require(lattice)

mu0<-sound_lib[[1]]$trace$mu

xyhist_data<-numeric()
for(i in 1:18){
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}

pdf('apple_mu.pdf',10,6)
histogram(~as.numeric(xyhist_data[,2])|as.factor(xyhist_data[,1]),as.table=T,breaks=10,col='blue',xlab='Emission Distribution',,type="count")
dev.off()

#################################
require(lattice)

mu0<-sound_lib[[1]]$trace$sigma

xyhist_data<-numeric()
for(i in 1:18){
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}

pdf('../final_report/apple_sigma.pdf',10,6)
histogram(~as.numeric(xyhist_data[,2])|as.factor(xyhist_data[,1]),as.table=T,breaks=10,col='blue',xlab='Emission Distribution',,type="count")
dev.off()




#################################
require(lattice)

mu0<-sound_lib[[1]]$trace$q

xyhist_data<-numeric()
for(i in 1:9){
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}

pdf('../final_report/apple_q.pdf',6,6)
histogram(~as.numeric(xyhist_data[,2])|as.factor(xyhist_data[,1]),as.table=T,breaks=10,col='blue',xlab='Transition Distribution',,type="count")
dev.off()



#################################
require(lattice)

mu0<-sound_lib[[4]]$trace$q

xyhist_data<-numeric()
for(i in 1:9){
  xyhist_data<-rbind(xyhist_data,cbind(i,mu0[,i]))
}

pdf('../final_report/orange_q.pdf',6,6)
histogram(~as.numeric(xyhist_data[,2])|as.factor(xyhist_data[,1]),as.table=T,breaks=10,col='blue',xlab='Transition Distribution',,type="count")
dev.off()

pdf("../final_report/apple_acf.pdf",8,6)
acf(main="Autocorrelation of mu",sound_lib[[1]]$trace$mu[,1])
dev.off()