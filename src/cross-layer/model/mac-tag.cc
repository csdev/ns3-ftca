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

#include "mac-tag.h"

namespace ns3 {

MacTag::MacTag () :
    m_addr ()
{
}

MacTag::MacTag (Mac48Address addr) :
    m_addr (addr)
{
}

MacTag::~MacTag ()
{
}

TypeId
MacTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MacTag")
      .SetParent<Tag> ()
      .AddConstructor<MacTag> ()
  ;
  return tid;
}

TypeId
MacTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t
MacTag::GetSerializedSize (void) const
{
  return 6;
}

void
MacTag::Serialize (TagBuffer i) const
{
  uint8_t buffer[6];
  m_addr.CopyTo (buffer);
  i.Write (buffer, 6);
}

void
MacTag::Deserialize (TagBuffer i)
{
  uint8_t buffer[6];
  i.Read (buffer, 6);
  m_addr.CopyFrom (buffer);
}

void
MacTag::Print (std::ostream& os) const
{
  os << "addr=" << m_addr;
}

void
MacTag::SetAddress (Mac48Address addr)
{
  m_addr = addr;
}

Mac48Address
MacTag::GetAddress (void) const
{
  return m_addr;
}

} /* namespace ns3 */
