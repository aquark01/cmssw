#include "L1Trigger/L1TMuonEndCap/interface/DebugTools.h"
#include "DataFormats/L1TMuon/interface/L1TMuonSubsystems.h"

namespace emtf {

  void dump_fw_raw_input(const l1t::EMTFHitCollection& out_hits, const l1t::EMTFTrackCollection& out_tracks) {
    // from interface/Common.h
    constexpr int MIN_ENDCAP = 1;
    constexpr int MAX_ENDCAP = 2;
    constexpr int MIN_TRIGSECTOR = 1;
    constexpr int MAX_TRIGSECTOR = 6;

    for (int endcap = 1; endcap <= MAX_ENDCAP; ++endcap) {
    // for (int endcap = 2; endcap <= 2; ++endcap) {
      for (int sector = 1; sector <= MAX_TRIGSECTOR; ++sector) {
      // for (int sector = 2; sector <= 2; ++sector) {
        const int es = (endcap - MIN_ENDCAP) * (MAX_TRIGSECTOR - MIN_TRIGSECTOR + 1) + (sector - MIN_TRIGSECTOR);

        // _____________________________________________________________________
        // This prints the hits as raw text input to the firmware simulator
        // "12345" is the BX separator

        // std::cout << "==== Endcap " << endcap << " Sector " << sector << " Hits ====" << std::endl;
        // std::cout << "bx e s ss st vf ql cp wg id bd hs" << std::endl;

        bool empty_sector = true;
        for (const auto& h : out_hits) {
          if (h.Sector_idx() != es)
            continue;
          empty_sector = false;
        }

        for (int ibx = -3 - 5; (ibx < +3 + 5 + 5) && !empty_sector; ++ibx) {
          for (const auto& h : out_hits) {
            if (h.Subsystem() == L1TMuon::kCSC) {
              if (h.Sector_idx() != es)
                continue;
              if (h.BX() != ibx)
                continue;

              int bx = 0;
              int endcap = (h.Endcap() == 1) ? 1 : 2;
              int sector = h.PC_sector();
              int station = (h.PC_station() == 0 && h.Subsector() == 1) ? 1 : h.PC_station();
              int chamber = h.PC_chamber() + 1;
              int strip = (h.Station() == 1 && h.Ring() == 4) ? h.Strip() + 128 : h.Strip();  // ME1/1a
              int wire = h.Wire();
              int valid = 1;
              std::cout << "-------------------------------" << std::endl;
              std::cout << "CSC EMU" << std::endl;
              // std::cout << bx << " " << endcap << " " << sector << " " << h.Subsector() << " " << station << " "
              //           << valid << " " << h.Quality() << " " << h.Pattern() << " " << wire << " " << chamber << " "
              //           << h.Bend() << " " << strip << " " << h.Ring() << std::endl;

              // New format 08.06.2022
              int qs_es = h.Strip_quart_bit()*2+h.Strip_eighth_bit();

              std::cout << bx << " " << endcap << " " << sector << " " << h.Subsector() << " " << station << " "
                        << valid << " " << h.Quality() << " " << h.Pattern() << " " << wire << " " << chamber << " "
                        << h.Slope() << " " << strip << " " << 2*h.Strip_quart_bit() + h.Strip_eighth_bit() << std::endl;
              std::cout << "hit_phi: " << h.Phi_fp() << " theta: " << h.Theta_fp() << " zone_hit: " << h.Zone_hit() << " zone_code: " << h.Zone_code()
              << " phi_loc:" << h.Phi_loc() << " phi_glob: " << h.Phi_glob() << " theta: " << h.Theta() << std::endl;

            } else if (h.Subsystem() == L1TMuon::kRPC) {
              if (h.Sector_idx() != es)
                continue;
              if (h.BX() + 6 != ibx)
                continue;  // RPC hits should be supplied 6 BX later relative to CSC hits

              // Assign RPC link index. Code taken from src/PrimitiveSelection.cc
              int rpc_sub = -1;
              int rpc_chm = -1;
              if (!h.Neighbor()) {
                rpc_sub = ((h.Subsector_RPC() + 3) % 6);
              } else {
                rpc_sub = 6;
              }
              if (h.Station() <= 2) {
                rpc_chm = (h.Station() - 1);
              } else {
                rpc_chm = 2 + (h.Station() - 3) * 2 + (h.Ring() - 2);
              }

              int bx = 1;
              int endcap = (h.Endcap() == 1) ? 1 : 2;
              int sector = h.PC_sector();
              int station = rpc_sub;
              int chamber = rpc_chm + 1;
              int strip = (h.Phi_fp() >> 2);
              int wire = (h.Theta_fp() >> 2);
              int valid = 2;  // this marks RPC stub
              // std::cout << "RPC" << std::endl;
              // std::cout << bx << " " << endcap << " " << sector << " " << 0 << " " << station << " " << valid << " "
                        // << 0 << " " << 0 << " " << wire << " " << chamber << " " << 0 << " " << strip << std::endl;
            }
          }  // end loop over hits

          std::cout << "12345" << std::endl;
        }  // end loop over bx

        // _____________________________________________________________________
        // This prints the tracks as raw text output from the firmware simulator
        if (out_tracks.size() == 0) continue;
        // std::cout << "==== Endcap " << endcap << " Sector " << sector << " Tracks ====" << std::endl;
        // std::cout << "bx e s a mo et ph cr q pt" << std::endl;

        for (const auto& t : out_tracks) {
          if (t.Sector_idx() != es)
            continue;
            // EMTFPtLUT data = track.PtLUT();
          std::cout << "-------------------------------" << std::endl;
          std::cout << "Track EMU:" << std::endl;

          std::cout << "mode: " << t.Mode() << " phi_deltas: " << t.PtLUT().delta_ph[0] << " " << t.PtLUT().delta_ph[1] << " " << t.PtLUT().delta_ph[2] << " " << t.PtLUT().delta_ph[3]
          << " " << t.PtLUT().delta_ph[4] << " " << t.PtLUT().delta_ph[5] << " theta_deltas: " << t.PtLUT().delta_th[0] << " " << t.PtLUT().delta_th[1] << " " << t.PtLUT().delta_th[2]
          << " " << t.PtLUT().delta_th[3] << " " << t.PtLUT().delta_th[4] << " " << t.PtLUT().delta_th[5] << " track_phi: " << t.Phi_fp() << " track_theta: " << t.Theta_fp()
          << " address: " << t.PtLUT().address << std::endl;


          std::cout << t.BX() << " " << (t.Endcap() == 1 ? 1 : 2) << " " << t.Sector() << " " << t.PtLUT().address
                    << " " << t.Mode() << " " << (t.GMT_eta() >= 0 ? t.GMT_eta() : t.GMT_eta() + 512) << " "
                    << t.GMT_phi() << " " << t.GMT_charge() << " " << t.GMT_quality() << " " << t.Pt() << " " << t.Pt_dxy() << std::endl;
          std::cout << "-------------------------------" << std::endl;

        }  // end loop over tracks

      }  // end loop over sector
    }    // end loop over endcap
  }

}  // namespace emtf
