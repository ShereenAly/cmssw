// livio.fano@cern.ch
#ifndef BHFILTER_H
#define BHFILTER_H


#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
//#include "AnalysisExamples/SiStripDetectorPerformance/interface/CosmicGenFilter.h"
#include "FastSimulation/BaseParticlePropagator/interface/BaseParticlePropagator.h"

#include <map>
#include <vector>

namespace cms
//class TTree;
{
class BHFilter : public edm::EDFilter {
  public:
  explicit BHFilter(const edm::ParameterSet& conf);
  virtual ~BHFilter() {}
  //   virtual bool filter(edm::Event & e, edm::EventSetup const& c);
  bool filter(edm::Event & iEvent, edm::EventSetup const& c);
  bool BSC1(HepMC::FourVector,  HepMC::FourVector, HepMC::FourVector);

 private:
  edm::ParameterSet conf_;

  bool inTK;
  std::vector<double> zBounds;
  std::vector<double> rBounds;
  std::vector<double> bFields;
  double bReduction;
  int trig_;
  int trig2_;


  bool pad_plus;
  bool pad_minus;
  bool circ_plus;
  bool circ_minus;
};

}

#endif 
