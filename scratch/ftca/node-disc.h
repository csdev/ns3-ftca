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

#ifndef NODE_DISC_H_
#define NODE_DISC_H_

#include "node-system.h"

namespace sim {

class NodeDisc : public NodeSystem
{
protected:
  int m_staCount;
  double m_radius;
  double m_apHeight;

public:
  NodeDisc (int n, double radius, double apHeight);
  virtual ~NodeDisc ();

  virtual uint32_t GetApId () const;
  virtual uint32_t GetServerId () const;

protected:
  virtual void MakeNodes ();
  virtual void ConfigureDevices ();
  virtual void ConfigureLayout ();
  virtual std::string GetPcapFileName () const;
};

} /* namespace sim */
#endif /* NODE_DISC_H_ */
