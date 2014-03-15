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

#ifndef PHYSIM_COLLISION_COUNTER_H_
#define PHYSIM_COLLISION_COUNTER_H_

#include <iostream>

#include <boost/format.hpp>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/physim-wifi-module.h"

#include "ns3/dpi-tag.h"
#include "ns3/mac-tag.h"

#include "trace-utils.h"

namespace sim {

using namespace ns3;

template <int N>
class PhySimCollisionCounter
{
private:
  typedef unsigned long counter_t;

  // trace counters for TCP packet collisions, indexed by [source id, destination id] pair
  struct CollisionCounterSet {
    counter_t data_data[N][N];  // collision between two DATA packets, with the first DATA definitely lost
    counter_t data_ack[N][N];   // collision between DATA and ACK, with the DATA packet definitely lost
    counter_t ack_data[N][N];   // collision between ACK and DATA, with the ACK definitely lost
    counter_t ack_ack[N][N];    // collision between two ACK packets, with the first ACK definitely lost
  } m_counters;

public:
  PhySimCollisionCounter ();
  virtual ~PhySimCollisionCounter ();

  void Enable ();
  void Reset ();
  void PrintCounters (uint32_t s, uint32_t d, std::ostream& out = std::cout) const;

private:
  void
  PhyCollisionTrace (std::string context, Ptr<const PhySimInterferenceHelper::Event> event,
      const std::list<Ptr<PhySimInterferenceHelper::Event> >& interferenceEvents);
};

template <int N>
PhySimCollisionCounter<N>::PhySimCollisionCounter ()
{
  memset (m_counters.data_data, 0, sizeof (m_counters.data_data[0][0]) * N * N);
  memset (m_counters.data_ack, 0, sizeof (m_counters.data_ack[0][0]) * N * N);
  memset (m_counters.ack_data, 0, sizeof (m_counters.ack_data[0][0]) * N * N);
  memset (m_counters.ack_ack, 0, sizeof (m_counters.ack_ack[0][0]) * N * N);
}

template <int N>
PhySimCollisionCounter<N>::~PhySimCollisionCounter ()
{
}

template <int N>
void
PhySimCollisionCounter<N>::Enable ()
{
  Config::Connect (TraceUtils::phySimWifiTracePath ("Collision", -1),
      MakeCallback (&PhySimCollisionCounter<N>::PhyCollisionTrace, this));
}

template <int N>
void
PhySimCollisionCounter<N>::Reset ()
{
  memset (m_counters.data_data, 0, sizeof (m_counters.data_data[0][0]) * N * N);
  memset (m_counters.data_ack, 0, sizeof (m_counters.data_ack[0][0]) * N * N);
  memset (m_counters.ack_data, 0, sizeof (m_counters.ack_data[0][0]) * N * N);
  memset (m_counters.ack_ack, 0, sizeof (m_counters.ack_ack[0][0]) * N * N);
}

template <int N>
void
PhySimCollisionCounter<N>::PhyCollisionTrace (std::string context, Ptr<const PhySimInterferenceHelper::Event> event,
    const std::list<Ptr<PhySimInterferenceHelper::Event> >& interferenceEvents)
{
  Ptr<const Packet> packet = event->GetPacket ();
  Ptr<PhySimWifiPhyTag> tag = event->GetWifiPhyTag ();

  // Non-promiscuous trace -- ignore packets intended for other nodes
  MacTag addrTag;
  bool hasAddrTag = packet->PeekPacketTag (addrTag);
  if (!hasAddrTag || (addrTag.GetAddress () != tag->GetRxNetDevice()->GetAddress()) )
    return;

  // Retrieve information for the primary packet
  uint32_t sourceId = tag->GetTxNetDevice()->GetNode()->GetId();
  uint32_t destId = tag->GetRxNetDevice()->GetNode()->GetId();
  enum DpiTag::PacketType type = DpiTag::GetPacketType (packet);
  if (type == DpiTag::UNSPECIFIED)
    return;

  // Retrieve information for each interferer and update counters
  for (std::list<Ptr<PhySimInterferenceHelper::Event> >::const_iterator itr = interferenceEvents.begin ();
      itr != interferenceEvents.end (); itr++)
    {
      if ((*itr) == event || !(*itr)->Overlaps (event))
        continue;

      Ptr<const Packet> interferencePacket = (*itr)->GetPacket ();
//      Ptr<PhySimWifiPhyTag> interferenceTag = (*itr)->GetWifiPhyTag ();
      enum DpiTag::PacketType interferenceType = DpiTag::GetPacketType (interferencePacket);
      if (interferenceType == DpiTag::UNSPECIFIED)
        continue;

      if (type == DpiTag::MAC_ACK || interferenceType == DpiTag::MAC_ACK)
        {
          // these collisions are very rare and we are not really interested in them
//          NS_LOG_DEBUG ("Collision involving MAC layer ACK");
          continue;
        }

      if (type == DpiTag::TCP_ACK && interferenceType == DpiTag::TCP_ACK)
        m_counters.ack_ack[sourceId][destId]++;
      else if ((type == DpiTag::TCP_DATA || type == DpiTag::UDP) && interferenceType == DpiTag::TCP_ACK)
        m_counters.data_ack[sourceId][destId]++;
      else if (type == DpiTag::TCP_ACK && (interferenceType == DpiTag::TCP_DATA || interferenceType == DpiTag::UDP))
        m_counters.ack_data[sourceId][destId]++;
      else if ((type == DpiTag::TCP_DATA || type == DpiTag::UDP) && (interferenceType == DpiTag::TCP_DATA || interferenceType == DpiTag::UDP))
        m_counters.data_data[sourceId][destId]++;
      else
        continue;

      //TODO ignore 3-way packet collisions (also assumed to be rare)
      return;
    }
//  std::cerr << "Warning: No tag match in collision trace" << std::endl;
}

template <int N>
void
PhySimCollisionCounter<N>::PrintCounters (uint32_t s, uint32_t d, std::ostream& out) const
{
  out << m_counters.data_data[s][d] << ", ";
  out << m_counters.data_ack[s][d] << ", ";
  out << m_counters.ack_data[s][d] << ", ";
  out << m_counters.ack_ack[s][d];
  out << std::endl;
}

} /* namespace sim */
#endif /* PHYSIM_COLLISION_COUNTER_H_ */
