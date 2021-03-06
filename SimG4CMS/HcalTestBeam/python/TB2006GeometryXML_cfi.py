import FWCore.ParameterSet.Config as cms

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('Geometry/CMSCommonData/data/materials.xml', 
        'Geometry/CMSCommonData/data/rotations.xml', 
        'Geometry/HcalCommonData/data/hcalrotations.xml', 
        'SimG4CMS/HcalTestBeam/test/2006/TBHcal.xml', 
        'Geometry/HcalTestBeamData/data/TBHcal06BeamLine.xml', 
        'Geometry/HcalTestBeamData/data/TBHcalCable.xml', 
        'Geometry/HcalTestBeamData/data/2006/TBHcalBarrel.xml', 
        'Geometry/HcalTestBeamData/data/TBHcalEndcap.xml', 
        'Geometry/HcalTestBeamData/data/TBHcal06HcalOuter.xml', 
        'Geometry/HcalTestBeamData/data/ebcon.xml', 
        'Geometry/HcalTestBeamData/data/eregalgo.xml', 
        'Geometry/EcalCommonData/data/ebrot.xml', 
        'Geometry/EcalCommonData/data/ebNoAPD/ebalgo.xml', 
        'Geometry/HcalTestBeamData/data/TBHcal06Sens.xml', 
        'Geometry/HcalTestBeamData/data/TBHcal06ebsens.xml', 
        'Geometry/HcalTestBeamData/data/TBHcal06ProdCuts.xml', 
        'Geometry/EcalSimData/data/EBProdCuts.xml', 
        'Geometry/HcalTestBeamData/data/TBHcal06Util.xml'),
    rootNodeName = cms.string('TBHcal:OTBHCal')
)


