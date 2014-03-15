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

#ifndef TRACE_COUNTER_GROUP_H_
#define TRACE_COUNTER_GROUP_H_

namespace sim {

/*
 * Base class for trace counter objects.
 */
template <int N>
class TraceCounterGroup
{

public:
  TraceCounterGroup();

  virtual
  ~TraceCounterGroup();

  virtual void
  enableCounters(uint32_t nodeId = -1) = 0;

//    virtual void
//    addCounters(std::string packetTag) = 0;

  virtual void
  printCounters(uint32_t s, uint32_t d, std::ostream& out = std::cout) const = 0;

};

template <int N>
TraceCounterGroup<N>::TraceCounterGroup()
{
}

template <int N>
TraceCounterGroup<N>::~TraceCounterGroup()
{
}

} /* namespace sim */
#endif /* TRACE_COUNTER_GROUP_H_ */
