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

#ifndef NODE_LINE_3_H_
#define NODE_LINE_3_H_

#include <iostream>
#include <string>
#include <vector>

#include "ns3/core-module.h"
#include "ns3/physim-wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

#include "ns3/nqos-wifi-mac-helper.h"
#include "ns3/ssid.h"

#include "ns3/trace-helper.h"

#include "node-system.h"

namespace sim {

using namespace ns3;

// 3 nodes in a line, AP at the center
class NodeLine3 : public NodeSystem
{
private:
  double m_distance1;
  double m_distance2;

public:
  explicit  NodeLine3 (double distance);
  NodeLine3 (double distance1, double distance2);
  virtual ~NodeLine3 ();

  virtual uint32_t GetApId () const;
  virtual uint32_t GetServerId () const;

  void ForceHiddenNodes ();

protected:
  virtual void MakeNodes ();
  virtual void ConfigureDevices ();
  virtual void ConfigureLayout ();
  virtual std::string GetPcapFileName () const;
};

} /* namespace sim */
#endif /* NODE_LINE_3_H_ */
