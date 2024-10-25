#ifndef BIT_FLAG_H
#define BIT_FLAG_H


#include "core/object/method_info.h"
#include <cstdint>
#include <initializer_list>
#include <climits>

template <typename TEnum, typename TBitField = uint8_t>
class HBitFlag {
    private:
        TBitField bit_field = 0;
        const TEnum default_state;
        bool has_default : 1 = false; // force it to only take 1 bit of space instead of 4 bytes
    
    private:
        friend class HMethodInfo;
        void set_raw_data(TBitField p_raw_data) {
            this->bit_field = p_raw_data;
        }

    public:
        /**
         * @brief Returns the raw value of the underlying bitfield
         */
        const TBitField &get_raw_data() const { return this->bit_field; }

        /**
         * @brief Returns the number of bits this bit flag can store
         * 
         * @return The number of bits
         */
        const uint8_t get_bitflag_size() const {
            return sizeof(TBitField) * CHAR_BIT;
        }

        /**
         * @brief Returns the state of the given flag in the bitfield
         * 
         * @param p_flag The flag to check
         * @return true if set,
         * @return false otherwise
         */
        const bool is_flag_set(TEnum p_flag) const {
            return this->bit_field & (TBitField)p_flag;
        }

        /**
         * @brief Set the list of flags in the bitfield. If a flag in the list is already set, it is ignored.
         * 
         * Since this uses std::initializer_list, you pass in a list like {Flag1, Flag2, Flag3} without needing
         * to worry about using a vector or an actual list object
         * 
         * @param p_flags the list of flags to set
         */
        void set_flags(std::initializer_list<TEnum> p_flags) {
            for(auto flag : p_flags) {
                this->set_flag(flag);
            }
        }

        /**
         * @brief Set a single flag in the bitfield. If it's already set, it is ignored
         * 
         * @param p_flag the flag to set.
         */
        void set_flag(TEnum p_flag) {
            if(!this->is_flag_set(p_flag)) {
                this->bit_field |= (TBitField)p_flag; //Bitwise OR to set
            }
        }

        /**
         * @brief Clears the list of flags in the bitfield. If a flag in the list is not set, it is ignored.
         * 
         * Since this uses std::initializer_list, you pass in a list like {Flag1, Flag2, Flag3} without needing
         * to worry about using a vector or an actual list object
         * 
         * @param p_flags the list of flags to clear
         */
        void clear_flags(std::initializer_list<TEnum> p_flags) {
            for(auto flag : p_flags) {
                this->clear_flag(flag);
            }
        }

        /**
         * @brief Clears a single flag in the bitfield. If it's not set, it is ignored
         * 
         * @param p_flag the flag to clear
         */
        void clear_flag(TEnum p_flag) {
            if(this->is_flag_set(p_flag)) {
                this->bit_field &= ~(TBitField)p_flag; //Bitwise AND to clear
            }
        }

        /**
         * @brief Resets the bitfield to the default state, if it has one. If it doesn't, it is reset to 0
         */
        void reset_flags() {
            this->bit_field = 0;

            if(this->has_default) {
                this->set_flag(this->default_state);
            }
        }

        /**
         * @brief Construct a new HBitFlag object with no default state set
         */
        HBitFlag() = default;

        /**
         * @brief Construct a new HBitFlag object with a default state. 
         * 
         * @param p_default_state The state to default to if HBitFlag::reset_flags() is called
         */
        HBitFlag(TEnum p_default_state) : default_state(p_default_state) {
            this->has_default = true;
            this->set_flag(p_default_state);
        }

};

#endif // BIT_FLAG_H