/*******************************************************************************
 * DroneRK_Transport_Read_Thread.h
 *
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *
 * Custom Madara transport for DroneRK
 ******************************************************************************/

#ifndef _DRONERK_TRANSPORT_READ_THREAD_H_
#define _DRONERK_TRANSPORT_READ_THREAD_H_

#include <string>

#include "madara/utility/Scoped_Array.h"
#include "madara/knowledge_engine/Thread_Safe_Context.h"
#include "madara/transport/Transport.h"
#include "madara/expression_tree/Expression_Tree.h"

#include "ace/Task.h"
#include "ace/Mutex.h"
#include "ace/Barrier.h"
#include "ace/Atomic_Op_T.h"
#include "ace/Thread_Mutex.h"
#include "ace/SOCK_Dgram.h"

#include "ace/Synch.h"

/**
 * @class DroneRK_Transport_Read_Thread
 * @brief Thread for reading knowledge updates through a Multicast
 * datagram socket
 **/
class DroneRK_Transport_Read_Thread : public ACE_Task<ACE_MT_SYNCH>
{
public:
  /**
   * Constructor
   * @param    settings   Transport settings
   * @param    id      host:port identifier of this process, to allow for 
   *                   rejection of duplicates
   * @param    context    the knowledge variables to update
   * @param    address    the ACE socket address to read from 
   **/
  DroneRK_Transport_Read_Thread(
    const Madara::Transport::Settings& settings,
    const std::string& id,
    Madara::Knowledge_Engine::Thread_Safe_Context& context,
    const ACE_INET_Addr& address);
  
  /**
  * Destructor
  **/
  ~DroneRK_Transport_Read_Thread();

  /**
  * Signals the read thread to terminate
  **/
  int enter_barrier(void);
  
  /**
  * Closes the reading socket and clean up the thread
  **/
  int close(void);

  /**
  * Reads messages from a socket
  **/
  int svc(void);
  
  /**
  * Wait for the transport to be ready
  **/
  void wait_for_ready(void);

private:
  /// Transport settings
  const Madara::Transport::Settings& settings_;

  /// host:port identifier of this process
  const std::string                               id_;

  /// knowledge context
  Madara::Knowledge_Engine::Thread_Safe_Context&  context_;
  
  /// barrier for closing and waiting on the read thread
  ACE_Barrier                                     barrier_;

  /// atomic variable that signals termination
  ACE_Atomic_Op<ACE_Mutex, bool>                  terminated_;
  
  /// Mutex for use with terminated_
  ACE_Thread_Mutex                                mutex_;
  
  /// Condition that is waited on my calling transport on startup
  Madara::Transport::Condition                    is_not_ready_;

  /// Indicates whether the read thread is ready to accept messages
  bool                                            is_ready_;

  /// The broadcast address we are subscribing to
  ACE_INET_Addr                                   address_;
  
  /// The broadcast socket we are reading from
  ACE_SOCK_Dgram                                  socket_;

  /// data received rules, defined in Transport settings
  Madara::Expression_Tree::Expression_Tree        on_data_received_;
  
  /// buffer for sending
  Madara::Utility::Scoped_Array<char>             buffer_;
};

#endif // _DRONERK_TRANSPORT_READ_THREAD_H_