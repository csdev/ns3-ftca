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

#include "node-star.h"

#include "sim-log.h"

namespace sim {

NodeStar::NodeStar (int n, double radius) :
    NodeSystem (),
    m_staCount (n),
    m_radius (radius)
{
}

NodeStar::~NodeStar ()
{
}

uint32_t
NodeStar::GetApId () const
{
  return 0;
}

uint32_t
NodeStar::GetServerId () const
{
  return 0;
}

void
NodeStar::MakeNodes ()
{
  m_apNodes.Create (1);
  m_staNodes.Create (m_staCount);
  m_nodes.Add (m_apNodes);
  m_nodes.Add (m_staNodes);
}

void
NodeStar::ConfigureDevices ()
{
  Ssid ssid = Ssid ("ns-3-ssid");

  m_wifiMac.SetType ("ns3::ApWifiMac",
      "Ssid", SsidValue (ssid));
  m_apDevices = m_wifi.Install (m_wifiPhy, m_wifiMac, m_apNodes);
  m_devices.Add (m_apDevices);

  m_wifiMac.SetType ("ns3::StaWifiMac",
      "Ssid", SsidValue (ssid),
      "ActiveProbing", BooleanValue (false));
  m_staDevices = m_wifi.Install (m_wifiPhy, m_wifiMac, m_staNodes);
  m_devices.Add (m_staDevices);
}

void
NodeStar::ConfigureLayout ()
{
  SIM_LOG ("Star Layout");

  // Put the AP at the center
  ListPositionAllocator apPositionAllocator;
  apPositionAllocator.Add (Vector3D (0.0, 0.0, 0.0));

  MobilityHelper mobility;
  mobility.SetPositionAllocator (Ptr<ListPositionAllocator> (&apPositionAllocator));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (m_apNodes);

  // Allocate STA nodes evenly spaced on a circle
  mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
      "Theta", RandomVariableValue (SequentialVariable (0.0, 2*M_PI, 2*M_PI/m_staCount)),
      "Rho", RandomVariableValue (ConstantVariable (m_radius))
  );
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (m_staNodes);
}

std::string
NodeStar::GetPcapFileName () const
{
  return "sim-node-star";
}

} /* namespace sim */
