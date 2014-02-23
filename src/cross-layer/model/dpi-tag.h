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

#ifndef DPI_TAG_H_
#define DPI_TAG_H_

#include <iostream>
#include "ns3/tag.h"
#include "ns3/packet.h"

namespace ns3
{

class DpiTag : public Tag
{
public:
  static const int DPI_PACKET_TYPES = 5;

  //TODO leaked macro definition
#define _ENUM_PACKET_TYPE       \
    _ENUM_ITEM(UNSPECIFIED),    \
    _ENUM_ITEM(UDP),            \
    _ENUM_ITEM(TCP_DATA),       \
    _ENUM_ITEM(TCP_ACK),        \
    _ENUM_ITEM(MAC_ACK)

#define _ENUM_ITEM(x) x

  enum PacketType { _ENUM_PACKET_TYPE };

#undef _ENUM_ITEM

  static const char * const PacketTypeName[DpiTag::DPI_PACKET_TYPES];

  DpiTag ();
  DpiTag (enum PacketType type);
  virtual ~DpiTag ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  void SetPacketType (enum PacketType type);
  enum PacketType GetPacketType (void) const;

  static enum PacketType GetPacketType (Ptr<const Packet> packet);

private:
  enum PacketType m_packetType;
};

} /* namespace ns3 */
#endif /* DPI_TAG_H_ */
