/** 
 * Hardware interrupt interface.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2015-2016 Sergey Baigudin
 * @license   http://baigudin.software/license/
 * @link      http://baigudin.software
 */
#ifndef BOOS_API_INTERRUPT_HPP_
#define BOOS_API_INTERRUPT_HPP_

#include "boos.api.Toggle.hpp"

namespace api
{
  class Interrupt : public ::api::Toggle
  {

  public:
    
    /** 
     * Destructor.
     */                               
    virtual ~Interrupt(){}
    
    /**
     * Jumps to interrupt hardware vector.
     */      
    virtual void jump() = 0;
    
    /**
     * Clears an interrupt status of this source.
     */     
    virtual void clear() = 0;
    
    /**
     * Sets an interrupt status of this source.
     */    
    virtual void set() = 0;
    
  };
}
#endif // BOOS_CORE_INTERRUPT_HPP_
