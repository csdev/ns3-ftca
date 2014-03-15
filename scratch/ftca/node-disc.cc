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

#include "node-disc.h"

#include "sim-log.h"

namespace sim {

NodeDisc::NodeDisc (int n, double radius, double apHeight) :
    NodeSystem (),
    m_staCount (n),
    m_radius (radius),
    m_apHeight (apHeight)
{
}

NodeDisc::~NodeDisc ()
{
}

uint32_t
NodeDisc::GetApId () const
{
  return 0;
}

uint32_t
NodeDisc::GetServerId () const
{
  return this->GetApId ();
}

void
NodeDisc::MakeNodes ()
{
  m_apNodes.Create (1);
  m_staNodes.Create (m_staCount);
  m_nodes.Add (m_apNodes);
  m_nodes.Add (m_staNodes);
}

void
NodeDisc::ConfigureDevices ()
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
NodeDisc::ConfigureLayout ()
{
  SIM_LOG ("Disc layout");

  // Put the AP at the center
  ListPositionAllocator apPositionAllocator;
  apPositionAllocator.Add (Vector3D (0.0, 0.0, m_apHeight));

  MobilityHelper mobility;
  mobility.SetPositionAllocator (Ptr<ListPositionAllocator> (&apPositionAllocator));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (m_apNodes);

  // Allocate STA nodes in disc
  mobility.SetPositionAllocator (
      "ns3::UniformDiscPositionAllocator",
      "rho", DoubleValue (m_radius)
  );
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (m_staNodes);
}

std::string
NodeDisc::GetPcapFileName () const
{
  return "sim-node-disc";
}

} /* namespace sim */
