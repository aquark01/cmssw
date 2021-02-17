#ifndef L1TMuonEndCap_L1TMuonEndCapTrackProducer_h
#define L1TMuonEndCap_L1TMuonEndCapTrackProducer_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/L1TMuonEndCap/interface/TrackFinder.h"
#include "L1Trigger/L1TMuonEndCap/interface/MicroGMTConverter.h"
#include "L1Trigger/L1TMuonEndCap/interface/Common.h"


// Class declaration
class L1TMuonEndCapTrackProducer : public edm::stream::EDProducer<edm::GlobalCache<L1TMuonEndCapCache>> {
public:
  explicit L1TMuonEndCapTrackProducer(const edm::ParameterSet&, const L1TMuonEndCapCache *cache);
  ~L1TMuonEndCapTrackProducer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  // two additional static methods for handling the global cache
  static std::unique_ptr<L1TMuonEndCapCache> initializeGlobalCache(const edm::ParameterSet&);
  static void globalEndJob(const L1TMuonEndCapCache*);

private:
  void produce(edm::Event&, const edm::EventSetup&) override;

  //void beginJob() override;
  //void endJob() override;
  //void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //void endRun(edm::Run const&, edm::EventSetup const&) override;
  //void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

private:
  std::unique_ptr<TrackFinder> track_finder_;
  std::unique_ptr<MicroGMTConverter> uGMT_converter_;

  const edm::ParameterSet& config_;
};

#endif
