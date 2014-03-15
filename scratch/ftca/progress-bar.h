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

#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

#include <iostream>

namespace sim {

/*
 * A basic progress bar for tracking simulation time.
 */
class ProgressBar
{
private:
  double m_time;        // The time, in seconds, corresponding to 100% progress
  int m_size;           // How wide the progress bar should be (fixed width terminals)
  int m_resolution;     // How often to update the progress bar (will be updated every time/resolution seconds)
  std::ostream *m_out;  // Where to print the progress bar

public:
  explicit ProgressBar (double time);
  virtual ~ProgressBar ();

  void SetSize (int size);
  void SetResolution (int resolution);
  void SetCheckpoint (double time);
  void SetOstream (std::ostream *p);
  void Enable ();

private:
  void Update ();
};

} /* namespace sim */
#endif /* PROGRESS_BAR_H_ */
