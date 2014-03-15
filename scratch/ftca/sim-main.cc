/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 Christopher Sang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <ctime>
#include <fstream>
#include <iostream>

#include "ns3/core-module.h"
#include "node-line-3.h"
#include "node-grid.h"
#include "node-disc.h"

#include "physim-collision-counter.h"
#include "physim-collision-log.h"
#include "physim-trace-counter-group.h"
#include "tcp-cwnd-log.h"

#include "progress-bar.h"
#include "sim-log.h"

#include <itpp/base/random.h> // for RNG seeding

using namespace ns3;

/*
 * Start the simulator
 */
void
DoSim ()
{
  SIM_LOG ("Setup complete. Running simulation now...");

  time_t startTime = time (NULL);
  Simulator::Run ();
  time_t endTime = time (NULL);

  Simulator::Destroy ();

  int elapsedMins = (int) ((endTime - startTime) / 60);
  SIM_LOG ("Simulation done.");
  SIM_LOG ("Wall time elapsed = " << elapsedMins << " minutes");
}

/*
 * Callback function for connecting to the TCP cwnd trace source at runtime.
 *
 * Note that trace source connection cannot be done before the simulator
 * is started, because the TCP sockets have not been created yet.
 */
void
TcpLogEnable (sim::TcpCwndLog *l, uint32_t n)
{
  l->Enable (n);
}

/*
 * Sets up a basic 3x3 grid of WLAN nodes with TCP upload connections.
 */
void
DoGridSim ()
{
  SIM_LOG ("Grid Simulation");
  SIM_LOG ("Performing setup...");

  sim::NodeGrid simulation (3, 5.0);
  simulation.Configure ();
  simulation.ConnectAllUplinkTcp (9000, 2.0, 32.0, 0.0);

  simulation.EnablePcapPromisc (simulation.GetApId ());

  sim::PhySimTraceCounterGroup<9> counters;
  counters.enableCounters ();
  sim::PhySimCollisionCounter<9> collisionCounters;
  collisionCounters.Enable ();

  std::ofstream cwndLogFiles[9];
  sim::TcpCwndLog cwndLogs[9];
  for (uint32_t i = 0; i < simulation.GetN (); i++)
    {
      if (i != simulation.GetServerId())
        {
          std::stringstream s;
          s << "tcp-cwnd-" << i << ".log";
          cwndLogFiles[i].open (s.str ().c_str (), std::ofstream::out);
          cwndLogs[i].SetFile (cwndLogFiles[i]);
          Simulator::Schedule (Seconds (2.1), TcpLogEnable, &(cwndLogs[i]), i);
        }
    }

  sim::ProgressBar progressBar (simulation.GetEndTime ());
  progressBar.Enable ();
  Simulator::Stop (Seconds (simulation.GetEndTime ()));

  DoSim ();

  // Output a summary of the results after the simulation has completed:
  std::ofstream counterFile ("physim-counters.log");
  for (uint32_t i = 0; i < simulation.GetN (); i++)
    {
      if (i != simulation.GetServerId ())
        {
          counters.printMainCounters (i, simulation.GetServerId (), counterFile);
          counters.printCounters (DpiTag::TCP_DATA, i, simulation.GetServerId (), counterFile);
          counters.printCounters (DpiTag::MAC_ACK, simulation.GetServerId (), i, counterFile);
          counters.printCounters (DpiTag::TCP_ACK, simulation.GetServerId (), i, counterFile);
          counterFile << "-----------------------------------\n";
          counterFile << std::endl;
        }
    }

  std::cout << "Tx/Rx counters (TCP DATA):" << std::endl;
  for (uint32_t i = 0; i < simulation.GetN (); i++)
    {
      if (i != simulation.GetServerId ())
        {
          counters.printCsvPacketCounters (DpiTag::TCP_DATA, i, simulation.GetServerId ());
        }
    }

  std::cout << "Tx/Rx counters (TCP ACK):" << std::endl;
  for (uint32_t i = 0; i < simulation.GetN (); i++)
    {
      if (i != simulation.GetServerId ())
        {
          counters.printCsvPacketCounters (DpiTag::TCP_ACK, simulation.GetServerId (), i);
        }
    }

  std::cout << "Collision counters (upstream):" << std::endl;
  for (uint32_t i = 0; i < simulation.GetN (); i++)
    {
      if (i != simulation.GetServerId ())
        {
          collisionCounters.PrintCounters (i, simulation.GetServerId ());
        }
    }

  std::cout << "Collision counters (downstream):" << std::endl;
  for (uint32_t i = 0; i < simulation.GetN (); i++)
    {
      if (i != simulation.GetServerId ())
        {
          collisionCounters.PrintCounters (simulation.GetServerId (), i);
        }
    }
}

int
main (int argc, char **argv)
{
  /*
   * Initialize random number generators.
   *
   * IMPORTANT: THERE ARE TWO SEPARATE RANDOM NUMBER GENERATORS IN USE
   *
   * To run multiple independent simulations:
   * ns3:  Fix the seed and increment the run number as per the documentation.
   * it++: Use a sequence of non-repeating seed values (e.g. time)
   */
  int64_t nsSeed = 4112645778LL;
  Config::SetGlobal ("RngSeed", IntegerValue (nsSeed));
  unsigned long itSeed = time (NULL);
  itpp::RNG_reset (itSeed);

  SIM_LOG ("RNG initialized with seeds: " << nsSeed << ", " << itSeed);

  IntegerValue nsRunVal;
  GlobalValue::GetValueByName ("RngRun", nsRunVal);

  SIM_LOG ("RNG Run: " << nsRunVal.Get ());

  Config::SetDefault ("ns3::TcpSocketBase::AckPriority", BooleanValue (true));
//  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", UintegerValue (100));
//  Config::SetDefault ("ns3::TcpSocket::DelAckTimeout", TimeValue (Seconds (0.0)));
//  Config::SetDefault ("ns3::TcpSocket::DelAckCount", UintegerValue (0));

  LogComponentEnableAll (LOG_LEVEL_ERROR);

  DoGridSim ();

  return 0;
}
