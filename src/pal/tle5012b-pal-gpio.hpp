#ifndef TLE5012B_PAL_GPIO_HPP_
#define TLE5012B_PAL_GPIO_HPP_

#include "../corelib/Tle5012b.h"

class Tle5012b::GPIO
{    
    public:  
        
        /**
         * @name   Interrupt event
         * @{
         */
        enum IntEvent_t
        {
            INT_FALLING_EDGE   = 0,     /**< Interrupt on falling edge */
            INT_RISING_EDGE    = 1,     /**< Interrupt on rising edge */
        };
        /** @} */
        
        /**
         * @name    Voltage level
         * @{
         */
        enum VLevel_t
        {
            GPIO_LOW   = 0,        /**< Level low */
            GPIO_HIGH  = 1         /**< Level high */
        };        
        /** @} */      
        
        /**
         * @name    Voltage logic
         * @{
         */
        enum VLogic_t
        {
            NEGATIVE = 1,       /**< Negative logic. 0 on voltage level high, 1 on voltage low */
            POSITIVE = 2,       /**< Positive logic. 1 on voltage level high, 0 on voltage low */
        };
        /** @} */

        /**
         * @brief       Initializes the GPIO
         * @return      GPIO error code
         * @retval      OK if success
         * @retval      INTF_ERROR if initialization error
         */
        virtual Tle5012b::Error_t       init        () = 0;
        
        /**
          * @brief       Deinitializes the GPIO
          * @return      GPIO error code
          * @retval      OK if success
          * @retval      INTF_ERROR if deinitialization error
         */
         virtual Tle5012b::Error_t      deinit      () = 0;

        /**
         * @brief       Enables the hardware interrupt
         * @param[in]   *ptr HallSwith object pointer which holds the interrupt
         * @return      GPIO error code
         * @retval      OK if success
         * @retval      INTF_ERROR if initialization error
         */
        virtual Tle5012b::Error_t       enableInt   (Tle5012b *ptr) = 0;
        
        /**
         * @brief       Disables the hardware interrupt
         * @return      GPIO error code
         * @retval      OK if success
         * @retval      INTF_ERROR if disable error
         */
        virtual Tle5012b::Error_t       disableInt  () = 0;

        /**
         * @brief       Gets the latest interrupt event 
         * @return      GPIO interrupt event
         * @retval      INT_FALLING_EDGE if falling edge event
         * @retval      INT_RISING_EDGE if rising edge event
         */
        virtual IntEvent_t              intEvent    () = 0;
        
        /**
         * @brief       Reads the GPIO voltage level
         * @return      GPIO voltage level
         * @retval      GPIO_LOW if voltage low
         * @retval      GPIO_HIGH if voltage high
         */
        virtual VLevel_t                read        () = 0;
        
        /**
         * @brief       Writes the GPIO output voltage level
         * @param[in]   level  Voltage level
         * @return      GPIO error code
         * @retval      OK if success
         * @retval      INTF_ERROR if write error
         */
        virtual Tle5012b::Error_t       write       (VLevel_t level) = 0;

        /**
         * @brief       Enables the GPIO output according to the GPIO logic
         *              - Low if negative
         *              - High if positive
         * @return      GPIO interrupt event
         * @retval      OK if success
         * @retval      INTF_ERROR if write error
         */
        virtual Tle5012b::Error_t       enable      () = 0;

        /**
         * @brief       Disables the GPIO output according to the GPIO logic
         *              - Low if positive
         *              - High if negative
         * @return      GPIO error code
         * @retval      OK if success
         * @retval      INTF_ERROR if write error
         */
        virtual Tle5012b::Error_t       disable     () = 0;
};

/** @} */

#endif /** TLE5012B_PAL_GPIO_HPP_ **/
