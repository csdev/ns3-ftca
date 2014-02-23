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

#ifndef QOS_BULK_SEND_APPLICATION_H_
#define QOS_BULK_SEND_APPLICATION_H_

#include "bulk-send-application.h"

namespace ns3 {

class QosBulkSendApplication : public BulkSendApplication
{
protected:
  uint8_t m_qosTid;

public:
  static TypeId GetTypeId (void);

  QosBulkSendApplication ();
  virtual ~QosBulkSendApplication ();

  void SetQosTid (uint8_t tid);

private:
  // Overrides BulkSendApplication::SendData, tags packets with qos info
  virtual void SendData ();
};

} /* namespace ns3 */
#endif /* QOS_BULK_SEND_APPLICATION_H_ */
