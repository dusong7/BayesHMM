require(tuneR)
require(e1071)


buffer<-function(raw,framesize, overlap){
  size<-floor(length(raw)/(framesize-overlap))+1
  b=matrix(raw,(framesize-overlap),size)
  b[(length(raw)+1):length(b)]<-0
  
  overlap_part<-cbind(0,b[(framesize-overlap-overlap+1):(framesize-overlap),])
  header<-overlap_part[,-ncol(overlap_part)]
  b<-rbind(header,b)
  
  return(b);
}

nextpow2<-function(n){
  i=0
  while(2^i<n){
    i=i+1
  }
  return(i)
}

peak_extract<-function(f,x){
  peaks<-numeric()
  
  for(i in 2:(length(x)-1)){
    if(x[i]>x[i-1]&&x[i]>x[i+1]){
      peaks<-rbind(peaks,c(f[i],x[i]))
    }  
  }
  return(peaks)
}

fft_new<-function(x,N){
  j<-complex(real=0,im=1)
  X=x
  for(k in 1:length(x)){
    X[k]=0
  for(n in 1:min(length(x),N)){
      X[k] = X[k] + x[n]*exp(-j*2*pi*(k-1)*(n-1)/N)
      }
    }
  return (X)
}


feature_extract<-function(sound){
  Fs = 8000;
  framesize = 80;
  overlap = 20;
  D = 6;
  w=hamming.window(framesize)
  frames = buffer(sound, framesize, overlap);
  
  NFFT = 2^nextpow2(framesize);
  
  feature<-numeric()
  
  for(i in 1:ncol(frames)){
    x=frames[,i]*w
    X=fft_new(x,NFFT)/framesize;
    f = Fs/2*seq(0, 1, length.out=NFFT/2 + 1)
    y=abs(X[1:(NFFT/2 + 1)])
    peaks<-peak_extract(f,y)
    peaks<-peaks[order(peaks[,2],decreasing=T),]
    if(length(peaks)<D*2){peaks<-rbind(peaks,matrix(1,nrow=D-length(peaks)/2,ncol=2))}
    else{peaks<-peaks[1:D,]}
    feature<-rbind(feature,peaks[,1])
  }
  return(feature)
}

wave_process<-function(file){
  raw<-readWave(file)
  raw_data<-raw@left
  raw_data<-raw_data/32768 #normalization
    raw_psd<-feature_extract(raw_data)
  return(raw_psd)  
}

