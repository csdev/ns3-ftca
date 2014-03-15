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

#include "node-grid.h"

#include "sim-log.h"

NS_LOG_COMPONENT_DEFINE ("sim::NodeGrid");

namespace sim {

using namespace ns3;

NodeGrid::NodeGrid (int size, double spacing) :
    NodeSystem (),
    m_xLength (size),
    m_yLength (size),
    m_xDist (spacing),
    m_yDist (spacing)
{
}

NodeGrid::~NodeGrid ()
{
}

void
NodeGrid::MakeNodes ()
{
  int n = m_xLength * m_yLength;
  int center = n / 2;

  m_nodes.Create (n);

  for (int i = 0; i < n; i++)
    {
      if (i != center)
        m_staNodes.Add (m_nodes.Get (i));
    }

  m_apNodes.Add (m_nodes.Get (center));
}

void
NodeGrid::ConfigureDevices ()
{
  Ssid ssid = Ssid("ns-3-ssid");

  // * Set ActiveProbing to false to prevent collisions on 802.11 probe
  //   requests (known limitation in ns3)
  // * Do the nodes in order so that MAC addresses are properly assigned

  NS_ASSERT (m_staDevices.GetN () == 0); //TODO

  // STA nodes
  for (int i = 0; i < m_xLength * m_yLength / 2; i++)
    {
      m_wifiMac.SetType ("ns3::StaWifiMac",
          "Ssid", SsidValue (ssid),
          "ActiveProbing", BooleanValue (false));
      NetDeviceContainer newDevice = m_wifi.Install (m_wifiPhy, m_wifiMac, m_staNodes.Get (i));
      m_staDevices.Add (newDevice);
      m_devices.Add (newDevice);
    }

  // AP node
  m_wifiMac.SetType ("ns3::ApWifiMac",
      "Ssid", SsidValue (ssid));
  m_apDevices = m_wifi.Install (m_wifiPhy, m_wifiMac, m_apNodes);
  m_devices.Add (m_apDevices);

  // Other STA nodes
  for (int i = m_xLength * m_yLength / 2; i < m_xLength * m_yLength - 1; i++)
    {
      m_wifiMac.SetType ("ns3::StaWifiMac",
          "Ssid", SsidValue (ssid),
          "ActiveProbing", BooleanValue (false));

      NetDeviceContainer newDevice = m_wifi.Install (m_wifiPhy, m_wifiMac, m_staNodes.Get (i));
      m_staDevices.Add (newDevice);
      m_devices.Add (newDevice);
    }
}

void
NodeGrid::ConfigureLayout ()
{
  SIM_LOG (m_xLength << "x" << m_yLength << " grid layout");

  MobilityHelper mobility;

  mobility.SetPositionAllocator (
      "ns3::GridPositionAllocator",
      "MinX",           DoubleValue (0.0),
      "MinY",           DoubleValue (0.0),
      "DeltaX",         DoubleValue (m_xDist),
      "DeltaY",         DoubleValue (m_yDist),
      "GridWidth",      UintegerValue (m_xLength),
      "LayoutType",     StringValue ("RowFirst")
  );

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (m_nodes);
}

std::string
NodeGrid::GetPcapFileName () const
{
  return "sim-node-grid";
}

uint32_t
NodeGrid::GetApId () const
{
  return m_xLength * m_yLength / 2;
}

uint32_t
NodeGrid::GetServerId () const
{
  return this->GetApId ();
}

} /* namespace sim */
