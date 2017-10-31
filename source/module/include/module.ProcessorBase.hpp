/** 
 * Target processor resource. 
 *
 * The class describes a skeleton implementation of a processor resource.
 * It should be inherited by all classes, which aim expanding the implementation. 
 * 
 * @author    Sergey Baigudin, sergey@baigudin.software
 * @copyright 2017, Embedded Team, Sergey Baigudin
 * @license   http://embedded.team/license/
 */
#ifndef MODULE_PROCESSOR_BASE_HPP_
#define MODULE_PROCESSOR_BASE_HPP_

#include "Object.hpp"
#include "module.Processor.hpp"

namespace module
{
    class ProcessorBase : public ::Object<>, public ::module::Processor
    {
        typedef ::Object<>  Parent;
      
    public:
    
        /** 
         * Constructor.
         */     
        ProcessorBase() : Parent(),
            isConstructed_ (getConstruct()){
        }    
        
        /** 
         * Destructor.
         */                               
        virtual ~ProcessorBase()
        {
        }
        
        /**
         * Tests if this object has been constructed.
         *
         * @return true if object has been constructed successfully.
         */    
        virtual bool isConstructed() const
        {
            return isConstructed_;
        }
      
    protected:
      
        /** 
         * The root object constructed flag.
         */  
        const bool& isConstructed_;
      
    private:
    
        /**
         * Copy constructor.
         *
         * @param obj reference to source object.
         */
        ProcessorBase(const ProcessorBase& obj);
        
        /**
         * Assignment operator.
         *
         * @param obj reference to source object.
         * @return reference to this object.     
         */
        ProcessorBase& operator =(const ProcessorBase& obj);      
  
    };
}
#endif // MODULE_PROCESSOR_BASE_HPP_