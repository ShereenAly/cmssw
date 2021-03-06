//
// Package:         RecoTracker/RoadSearchHelixMaker/test
// Class:           RoadSearchTrackReaderAlgorithm
// 
// Description:     read and analyze Tracks
//
// Original Author: Oliver Gutsche, gutsche@fnal.gov
// Created:         Wed Mar 29 20:00:00 UTC 2006
//
// $Author: gutsche $
// $Date: 2007/01/15 22:16:28 $
// $Revision: 1.3 $
//

#include <vector>
#include <algorithm>
#include <iostream>

#include "RecoTracker/RoadSearchHelixMaker/test/RoadSearchTrackReaderAlgorithm.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

RoadSearchTrackReaderAlgorithm::RoadSearchTrackReaderAlgorithm(const edm::ParameterSet& conf) : conf_(conf) { 
}

RoadSearchTrackReaderAlgorithm::~RoadSearchTrackReaderAlgorithm() {
}


void RoadSearchTrackReaderAlgorithm::run(const reco::TrackCollection* input)
{
  
  edm::LogInfo("RoadSearch") << "number of Tracks: " << input->size();
  
  for ( reco::TrackCollection::const_iterator track = input->begin(); track != input->end(); ++track ) {
    edm::LogInfo("RoadSearch") << "Perigee-Parameter: qoverp: " << track->qoverp();
    edm::LogInfo("RoadSearch") << "Perigee-Parameter: theta: " << track->theta();
    edm::LogInfo("RoadSearch") << "Perigee-Parameter: lambda: " << track->lambda();
    edm::LogInfo("RoadSearch") << "Perigee-Parameter: d0: " << track->d0();
    edm::LogInfo("RoadSearch") << "Perigee-Parameter: dz: " << track->dz();
  }
  
}
