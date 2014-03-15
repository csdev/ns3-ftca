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

#include "progress-bar.h"

#include <cmath>
#include "ns3/core-module.h"

namespace sim {

using namespace ns3;

ProgressBar::ProgressBar (double time) :
    m_time (time),
    m_size (50),
    m_resolution (100),
    m_out (&std::clog)
{
}

ProgressBar::~ProgressBar ()
{
}

void
ProgressBar::SetSize (int size)
{
  m_size = size;
}

void
ProgressBar::SetResolution (int resolution)
{
  m_resolution = resolution;
}

void
ProgressBar::SetCheckpoint (double time)
{
  m_resolution = (int) std::ceil (m_time / time);
}

void
ProgressBar::SetOstream (std::ostream *p)
{
  m_out = p;
}

void
ProgressBar::Enable ()
{
  for (int i = 0; i <= m_resolution; i++)
    {
      double t = (double) i / m_resolution * m_time;
      Simulator::Schedule (Seconds (t), &ProgressBar::Update, this);
    }
}

void
ProgressBar::Update ()
{
  double now = Simulator::Now ().GetSeconds ();
  int nMarks = (int) (now / m_time * m_size);
  int percent = (int) (now / m_time * 100);

  if (nMarks > m_size)
    nMarks = m_size;  // clamp in case of roundoff error

  *m_out << "\r[";
  for (int i = 0; i < nMarks; i++)
    *m_out << '#';
  for (int i = nMarks; i < m_size; i++)
    *m_out << ' ';
  *m_out << "] ";

  *m_out << (int) now << "s " << percent << "% ";
  if (nMarks == m_size)
    *m_out << '\n';
  *m_out << std::flush;
}

} /* namespace sim */
