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

#ifndef TCP_CWND_LOG_H_
#define TCP_CWND_LOG_H_

#include <iostream>

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"

#include "trace-utils.h"

namespace sim {

using namespace ns3;

/*
 * Logs changes in the TCP congestion window to a file.
 */
class TcpCwndLog
{
private:
  std::ostream *m_file;
  unsigned long m_eventNum;

public:
  TcpCwndLog ();
  explicit TcpCwndLog (std::ostream& out);
  virtual ~TcpCwndLog ();

  void SetFile (std::ostream& out);
  void Enable (uint32_t nodeId);

private:
  void TcpCwndTrace (uint32_t oldValue, uint32_t newValue);
};

} /* namespace sim */
#endif /* TCP_CWND_LOG_H_ */
