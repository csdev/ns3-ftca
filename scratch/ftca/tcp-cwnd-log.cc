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

#include "tcp-cwnd-log.h"

namespace sim {

using namespace ns3;

TcpCwndLog::TcpCwndLog () :
    m_file (&std::cout),
    m_eventNum (0)
{
}

TcpCwndLog::TcpCwndLog (std::ostream& out) :
    m_file (&out),
    m_eventNum (0)
{
}

TcpCwndLog::~TcpCwndLog ()
{
}

void
TcpCwndLog::SetFile (std::ostream& out)
{
  m_file = &out;
}

void
TcpCwndLog::Enable (uint32_t nodeId)
{
  Config::ConnectWithoutContext (TraceUtils::TcpTracePath ("CongestionWindow", nodeId, 0),
      MakeCallback (&TcpCwndLog::TcpCwndTrace, this));
}

void
TcpCwndLog::TcpCwndTrace (uint32_t oldValue, uint32_t newValue)
{
  *m_file << m_eventNum++ << " " << Simulator::Now ().GetMilliSeconds () << " " << newValue << std::endl;
}

} /* namespace sim */
