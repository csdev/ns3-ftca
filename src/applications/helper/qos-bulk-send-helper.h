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

#ifndef QOS_BULK_SEND_HELPER_H_
#define QOS_BULK_SEND_HELPER_H_

#include <stdint.h>
#include <string>
#include "ns3/object-factory.h"
#include "ns3/address.h"
#include "ns3/attribute.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/application-container.h"

namespace ns3 {

class QosBulkSendHelper
{
public:
  QosBulkSendHelper (std::string protocol, Address address);
  QosBulkSendHelper (std::string protocol, Address address, uint8_t qosTid);
  virtual ~QosBulkSendHelper ();

  void SetAttribute (std::string name, const AttributeValue &value);
  ApplicationContainer Install (NodeContainer c) const;
  ApplicationContainer Install (Ptr<Node> node) const;
  ApplicationContainer Install (std::string nodeName) const;

private:
  Ptr<Application> InstallPriv (Ptr<Node> node) const;
  std::string m_protocol;
  Address m_remote;
  ObjectFactory m_factory;
};

} /* namespace ns3 */
#endif /* QOS_BULK_SEND_HELPER_H_ */
