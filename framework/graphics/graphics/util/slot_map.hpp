#ifndef VG_SLOT_MAP_HPP
#define VG_SLOT_MAP_HPP

#include <set>
#include <unordered_map>

namespace vg
{
    template <typename T>
    class SlotMap {
    public:
        using SlotType = uint32_t;
        using ValueType = T;

        SlotMap();
        Bool32 hasValue(const lotType &slot) const;
        void addValue(const SlotType &slot, const ValueType &value);
        void removeValue(const SlotType &slot);
        ValueType &getValue(const SlotType &slot) const;
        void setValue(const SlotType &slot, const ValueType &value);
        const std::set<SlotType> &getSlots() const;

    private:
        std::set<SlotType> m_slots;
        std::unordered_map<SlotType, ValueType> m_map;
    };
} //vg

#include "graphics/util/slot_map.inl"

#endif //VG_SLOT_MAP_HPP