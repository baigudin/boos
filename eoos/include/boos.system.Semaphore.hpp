/**
 * Semaphore class.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2017 Sergey Baigudin
 * @license   http://baigudin.software/license/
 * @link      http://baigudin.software
 */
#ifndef BOOS_SYSTEM_SEMAPHORE_HPP_
#define BOOS_SYSTEM_SEMAPHORE_HPP_

#include "boos.Object.hpp"
#include "boos.api.Semaphore.hpp"
#include "boos.util.LinkedList.hpp"

namespace system
{
  class Thread;

  class Semaphore : public ::Object<>, public ::api::Semaphore
  {
    typedef ::Object<> Parent;
   
  public:

    /** 
     * Constructor.
     *
     * @param permits the initial number of permits available.   
     */      
    Semaphore(int32 permits);
    
    /** 
     * Constructor.
     *
     * @param permits the initial number of permits available.      
     * @param fair true if this semaphore will guarantee FIFO granting of permits under contention.
     */      
    Semaphore(int32 permits, bool fair);
                            
    /** 
     * Destructor.
     */
    virtual ~Semaphore();
    
    /**
     * Tests if this object has been constructed.
     *
     * @return true if object has been constructed successfully.
     */    
    virtual bool isConstructed() const;

    /**
     * Acquires one permit from this semaphore.
     *
     * @return true if the semaphore is acquired successfully.
     */  
    virtual bool acquire();

    /**
     * Acquires the given number of permits from this semaphore.
     *
     * @param permits the number of permits to acquire.
     * @return true if the semaphore is acquired successfully.
     */  
    virtual bool acquire(int32 permits);

    /**
     * Releases one permit.
     */
    virtual void release();

    /**
     * Releases the given number of permits.
     *
     * @param permits the number of permits to release.
     */  
    virtual void release(int32 permits);

    /**
     * Tests if this semaphore is fair.
     *
     * @return true if this semaphore has fairness set true.
     */  
    virtual bool isFair() const;

    /** 
     * Tests if this resource is blocked.
     *
     * @return true if this resource is blocked.
     */ 
    virtual bool isBlocked();

  private:

    /**
     * Fairly acquires the given number of permits from this semaphore.
     *
     * @param permits the number of permits to acquire.
     * @return true if the semaphore is acquired successfully.
     */  
    bool acquireFair(int32 permits);
    
    /**
     * Unfairly acquires the given number of permits from this semaphore.
     *
     * @param permits the number of permits to acquire.
     * @return true if the semaphore is acquired successfully.
     */  
    bool acquireUnfair(int32 permits);      

    /**
     * Constructor.
     *
     * @return true if object has been constructed successfully.     
     */    
    bool construct();
    
    /**
     * Copy constructor.
     *
     * @param obj reference to source object.
     */
    Semaphore(const Semaphore& obj);

    /**
     * Assignment operator.
     *
     * @param obj reference to source object.
     * @return reference to this object.     
     */
    Semaphore& operator =(const Semaphore& obj);     
    
    /** 
     * The root object constructed flag.
     */  
    const bool& isConstructed_;
    
    /**
     * Number of permits for acquiring this semaphore.
     */
    int32 permits_;
    
    /**
     * Semaphore fair flag.
     */    
    bool isFair_;

    /** 
     * Queue of locked threads.
     */     
    ::util::LinkedList<Thread*> fifo_;

  };  
}
#endif //BOOS_SYSTEM_SEMAPHORE_HPP_
