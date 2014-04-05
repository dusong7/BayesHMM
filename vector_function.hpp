using namespace std;

template <class T>
T sum(std::vector<T> v){
   T sum1=std::accumulate(v.begin(),v.end(),0.);
   return sum1;
}


template <class T>
T sum(vector<vector<T>> v){
  std::vector<T> temp(v.size());
  for (int i = 0; i < v.size(); ++i)
  {
    temp[i]=sum(v[i]);    
  }
   return sum(temp);
}


template <class T>
std::vector<T> scale(std::vector<T> v, T scalar){
  std::transform(v.begin(), v.end(), v.begin(),std::bind2nd(std::multiplies<T>(), scalar));
  return v;
}


template <class T>
std::vector<T> scale(std::vector<std::vector<T>> v, T scalar){
  for_each (v.begin(), v.end(),[&](std::vector<T>& e){e=scale(e,scalar);});
  return v;
}

template <class T>
std::vector<T> plus_v(std::vector<T> v, T scalar){
  std::transform(v.begin(), v.end(), v.begin(),std::bind2nd(std::plus<T>(), scalar));
  return v;
}

template <class T>
std::vector<T> plus_v(std::vector<T> v1, std::vector<T> v2){
  if(v1.size()==v2.size()){
    for (int i = 0; i < v1.size(); ++i){
      v2[i]=v1[i]+v2[i];
    }
  }
   return v2;
}

template <class T>
std::vector<T> minus_v(std::vector<T> v1, std::vector<T> v2){
  if(v1.size()==v2.size()){
    for (int i = 0; i < v1.size(); ++i){
      v2[i]=v1[i]-v2[i];
    }
  }
   return v2;
}

template <class T>
std::vector<T> exp(std::vector<T> v){
  for_each (v.begin(), v.end(),[](T& e){e=exp(e);});
  return v;
}

template <class T>
std::vector<T> log(std::vector<T> v){
  for_each (v.begin(), v.end(),[](T& e){e=log(e);});
  return v;
}

template <class T>
std::vector<T> normalize(std::vector<T> v){
    v=scale(v,1./sum(v));
    return v;
}

template <class T>
std::vector<T> cumsum(std::vector<T> v){
  std::partial_sum (v.begin(), v.end(), v.begin());
  return v;
}

template <class T>
std::vector<T> seq(T start, int length){
  std::vector<T> v(length);
  for(int i=0;i<length;i++){
    v[i]=(T)(start+i);
  }
  return v;
}


template <class T>
std::vector<T> shuffle(std::vector<T> v,shared_ptr<std::mt19937>  gen){
   std::shuffle(v.begin(), v.end(), *gen);
  return v;
}

template <class T>
std::vector<T> sample(std::vector<T> v, int n,shared_ptr<std::mt19937>  gen){
    v=shuffle(v,gen);
    std::vector<T> v1(v.begin(),v.begin()+n);
  return v1;
}


template <class T>
void fill(std::vector<T>& v, T k){
    std::fill(v.begin(),v.end(),k);
}


template <class T>
void fill(std::vector<std::vector<T>>& v, T k){
    for_each (v.begin(), v.end(),[&](vector<T>& e){fill(e,k);});
}

template <class T>
std::vector<T>  rep(T v, int N){
  std::vector<T> temp(N);
  fill(temp,v);
  return temp;
}

template <class T>
int count(std::vector<T> v, T k){
  return std::count (v.begin(), v.end(), k);
}


template <class T>
int count(std::vector<vector<T>> v, T k){
  std::vector<int> temp(v.size());
  for(int i=0;i<v.size();++i){
    temp[i]=count(v[i],k);
  }
  return sum(temp);
}

template <class T>
std::vector<T> sort(std::vector<T> v){
    std::sort(v.begin(),v.end());
    return v;
}


template <class T>
std::vector<T> unique(std::vector<T> v){
    std::vector<int>::iterator it;
    v=sort(v);
    it=std::unique(v.begin(),v.end());
    v.resize( std::distance(v.begin(),it) );
    return v;
  }


template <class T>
void  frequency( std::vector<T> v){
    int end=1000;
    int c;
    for(int i=0;i<end;i++){
      c=count(v,i);
      if(c>0){ cout <<  i <<" " <<c<<endl;}}
}



template <class T>
void  mat_resize( std::vector<std::vector<T>>& v, int m, int n){
    v.resize(m);
    for_each (v.begin(), v.end(),[&](vector<T>& e){e.resize(n);});
}



template <class T>
T max( std::vector<T> v){
return *std::max_element(v.begin(),v.end());
}

template <class T>
T min( std::vector<T> v){
return *std::min_element(v.begin(),v.end());
}

template <class T>
int  max_loc( std::vector<T> v){
return max_element(v.begin(), v.end()) - v.begin();
}

template <class T>
int  min_loc( std::vector<T> v){
return min_element(v.begin(), v.end()) - v.begin();
}

template <class T>
void  remove( std::vector<T> &v, T number){
v.erase(
std::remove_if(  v.begin(), v.end(), [&](T i){ return i == number;}),
v.end());
}

template <class T>
bool compare_ge( std::vector<T> v, T number){
    bool test=1;
    for (int i = 0; i < v.size(); ++i)
    {
      if(v[i]<number){test=0;break;}
    }
    return test;
}

template <class T>
std::vector<T>  combine( std::vector<T> A, std::vector<T> B){
  std::vector<T> AB;
  AB.reserve( A.size() + B.size() ); // preallocate memory
  AB.insert( AB.end(), A.begin(), A.end() );
  AB.insert( AB.end(), B.begin(), B.end() );
  return AB;
}

template <class T>
std::vector<T>  flatten( std::vector<std::vector<T>> A){
  std::vector<T> AB;
  AB.resize(0);
  for (int i = 0; i < A.size(); ++i)
  {
    AB=combine(AB,A[i]);
  }

  return AB;
}

