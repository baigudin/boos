/** 
 * Resource interface.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2015 Sergey Baigudin
 * @license   http://baigudin.software/license/
 * @link      http://baigudin.software
 */
#ifndef BOOS_API_RESOURCE_HPP_
#define BOOS_API_RESOURCE_HPP_

#include "boos.api.Object.hpp"

namespace api
{
  class Resource : public ::api::Object
  {

  public:
  
    /** 
     * Destructor.
     */
    virtual ~Resource(){}

    /** 
     * Tests if this resource is blocked.
     *
     * @return true if this resource is blocked.
     */ 
    virtual bool isBlocked() = 0;

  };
}
#endif // BOOS_API_RESOURCE_HPP_
