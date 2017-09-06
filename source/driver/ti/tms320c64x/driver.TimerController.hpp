/** 
 * Hardware timer resource.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2016-2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef DRIVER_TIMER_CONTROLLER_HPP_
#define DRIVER_TIMER_CONTROLLER_HPP_

#include "driver.TimerResource.hpp"
#include "driver.Interrupt.hpp"
#include "driver.reg.Timer.hpp"

namespace driver
{
  class TimerController : public ::driver::TimerResource
  {
    typedef ::driver::TimerResource  Parent;
    
  public:
  
    /**
     * Available interrupt sources.
     */
    enum InterruptSource
    {
      TINT0 = 0x01, //Timer 0 interrupt
      TINT1 = 0x02, //Timer 1 interrupt
      TINT2 = 0x13  //Timer 2 interrupt
    };

    /** 
     * Constructor.
     */      
    TimerController() : Parent(),
      index_  (-1),
      regTim_ (NULL){
      for(int32 i=0; i<RESOURCES_NUMBER; i++) 
      {
        if( construct(i) == true )
        {
          setConstruct(true);
          return;
        }
      }
      setConstruct(false);
    }    
    
    /** 
     * Constructor.
     *
     * @param index available timer index.
     */
    TimerController(int32 index) : Parent(),
      index_  (-1),
      regTim_ (NULL){
      setConstruct( construct(index) );
    }

    /** 
     * Destructor.
     */    
    virtual ~TimerController()
    {
      if(!isConstructed_) return;
      bool is = Interrupt::globalDisable();
      lock_[index_] = false;
      stop();
      regTim_ = NULL;
      index_ = -1;      
      Interrupt::globalEnable(is);    
    }
    
    /**
     * Gets this timer counter.
     *
     * @return timer counter register value.   
     */      
    virtual int64 getCount() const
    {
      return isConstructed_ ? regTim_->cnt.value : 0;
    }
    
    /**
     * Gets this timer period.
     *
     * @return timer period register value.
     */      
    virtual int64 getPeriod() const
    {
      return isConstructed_ ? regTim_->prd.value : 0;
    }  
    
    /**
     * Sets this timer counter.
     *
     * @param count timer counter register value.
     */      
    virtual void setCount(int64 count)
    {
      if(!isConstructed_) return;
      uint64 cnt = count;
      uint64 prd = getPeriod();
      if(cnt > prd) return;
      regTim_->cnt.value = cnt bitand 0x00000000ffffffff;
    }      
    
    /**
     * Sets this timer period.
     *
     * @param us timer period in microseconds, zero sets the period to maxinum value.
     */      
    virtual void setPeriod(int64 us=0)
    {
      if(!isConstructed_) return;
      if(us == 0) 
      {
        regTim_->prd.value = 0xffffffff;
      }
      else
      {
        int64 clock = internalClock();
        if(clock == 0) return;      
        uint64 prd = (us * clock) / 1000000;
        if( (prd & 0xffffffff00000000) == 0) regTim_->prd.value = prd & 0x00000000ffffffff;
        else return setPeriod();
      }    
    }
    
    /**
     * Starts this timer count.
     */      
    virtual void start()
    {
      if(!isConstructed_) return;
      reg::Timer::CTL ctl = 0;
      ctl.bit.hld = 1;      
      ctl.bit.clksrc = 1;
      ctl.bit.spnd = 1;     
      ctl.bit.go = 1;
      regTim_->ctl = ctl;    
    }
    
    /**
     * Stops this timer count.
     */      
    virtual void stop()
    {
      if(!isConstructed_) return;
      regTim_->ctl.value = 0;     
    }

    /**
     * Returns this timer index.
     *
     * @return index of this timer, or -1 if error has been occurred.
     */      
    virtual int32 getIndex() const
    {
      return isConstructed_ ? index_ : -1;
    }
    
    /**
     * Returns number of timer digits.
     *
     * @return timer digits.
     */  
    virtual int32 digits() const
    {
      return 32;
    }
    
    /**
     * Returns this timer internal clock in Hz.
     *
     * @return timer internal clock.
     */  
    virtual int64 internalClock() const
    {
      return config_.cpuClock >> 3;    
    }    
    
    /**
     * Tests if this timer can interrupt a CPU.
     *
     * @return true if this source is polarizing.
     */  
    virtual bool isInterrupting() const
    {
      return true;
    }
    
    /**
     * Returns an available interrupt source for this timer.
     *
     * @return available interrupt source, or -1 if error has been occurred.
     */  
    virtual int32 interrupSource() const
    {
      switch(index_)
      {
        case 0: return TINT0;
        case 1: return TINT1;
        case 2: return TINT2;
      }
      return -1;
    }
    
    /**
     * Initialization.
     *
     * @param config the operating system configuration.
     * @return true if no errors.
     */
    static bool init(const ::Configuration& config)
    {
      isInitialized_ = 0;        
      config_ = config;
      for(int32 i=0; i<RESOURCES_NUMBER; i++) 
      {
        switch(i)
        {
          case  0: new (reg::Timer::ADDRESS0) reg::Timer(); break;
          case  1: new (reg::Timer::ADDRESS1) reg::Timer(); break;
          case  2: new (reg::Timer::ADDRESS2) reg::Timer(); break;
          default: return false;
        }
        lock_[i] = false;      
      } 
      isInitialized_ = IS_INITIALIZED;            
      return true;
    }
    
    /**
     * Deinitialization.
     */
    static void deinit()
    {
      isInitialized_ = 0;    
    }    
    
  private:
  
    /** 
     * Constructor.
     *
     * @param index available timer index.
     * @return boolean result.
     */  
    bool construct(int32 index)
    {
      if(isInitialized_ != IS_INITIALIZED) return false;    
      uint32 addr;
      bool is = Interrupt::globalDisable();
      switch(index)
      {
        case  0: addr = reg::Timer::ADDRESS0; break;
        case  1: addr = reg::Timer::ADDRESS1; break;
        case  2: addr = reg::Timer::ADDRESS2; break;
        default: return Interrupt::globalEnable(is, false);
      }    
      if(lock_[index] == true) return Interrupt::globalEnable(is, false); 
      regTim_ = new (addr) reg::Timer();
      lock_[index] = true;
      index_ = index;
      return Interrupt::globalEnable(is, true);    
    }
    
    /**
     * Copy constructor.
     *
     * @param obj reference to source object.
     */
    TimerController(const TimerController& obj);

    /**
     * Assignment operator.
     *
     * @param obj reference to source object.
     * @return reference to this object.     
     */
    TimerController& operator =(const TimerController& obj);
    
    /**
     * The driver initialized falg value.
     */
    static const int32 IS_INITIALIZED = 0x95633217;        

    /**
     * Number of HW timers.
     */
    static const int32 RESOURCES_NUMBER = 3;
    
    /**
     * Driver has been initialized successfully (no boot).
     */
    static int32 isInitialized_;        
    
    /**
     * The operating system configuration (no boot).
     */
    static ::Configuration config_;     
    
    /**
     * Locked by some object flag of each HW timer (no boot).
     */    
    static bool lock_[RESOURCES_NUMBER];
    
    /**
     * Number of hardware timer
     */
    int32 index_;
    
    /**
     * HW timer registers.
     */
    reg::Timer* regTim_;        
  
  };
  
  /**
   * Driver has been initialized successfully (no boot).
   */
  int32 TimerController::isInitialized_;    
  
  /**
   * The operating system configuration (no boot).
   */
  ::Configuration TimerController::config_;

  /**
   * Locked by some object flag of each HW timer (no boot).  
   */
  bool TimerController::lock_[TimerController::RESOURCES_NUMBER];
  
}
#endif // DRIVER_TIMER_CONTROLLER_HPP_
