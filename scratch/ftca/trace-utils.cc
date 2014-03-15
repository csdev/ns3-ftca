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

#include "trace-utils.h"

namespace sim {

std::string
TraceUtils::phySimWifiTracePath(const std::string& traceSource, int nodeId, int deviceId)
{
  std::stringstream path;

  path << "/NodeList/";
  if (nodeId >= 0)
    path << nodeId;
  else
    path << "*";

  path << "/DeviceList/";
  if (deviceId >= 0)
    path << deviceId;
  else
    path << "*";

  path << "/$ns3::WifiNetDevice/Phy/" << traceSource;

  return path.str();
}

std::string
TraceUtils::TcpTracePath (const std::string& traceSource, int nodeId, int socketId)
{
  std::stringstream path;

  path << "/NodeList/";
  if (nodeId >= 0)
    path << nodeId;
  else
    path << "*";

  path << "/$ns3::TcpL4Protocol/SocketList/";
  if (socketId >= 0)
    path << socketId;
  else
    path << "*";

  path << "/" << traceSource;

  return path.str ();
}


TraceUtils::TraceUtils()
{
}

TraceUtils::~TraceUtils()
{
}

} /* namespace sim */
