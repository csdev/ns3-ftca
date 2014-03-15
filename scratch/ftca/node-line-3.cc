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

#include "node-line-3.h"

#include "sim-log.h"

NS_LOG_COMPONENT_DEFINE("sim::NodeLine3");

namespace sim {

using namespace ns3;

NodeLine3::NodeLine3 (double distance) :
    NodeSystem (),
    m_distance1 (distance),
    m_distance2 (distance)
{
}

NodeLine3::NodeLine3 (double distance1, double distance2) :
    NodeSystem (),
    m_distance1 (distance1),
    m_distance2 (distance2)
{
}

NodeLine3::~NodeLine3 ()
{
}

void
NodeLine3::ForceHiddenNodes ()
{
  Config::Set ("/NodeList/0/DeviceList/*/$ns3::WifiNetDevice/Phy/CcaModelThreshold", DoubleValue (9000.0));
  Config::Set ("/NodeList/2/DeviceList/*/$ns3::WifiNetDevice/Phy/CcaModelThreshold", DoubleValue (9000.0));
}

void
NodeLine3::MakeNodes ()
{
  m_nodes.Create (3);
  m_staNodes.Add (m_nodes.Get (0));
  m_apNodes.Add (m_nodes.Get (1));
  m_staNodes.Add (m_nodes.Get (2));
}

void
NodeLine3::ConfigureDevices ()
{
  Ssid ssid = Ssid ("ns-3-ssid");

  // First STA node:
  //  * Set ActiveProbing to false to prevent collisions on 802.11 probe
  //    requests (known limitation in ns3)
  //  * Do the nodes in order so that MAC addresses are properly assigned
  m_wifiMac.SetType ("ns3::StaWifiMac",
      "Ssid", SsidValue (ssid),
      "ActiveProbing", BooleanValue (false));
  m_staDevices = m_wifi.Install (m_wifiPhy, m_wifiMac, m_staNodes.Get (0));

  // AP node
  m_wifiMac.SetType ("ns3::ApWifiMac",
      "Ssid", SsidValue (ssid));
  m_apDevices = m_wifi.Install (m_wifiPhy, m_wifiMac, m_apNodes);

  // Other STA node
  m_wifiMac.SetType ("ns3::StaWifiMac",
      "Ssid", SsidValue (ssid),
      "ActiveProbing", BooleanValue (false));
  m_staDevices.Add (m_wifi.Install (m_wifiPhy, m_wifiMac, m_staNodes.Get (1)));

  m_devices.Add (m_staDevices.Get (0));
  m_devices.Add (m_apDevices);
  m_devices.Add (m_staDevices.Get (1));
}

void
NodeLine3::ConfigureLayout ()
{
  SIM_LOG ("Linear layout with distances: " << m_distance1 << ", " << m_distance2);

  ListPositionAllocator positionAllocator;
  positionAllocator.Add (Vector3D (0.0, 0.0, 0.0));
  positionAllocator.Add (Vector3D (m_distance1, 0.0, 0.0));
  positionAllocator.Add (Vector3D (m_distance1 + m_distance2, 0.0, 0.0));

  MobilityHelper mobility;
  mobility.SetPositionAllocator (Ptr<ListPositionAllocator> (&positionAllocator));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (m_nodes);
}

std::string
NodeLine3::GetPcapFileName () const
{
  return "sim-node-line-3";
}

uint32_t
NodeLine3::GetApId () const
{
  return 1;
}

uint32_t
NodeLine3::GetServerId () const
{
  return this->GetApId ();
}

} /* namespace sim */
