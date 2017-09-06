#ifndef FD_UTIL_H
#define FD_UTIL_H

#include <cstdint>

namespace fd
{
	template <typename FlagBitsType> struct FlagTraits
	{
		enum { allFlags = 0 };
	};

	template <typename BitType, typename MaskType = uint32_t>
	class Flags
	{
	public:
		Flags()
			: m_mask(0)
		{
		}

		Flags(BitType bit)
			: m_mask(static_cast<MaskType>(bit))
		{
		}

		Flags(Flags<BitType> const& rhs)
			: m_mask(rhs.m_mask)
		{
		}

		Flags<BitType> & operator=(Flags<BitType> const& rhs)
		{
			m_mask = rhs.m_mask;
			return *this;
		}

		Flags<BitType> & operator|=(Flags<BitType> const& rhs)
		{
			m_mask |= rhs.m_mask;
			return *this;
		}

		Flags<BitType> & operator&=(Flags<BitType> const& rhs)
		{
			m_mask &= rhs.m_mask;
			return *this;
		}

		Flags<BitType> & operator^=(Flags<BitType> const& rhs)
		{
			m_mask ^= rhs.m_mask;
			return *this;
		}

		Flags<BitType> operator|(Flags<BitType> const& rhs) const
		{
			Flags<BitType> result(*this);
			result |= rhs;
			return result;
		}

		Flags<BitType> operator&(Flags<BitType> const& rhs) const
		{
			Flags<BitType> result(*this);
			result &= rhs;
			return result;
		}

		Flags<BitType> operator^(Flags<BitType> const& rhs) const
		{
			Flags<BitType> result(*this);
			result ^= rhs;
			return result;
		}

		bool operator!() const
		{
			return !m_mask;
		}

		Flags<BitType> operator~() const
		{
			Flags<BitType> result(*this);
			result.m_mask ^= FlagTraits<BitType>::allFlags;
			return result;
		}

		bool operator==(Flags<BitType> const& rhs) const
		{
			return m_mask == rhs.m_mask;
		}

		bool operator!=(Flags<BitType> const& rhs) const
		{
			return m_mask != rhs.m_mask;
		}

		explicit operator bool() const
		{
			return !!m_mask;
		}

		explicit operator MaskType() const
		{
			return m_mask;
		}

	private:
		MaskType  m_mask;
	};

	template <typename BitType>
	Flags<BitType> operator|(BitType bit, Flags<BitType> const& flags)
	{
		return flags | bit;
	}

	template <typename BitType>
	Flags<BitType> operator&(BitType bit, Flags<BitType> const& flags)
	{
		return flags & bit;
	}

	template <typename BitType>
	Flags<BitType> operator^(BitType bit, Flags<BitType> const& flags)
	{
		return flags ^ bit;
	}


	template <typename RefType>
	class Optional
	{
	public:
		Optional(RefType & reference) { m_ptr = &reference; }
		Optional(RefType * ptr) { m_ptr = ptr; }
		Optional(std::nullptr_t) { m_ptr = nullptr; }

		operator RefType*() const { return m_ptr; }
		RefType const* operator->() const { return m_ptr; }
		explicit operator bool() const { return !!m_ptr; }

	private:
		RefType *m_ptr;
	};

	template <typename T>
	class ArrayProxy
	{
	public:
		ArrayProxy(std::nullptr_t)
			: m_count(0)
			, m_ptr(nullptr)
		{}

		ArrayProxy(T & ptr)
			: m_count(1)
			, m_ptr(&ptr)
		{}

		ArrayProxy(uint32_t count, T * ptr)
			: m_count(count)
			, m_ptr(ptr)
		{}

		template <size_t N>
		ArrayProxy(std::array<typename std::remove_const<T>::type, N> & data)
			: m_count(N)
			, m_ptr(data.data())
		{}

		template <size_t N>
		ArrayProxy(std::array<typename std::remove_const<T>::type, N> const& data)
			: m_count(N)
			, m_ptr(data.data())
		{}

		template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
		ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> & data)
			: m_count(static_cast<uint32_t>(data.size()))
			, m_ptr(data.data())
		{}

		template <class Allocator = std::allocator<typename std::remove_const<T>::type>>
		ArrayProxy(std::vector<typename std::remove_const<T>::type, Allocator> const& data)
			: m_count(static_cast<uint32_t>(data.size()))
			, m_ptr(data.data())
		{}

		ArrayProxy(std::initializer_list<T> const& data)
			: m_count(static_cast<uint32_t>(data.end() - data.begin()))
			, m_ptr(data.begin())
		{}

		const T * begin() const
		{
			return m_ptr;
		}

		const T * end() const
		{
			return m_ptr + m_count;
		}

		const T & front() const
		{
			assert(m_count && m_ptr);
			return *m_ptr;
		}

		const T & back() const
		{
			assert(m_count && m_ptr);
			return *(m_ptr + m_count - 1);
		}

		bool empty() const
		{
			return (m_count == 0);
		}

		uint32_t size() const
		{
			return m_count;
		}

		T * data() const
		{
			return m_ptr;
		}

	private:
		uint32_t  m_count;
		T *       m_ptr;
	};
} // namespace fd
#endif // !FD_UTIL_H
