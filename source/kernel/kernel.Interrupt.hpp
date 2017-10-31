/** 
 * Hardware interrupt resource.
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2014-2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef KERNEL_INTERRUPT_HPP_
#define KERNEL_INTERRUPT_HPP_

#include "Object.hpp"
#include "api.Task.hpp"
#include "api.Interrupt.hpp"

namespace module { class Interrupt; }

namespace kernel
{
    class Interrupt : public ::Object<>, public ::api::Interrupt
    {
        typedef ::Object<> Parent;
  
    public:
      
        /** 
         * Constructor.
         */    
        Interrupt();
        
        /** 
         * Constructor.
         *
         * @param handler user class which implements an interrupt handler interface.
         * @param source  available interrupt source.
         */     
        Interrupt(::api::Task& handler, int32 source);
        
        /** 
         * Destructor.
         */
        virtual ~Interrupt();
        
        /**
         * Tests if this object has been constructed.
         *
         * @return true if object has been constructed successfully.
         */    
        virtual bool isConstructed() const;
        
        /**
         * Jumps to interrupt hardware vector.
         */      
        virtual void jump();    
        
        /**
         * Clears an interrupt status of this source.
         */     
        virtual void clear();
        
        /**
         * Sets an interrupt status of this source.
         */    
        virtual void set();
        
        /**
         * Locks this interrupt source.
         *
         * @return an interrupt enable source bit value before method was called.
         */    
        virtual bool disable();
        
        /**
         * Unlocks this interrupt source.
         *
         * @param status returned status by lock method.
         */
        virtual void enable(bool status);
      
    protected:
  
        /** 
         * Returns target processor interrupt interface.
         *
         * @return extended interface.
         */  
        ::module::Interrupt& getDriver() const;
  
    private:
      
        /**
         * Constructor.
         *
         * @param handler pointer to user class which implements an interrupt handler interface.   
         * @param source  available interrupt source.     
         * @return true if object has been constructed successfully.     
         */    
        bool construct(::api::Task* handler, int32 source);
                
        /**
         * Copy constructor.
         *
         * @param obj reference to source object.
         */
        Interrupt(const Interrupt& obj);
      
        /**
         * Assignment operator.
         *
         * @param obj reference to source object.
         * @return reference to this object.     
         */
        Interrupt& operator =(const Interrupt& obj);
        
        /**
         * The module initialized falg value.
         */
        static const int32 IS_INITIALIZED = 0x15798351;    
        
        /**
         * The module initialization stage (no boot).
         */
        static int32 stage_;
        
        /** 
         * The root object constructed flag.
         */  
        const bool& isConstructed_;    
      
        /**
         * Extended interrupt controller interface.
         */    
        ::module::Interrupt* driver_;
  
    };
}
#endif // KERNEL_INTERRUPT_HPP_
