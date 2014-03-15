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

#include "node-system.h"

#include "sim-log.h"

NS_LOG_COMPONENT_DEFINE ("sim::NodeSystem");

namespace sim {

NodeSystem::NodeSystem () :
    m_endTime (0)
{
}

NodeSystem::~NodeSystem ()
{
}

void
NodeSystem::Configure ()
{
  MakeNodes ();
  ConfigurePhy ();
  ConfigureMac ();
  ConfigureDevices ();
  ConfigureLayout ();
  ConfigureIpv4 ();

//  for (int i = 0; i < m_nodes.GetN (); i++)
//    {
//      Ptr<Node> node = m_nodes.Get (i);
//      NS_LOG_UNCOND (node->GetId () << " " << node->GetDevice (0)->GetAddress ());
//    }
}

void
NodeSystem::ConnectAllDownlinkTcp (int basePort, double startTime, double endTime, double offsetTime, uint8_t qosTid)
{
  NS_ASSERT (startTime >= 1.0);
  NS_ASSERT (endTime > startTime);

  int port = basePort;
  double start = startTime;
  double end = endTime;

  for (uint32_t i = 0; i < m_nodes.GetN (); i++)
    {
      uint32_t server = GetServerId ();

      if (i != server)
        {
          ConnectTcp (server, i, port, start, end, qosTid);
          port++;
          start += offsetTime;
          end += offsetTime;
        }
    }
}

void
NodeSystem::ConnectAllUplinkTcp (int basePort, double startTime, double endTime, double offsetTime, uint8_t qosTid)
{
  NS_ASSERT (startTime >= 1.0);
  NS_ASSERT (endTime > startTime);

  int port = basePort;
  double start = startTime;
  double end = endTime;

  for (uint32_t i = 0; i < m_nodes.GetN (); i++)
    {
      uint32_t server = GetServerId ();

      if (i != server)
        {
          ConnectTcp (i, server, port, start, end, qosTid);
          port++;
          start += offsetTime;
          end += offsetTime;
        }
    }
}

void
NodeSystem::ConnectTcp (int sourceId, int destId, int port, double startTime,
    double endTime, uint8_t qosTid)
{
  NS_ASSERT (sourceId != destId);
  NS_ASSERT (startTime >= 1.0);
  NS_ASSERT (endTime > startTime);

  uint32_t maxBytes = 0;

  // Configure client application
  QosBulkSendHelper bulkSource ("ns3::TcpSocketFactory",
      InetSocketAddress (m_wifiInterfaces.GetAddress (destId), port));
  bulkSource.SetAttribute ("QosTid", UintegerValue (qosTid));
  bulkSource.SetAttribute ("MaxBytes", UintegerValue (maxBytes));

  // Install client application on the sender
  ApplicationContainer sourceApps = bulkSource.Install (m_nodes.Get (sourceId));

  sourceApps.Start (Seconds (startTime));
  sourceApps.Stop (Seconds (endTime));

  // Configure server application
  PacketSinkHelper packetSink ("ns3::TcpSocketFactory",
      InetSocketAddress (Ipv4Address::GetAny (), port));

  // Install server application on the receiver
  ApplicationContainer sinkApps = packetSink.Install (m_nodes.Get (destId));

  // start the server a bit early to make sure it is ready by the time we start transmitting
  sinkApps.Start (Seconds (startTime - 1.0));
  double serverEndTime = endTime + 1.0;
  sinkApps.Stop (Seconds (serverEndTime));
  if (serverEndTime > m_endTime)
    m_endTime = serverEndTime;
}

void
NodeSystem::ConnectUdp (int sourceId, int destId, int port, double startTime, double endTime)
{
  NS_ASSERT (sourceId != destId);
  NS_ASSERT (startTime >= 1.0);
  NS_ASSERT (endTime > startTime);

  UdpClientHelper source (m_wifiInterfaces.GetAddress (destId), port);
  source.SetAttribute ("MaxPackets", UintegerValue (4294967295UL));  // limit packets by time, not by count
  source.SetAttribute ("Interval", TimeValue (MilliSeconds (1)));

  ApplicationContainer sourceApps = source.Install (m_nodes.Get (sourceId));

  sourceApps.Start (Seconds (startTime));
  sourceApps.Stop (Seconds (endTime));

  UdpServerHelper sink (port);

  ApplicationContainer sinkApps = sink.Install (m_nodes.Get (destId));

  // start the server a bit early to make sure it is ready by the time we start transmitting
  sinkApps.Start (Seconds (startTime - 1.0));
  double serverEndTime = endTime + 1.0;
  sinkApps.Stop (Seconds (serverEndTime));
  if (serverEndTime > m_endTime)
    m_endTime = serverEndTime;
}

void
NodeSystem::EnablePcap ()
{
  SIM_LOG ("Traces enabled: pcap (non-promisc)");

  m_wifiPhy.EnablePcapAll (GetPcapFileName (), false);
}

void
NodeSystem::EnablePcap (int nodeId)
{
  SIM_LOG ("Traces enabled for node " << nodeId << ": pcap (non-promisc)");
  NodeContainer nc (m_nodes.Get (nodeId));
  m_wifiPhy.EnablePcap (GetPcapFileName (), nc, false);
}

void
NodeSystem::EnablePcapPromisc ()
{
  SIM_LOG ("Traces enabled: pcap (promisc)");

  m_wifiPhy.EnablePcapAll (GetPcapFileName (), true);
}

void
NodeSystem::EnablePcapPromisc (int nodeId)
{
  SIM_LOG ("Traces enabled for node " << nodeId << ": pcap (promisc)");
  NodeContainer nc (m_nodes.Get (nodeId));
  m_wifiPhy.EnablePcap (GetPcapFileName (), nc, true);
}

double
NodeSystem::GetEndTime () const
{
  return m_endTime + 0.5;
}

uint32_t
NodeSystem::GetN () const
{
  return m_nodes.GetN ();
}

void
NodeSystem::ConfigurePhy ()
{
  SIM_LOG ("Using PhySimWifi");

  // Configure PhySimWifiPhy
  double txpower = 18.0;
  Config::SetDefault ("ns3::PhySimWifiPhy::TxPowerEnd", DoubleValue (txpower) );
  Config::SetDefault ("ns3::PhySimWifiPhy::TxPowerStart", DoubleValue (txpower) );
  Config::SetDefault ("ns3::PhySimWifiPhy::TxPowerLevels", UintegerValue (1) );
//  Config::SetDefault ("ns3::PhySimInterferenceHelper::NoiseFloor", DoubleValue (-90.0) );
  //Config::SetDefault("ns3::PhySimWifiPhy::PacketCapture", BooleanValue(false));
  //Config::SetDefault("ns3::PhySimWifiPhy::CaptureThreshold", DoubleValue(4.0));

  m_wifiPhy = PhySimWifiPhyHelper::Default();

  PhySimWifiChannelHelper wifiChannel;
  wifiChannel.AddPropagationLoss ("ns3::PhySimPropagationLossModel");
  wifiChannel.AddPropagationLoss ("ns3::PhySimLogDistancePropagationLoss");
  Config::SetDefault ("ns3::PhySimLogDistancePropagationLoss::ReferenceDistance", DoubleValue (1.0) );
  Config::SetDefault ("ns3::PhySimLogDistancePropagationLoss::ReferenceLoss", DoubleValue (46.6777) );
  Config::SetDefault ("ns3::PhySimLogDistancePropagationLoss::Exponent", DoubleValue (3.5) );
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");

  m_wifiPhy.SetChannel(wifiChannel.Create());
}

void
NodeSystem::ConfigureMac ()
{
  m_wifiMac = QosWifiMacHelper::Default ();
  m_wifi = WifiHelper::Default ();
//  m_wifi.SetRemoteStationManager ("ns3::ArfWifiManager");
  m_wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
      "DataMode", StringValue ("OfdmRate54Mbps"));
}

void
NodeSystem::ConfigureIpv4 ()
{
  InternetStackHelper internet;
  internet.SetIpv4StackInstall (true);
  internet.SetIpv6StackInstall (false);
  internet.Install (m_nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  m_wifiInterfaces = address.Assign (m_devices);
}

} /* namespace sim */
