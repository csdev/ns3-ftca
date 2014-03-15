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

#include "physim-collision-log.h"

NS_LOG_COMPONENT_DEFINE("sim::PhySimCollisionLog");

namespace sim {

PhySimCollisionLog::PhySimCollisionLog () :
    m_file (&std::cout),
    m_eventNum (0)
{
}

PhySimCollisionLog::PhySimCollisionLog (std::ostream& out) :
    m_file (&out),
    m_eventNum (0)
{
}

PhySimCollisionLog::~PhySimCollisionLog ()
{
}

void
PhySimCollisionLog::Enable (uint32_t nodeId)
{
  Config::Connect (TraceUtils::phySimWifiTracePath("Collision", nodeId),
      MakeCallback(&PhySimCollisionLog::PhyCollisionTrace, this));
}

void
PhySimCollisionLog::PhyCollisionTrace (std::string context, Ptr<const PhySimInterferenceHelper::Event> event,
    const std::list<Ptr<PhySimInterferenceHelper::Event> >& interferenceEvents)
{
  Ptr<const Packet> packet = event->GetPacket ();
  Ptr<PhySimWifiPhyTag> tag = event->GetWifiPhyTag ();

  // Non-promiscuous trace -- ignore packets intended for other nodes
  MacTag addrTag;
  bool hasAddrTag = packet->PeekPacketTag (addrTag);
  if (!hasAddrTag)
    {
      NS_LOG_DEBUG ("Packet missing MacTag");
      return;
    }

  if (addrTag.GetAddress () != tag->GetRxNetDevice()->GetAddress())
    return;

  // Print packet information to log file
  *m_file << boost::format ("%04lu %s %s %d\n")
      % m_eventNum++
      % Simulator::Now()
      % DpiTag::PacketTypeName[DpiTag::GetPacketType (packet)]
      % tag->GetTxedDataBits ().size ()
  ;

  for (std::list<Ptr<PhySimInterferenceHelper::Event> >::const_iterator itr = interferenceEvents.begin ();
      itr != interferenceEvents.end (); itr++)
    {
      if ((*itr) == event || !(*itr)->Overlaps (event))
        continue;

      Ptr<const Packet> interferencePacket = (*itr)->GetPacket ();
      Ptr<PhySimWifiPhyTag> interferenceTag = (*itr)->GetWifiPhyTag ();

      *m_file << boost::format ("\t%s %d\n")
          % DpiTag::PacketTypeName[DpiTag::GetPacketType(interferencePacket)]
          % interferenceTag->GetTxedDataBits ().size ()
      ;
    }

  *m_file << std::endl;
}

} /* namespace sim */
