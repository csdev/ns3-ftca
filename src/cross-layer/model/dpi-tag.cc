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

#include "dpi-tag.h"

namespace ns3
{
#define _ENUM_ITEM(x) #x
const char * const DpiTag::PacketTypeName[DpiTag::DPI_PACKET_TYPES] = { _ENUM_PACKET_TYPE };
#undef _ENUM_ITEM

DpiTag::DpiTag () :
        m_packetType (DpiTag::UNSPECIFIED)
{

}

DpiTag::DpiTag (enum PacketType type) :
        m_packetType (type)
{

}

DpiTag::~DpiTag ()
{

}

TypeId
DpiTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::DpiTag")
                .SetParent<Tag> ()
                .AddConstructor<DpiTag> ()
                ;
  return tid;
}

TypeId
DpiTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
DpiTag::GetSerializedSize (void) const
{
  return 1;
}

void
DpiTag::Serialize (TagBuffer i) const
{
  i.WriteU8 (static_cast<uint8_t> (m_packetType));
}

void
DpiTag::Deserialize (TagBuffer i)
{
  m_packetType = static_cast<enum PacketType> (i.ReadU8 ());
}

void
DpiTag::Print (std::ostream &os) const
{
  os << "packetType=" << static_cast<uint32_t> (m_packetType);
}

void
DpiTag::SetPacketType (enum PacketType type)
{
  m_packetType = type;
}

enum DpiTag::PacketType
DpiTag::GetPacketType (void) const
{
  return m_packetType;
}

enum DpiTag::PacketType
DpiTag::GetPacketType (Ptr<const Packet> packet)
{
  DpiTag t;
  if (packet->PeekPacketTag (t))
    return t.GetPacketType ();
  else
    return DpiTag::UNSPECIFIED;
}

} /* namespace ns3 */
