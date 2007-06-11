// Package:    SiPixelMonitorTrack
// Class:      SiPixelMonitorTrackResiduals
// 
// class SiPixelMonitorTrackResiduals SiPixelMonitorTrackResiduals.cc 
//       DQM/SiPixelMonitorTrack/src/SiPixelMonitorTrackResiduals.cc
//
// Description: SiPixel hit-to-track residual data quality monitoring modules
// Implementation: prototype -> improved -> never final - end of the 1st step 
//
// Original Author: Shan-Huei Chuang
//         Created: Fri Mar 23 18:41:42 CET 2007
// $Id: SiPixelMonitorTrackResiduals.cc,v 1.3 2007/05/24 06:11:46 schuang Exp $


#include <boost/cstdint.hpp>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"
#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "DQM/SiPixelCommon/interface/SiPixelFolderOrganizer.h"
#include "DQM/SiPixelMonitorTrack/interface/SiPixelMonitorTrackResiduals.h"


using namespace std;
using namespace edm;


SiPixelMonitorTrackResiduals::SiPixelMonitorTrackResiduals(const edm::ParameterSet& iConfig) {
  dbe_ = edm::Service<DaqMonitorBEInterface>().operator->();
  conf_ = iConfig;
}


SiPixelMonitorTrackResiduals::~SiPixelMonitorTrackResiduals() {
}


void SiPixelMonitorTrackResiduals::beginJob(edm::EventSetup const& iSetup) {
  std::cout << " *** SiPixelMonitorTrackResiduals " << std::endl;

  edm::ESHandle<TrackerGeometry> pDD;
  iSetup.get<TrackerDigiGeometryRecord>().get(pDD);

  std::cout <<" *** Geometry node for TrackerGeom is " << &(*pDD) << std::endl;
  std::cout <<" *** " << pDD->dets().size() <<" detectors; "
                      << pDD->detTypes().size() <<" types" << std::endl;
  
  for (TrackerGeometry::DetContainer::const_iterator it = pDD->dets().begin(); it!=pDD->dets().end(); it++) {
    if (dynamic_cast<PixelGeomDetUnit*>((*it))!=0) {
      DetId detId = (*it)->geographicalId();

      if (detId.subdetId()==static_cast<int>(PixelSubdetector::PixelBarrel)) {
	uint32_t id = detId();
	SiPixelResidualModule* module = new SiPixelResidualModule(id);
	thePixelStructure.insert(pair<uint32_t, SiPixelResidualModule*> (id, module));
      }	
      else if (detId.subdetId()==static_cast<int>(PixelSubdetector::PixelEndcap)) {
	uint32_t id = detId();
	SiPixelResidualModule* module = new SiPixelResidualModule(id);
	thePixelStructure.insert(pair<uint32_t, SiPixelResidualModule*> (id, module));
      }
    }
  }
  std::cout << " *** size of thePixelStructure is " << thePixelStructure.size() << std::endl; 
  dbe_->setVerbose(0);

  // create a folder tree and book histograms 
  // 
  SiPixelFolderOrganizer theSiPixelFolder;
  std::map<uint32_t, SiPixelResidualModule*>::iterator struct_iter;
  for (struct_iter = thePixelStructure.begin(); struct_iter!=thePixelStructure.end(); struct_iter++) {
    if (theSiPixelFolder.setModuleFolder((*struct_iter).first)) (*struct_iter).second->book(conf_);
    else throw cms::Exception("LogicError") << " *** creation of SiPixelMonitorTrackResiduals folder failed"; 
  }
  /* dbe_->setCurrentFolder("Tracker"); 
  char hisID[80]; 
  for (int s=0; s<3; s++) {
    sprintf(hisID,"residual_x_subdet_%i",s); 
    meSubpixelResidualX[s] = dbe_->book1D(hisID,"Hit-to-Track Residual in X",500,-5.,5.);
    
    sprintf(hisID,"residual_y_subdet_%i",s); 
    meSubpixelResidualY[s] = dbe_->book1D(hisID,"Hit-to-Track Residual in Y",500,-5.,5.);  
  } */
}


void SiPixelMonitorTrackResiduals::endJob(void) {
  dbe_->showDirStructure();
  bool outputMEsInRootFile = conf_.getParameter<bool>("OutputMEsInRootFile");
  std::string outputFileName = conf_.getParameter<std::string>("OutputFileName");
  if (outputMEsInRootFile) dbe_->save(outputFileName);
}


void SiPixelMonitorTrackResiduals::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  std::string TrackCandidateLabel = conf_.getParameter<std::string>("TrackCandidateLabel");
  std::string TrackCandidateProducer = conf_.getParameter<std::string>("TrackCandidateProducer");

  ESHandle<TrackerGeometry> theRG;
  iSetup.get<TrackerDigiGeometryRecord>().get(theRG);
  
  ESHandle<MagneticField> theRMF;
  iSetup.get<IdealMagneticFieldRecord>().get(theRMF);
  
  ESHandle<TransientTrackingRecHitBuilder> theBuilder;
  iSetup.get<TransientRecHitRecord>().get("WithTrackAngle",theBuilder);
  
  ESHandle<TrajectoryFitter> theRFitter;
  iSetup.get<TrackingComponentsRecord>().get("KFFittingSmoother",theRFitter);
 
  const TransientTrackingRecHitBuilder* builder = theBuilder.product();
  const TrackerGeometry* theG = theRG.product();
  const MagneticField* theMF = theRMF.product();
  const TrajectoryFitter* theFitter = theRFitter.product();

  Handle<TrackCandidateCollection> trackCandidateCollection;
  iEvent.getByLabel(TrackCandidateProducer, TrackCandidateLabel, trackCandidateCollection);

  for (TrackCandidateCollection::const_iterator track = trackCandidateCollection->begin(); 
       track!=trackCandidateCollection->end(); ++track) {
    const TrackCandidate* theTC = &(*track);
    PTrajectoryStateOnDet state = theTC->trajectoryStateOnDet();
    const TrackCandidate::range& recHitVec = theTC->recHits();
    const TrajectorySeed& seed = theTC->seed();
    std::cout <<" with "<< (int)(recHitVec.second - recHitVec.first) <<" hits "<< std::endl;

    TrajectoryStateTransform transformer; // to convert PTrajectoryStateOnDet to TrajectoryStateOnSurface

    DetId detId(state.detId());
    TrajectoryStateOnSurface theTSOS = transformer.transientState(state, &(theG->idToDet(detId)->surface()), theMF);

    Trajectory::RecHitContainer hits;
    
    TrackingRecHitCollection::const_iterator hit;
    for (hit = recHitVec.first; hit!=recHitVec.second; ++hit) hits.push_back(builder->build(&(*hit)));

    std::vector<Trajectory> trajVec = theFitter->fit(seed, hits, theTSOS);
    std::cout <<" fitted candidate with "<< trajVec.size() <<" tracks "<< std::endl;

    if (trajVec.size()!=0) {
      const Trajectory& theTraj = trajVec.front();

      Trajectory::DataContainer fits = theTraj.measurements();
      for (Trajectory::DataContainer::iterator fit = fits.begin(); fit!=fits.end(); fit++) {
        const TrajectoryMeasurement tm = *fit;
        TrajectoryStateOnSurface theCombinedPredictedState = TrajectoryStateCombiner().combine(tm.forwardPredictedState(),
	                                                                                       tm.backwardPredictedState());
        TransientTrackingRecHit::ConstRecHitPointer hit = tm.recHit();	
        const GeomDet* det = hit->det();
        	
        if (det->components().empty() && (det->subDetector()==GeomDetEnumerators::PixelBarrel ||
                			  det->subDetector()==GeomDetEnumerators::PixelEndcap)) {
          const GeomDetUnit* du = dynamic_cast<const GeomDetUnit*>(det);
          const Topology* theTopol = &(du->topology());
	  
          if (hit->isValid() && theCombinedPredictedState.isValid()) { 
	    MeasurementPoint theMeasHitPos = theTopol->measurementPosition(hit->localPosition());
	    MeasurementPoint theMeasStatePos = theTopol->measurementPosition(theCombinedPredictedState.localPosition());
	    Measurement2DVector hitResidual = theMeasHitPos - theMeasStatePos;
	    
	    DetId hit_detId = hit->geographicalId();
	    int IntRawDetID = hit_detId.rawId();
	    std::map<uint32_t, SiPixelResidualModule*>::iterator struct_iter = thePixelStructure.find(IntRawDetID);
	    if (struct_iter!=thePixelStructure.end()) (*struct_iter).second->fill(hitResidual);
	  
	    /* if (det->subDetector()==GeomDetEnumerators::PixelEndcap) {
              PXFDetId pxf(hit_detId);
              meSubpixelResidualX[pxf.side()]->Fill(hitResidual.x());
	      meSubpixelResidualY[pxf.side()]->Fill(hitResidual.y());	  
	    }
	    else {
              meSubpixelResidualX[0]->Fill(hitResidual.x());
	      meSubpixelResidualY[0]->Fill(hitResidual.y());	  
	    } */
	  }
     	}
      }
    }
  } 
}


DEFINE_FWK_MODULE(SiPixelMonitorTrackResiduals); // define this as a plug-in
