/*
  Copyright <2018-2019> <Ainstein, Inc.>

  Redistribution and use in source and binary forms, with or without modification, are permitted 
  provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this list of 
  conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice, this list of 
  conditions and the following disclaimer in the documentation and/or other materials provided 
  with the distribution.

  3. Neither the name of the copyright holder nor the names of its contributors may be used to 
  endorse or promote products derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
  FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>
#include "ainstein_radar_drivers/radar_interface_k79.h"

class NodeletK79 : public nodelet::Nodelet
{
public:
  NodeletK79( void ) {}
  ~NodeletK79( void ) {}
  
  virtual void onInit( void )
  {
    // Create the K79 interface and launch the data thread:
    NODELET_DEBUG("Initializing K79 interface nodelet");
    intf_ptr_.reset( new ainstein_radar_drivers::RadarInterfaceK79( getNodeHandle(), getPrivateNodeHandle() ) );
    intf_ptr_-> connect();
  }

private:
  std::unique_ptr<ainstein_radar_drivers::RadarInterfaceK79> intf_ptr_;
};

PLUGINLIB_EXPORT_CLASS( NodeletK79, nodelet::Nodelet )
