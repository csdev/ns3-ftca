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

#ifndef MAC_TAG_H_
#define MAC_TAG_H_

#include <iostream>

#include "ns3/mac48-address.h"
#include "ns3/packet.h"
#include "ns3/tag.h"

namespace ns3 {

class MacTag : public Tag
{
public:
  MacTag ();
  MacTag (Mac48Address addr);
  virtual ~MacTag ();

  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual void Print (std::ostream &os) const;

  void SetAddress (Mac48Address addr);
  Mac48Address GetAddress (void) const;

private:
  Mac48Address m_addr;
};

} /* namespace ns3 */
#endif /* MAC_TAG_H_ */
