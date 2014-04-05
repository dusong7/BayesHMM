
#include <boost/tokenizer.hpp>

using namespace std;


template <class T>
ostream&  operator << (ostream& os, vector<T> mat){

    for(int i=0;i<mat.size();i++){
                os <<  mat[i]<<endl;//",";
            }
    return os;
}

template <class T>
ostream&  operator << (ostream& os, vector<vector<T>> mat){

    for(int i=0;i<mat.size();i++){
            for(int j=0;j<mat[i].size();j++){
                os <<  mat[i][j]<<",";}
                os<<endl;}
    return os;
}


string load_file(const string& filename)
{
    ifstream infile(filename.c_str(), ios::binary);
    istreambuf_iterator<char> begin(infile), end;
    return string(begin, end);
}


std::vector<double> readCSV(const string& filename){

    std::vector<double> v;

    typedef boost::tokenizer<boost::char_separator<char> > 
    tokenizer;
    
    string str=load_file(filename);

    boost::char_separator<char> sep(",\n");
    tokenizer tokens(str, sep);

    for(const auto& t : tokens){
    //for (tokenizer::iterator tok_iter = tokens.begin();
      // tok_iter != tokens.end(); ++tok_iter){
          v.push_back(atof(t.c_str()));
       }

    return v;
}


std::vector<std::vector<double>> readCSV2(const string& filename){

    std::vector<double> v=readCSV(filename);

    std::string line;
    ifstream infile2(filename.c_str(), ios::binary);
    std::getline(infile2, line);
    int n_col=std::count(line.begin(),line.end(),',')+1;
    int N=v.size();
    int n_row=N/n_col;

    //if(n_col==1){return v;}
    std::vector<std::vector<double>> v2;
    std::vector<double> v2_col;

    if(n_col>1)
    {
      for(int i=0;i<n_row;i++){
        v2_col.resize(0);
        for(int j=0;j<n_col;j++){
          v2_col.push_back(v[i*n_col+j]);
        }
        //cout<<v2_col<<endl;
        v2.push_back(v2_col);
      }
    }

    return v2;
}
