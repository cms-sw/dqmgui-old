#ifndef DQMSERVICES_CORE_STANDALONE_H
# define DQMSERVICES_CORE_STANDALONE_H
# if !WITHOUT_CMS_FRAMEWORK
#  include "FWCore/ParameterSet/interface/ParameterSet.h"
#  include "FWCore/ServiceRegistry/interface/ServiceRegistry.h"
#  include "FWCore/ServiceRegistry/interface/Service.h"
#  include "FWCore/MessageLogger/interface/JobReport.h"
#  include "FWCore/Version/interface/GetReleaseVersion.h"
#  include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
# else
#  include <memory>
#  include <string>
#  include <vector>
#  include <map>

namespace edm
{
  inline std::string getReleaseVersion(void)
  { return "CMSSW_STANDALONE"; }

  class ParameterSet
  {
    public:
    template <class T> static const T &
    getUntrackedParameter(const char * /* key */, const T &value)
    { return value; }
  };

  struct ServiceToken
  {
    ServiceToken(int) {}
  };

  class ServiceRegistry
  {
  public:
    struct Operate
    {
      Operate(const ServiceToken &) {}
    };

    static int createSet(const std::vector<ParameterSet> &) { return 0; }
  };

  template <class T>
  class  Service
  {
  public:
    bool isAvailable(void) { return false; }
    T *operator->(void)
    {
      static char buf[sizeof(T)]; static T *x;
      if (! x) x = new (buf) T(ParameterSet());
      return x;
    }
    T &operator*(void) { return * operator->(); }
  };

  namespace service {
   struct SystemBounds {
    unsigned int maxNumberOfStreams() const { return 0; }
  };
  }

  struct PreallocationSignal {
    template <typename T>
    void connect( T&& ) {};
  };

  class ActivityRegistry
  {
  public:
    template <typename T>
    void watchPostSourceRun(void*, T) {}

    template <typename T>
    void watchPostSourceLumi(void*, T) {}

    PreallocationSignal preallocateSignal_;
  };
  
  
  class JobReport
  {
  public:
    JobReport(const edm::ParameterSet &) {}
    void reportAnalysisFile(const std::string &, const std::map<std::string, std::string> &) {}
  };
}
# endif // WITHOUT_CMS_FRAMEWORK
#endif // DQMSERVICES_CORE_STANDALONE_H
