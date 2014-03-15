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

#ifndef PHYSIM_COLLISION_LOG_H_
#define PHYSIM_COLLISION_LOG_H_

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

/*
 * Records packet collision information to a log file.
 */
class PhySimCollisionLog
{
private:
  std::ostream *m_file;
  unsigned long m_eventNum;

public:
  PhySimCollisionLog ();

  explicit
  PhySimCollisionLog (std::ostream& out);

  virtual
  ~PhySimCollisionLog ();

  void
  Enable (uint32_t nodeId = -1);

private:
  void
  PhyCollisionTrace (std::string context, Ptr<const PhySimInterferenceHelper::Event> event,
      const std::list<Ptr<PhySimInterferenceHelper::Event> >& interferenceEvents);
};

} /* namespace sim */
#endif /* PHYSIM_COLLISION_LOG_H_ */
