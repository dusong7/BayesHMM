#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

namespace boost {
namespace serialization {

template<class Archive>
  void serialize(Archive & ar, Posterior_Mode & mode, const unsigned int version)
  {	
      ar & mode.mu;
      ar & mode.sigma;
      ar & mode.q;
      ar & mode.pi0;
  }

  } // namespace serialization
} // namespace boost

template<class T>
void save(T obj,const string& filename)
{
 std::ofstream ofs(filename);

 boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
 oa << obj;
  // archive and stream closed when destructors are called
 //ofs.close();
}

template<class T>
void load(T& obj,const string& filename)
{       
        std::ifstream ifs(filename);
        boost::archive::text_iarchive ia(ifs);
        ia >> obj;
        //ifs.close();
  }  
