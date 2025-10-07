#pragma once
#include "defines.hpp"

namespace myos {

	// 일반적으로 write back 을 씁니다. Commonly write back used.
	enum class PagingCacheType : uint64 {
		WriteBack = 0b00,
		WriteThrough = 0b01,
		Disable = 0b10
	};

	enum class PagingType : uint64 {
		TablePointer = 0,
		PhysicalAddress = 1
	};

	enum class PagingSize : uint64 {
		_1GB = 1, _2MB = 2, _4KB = 3
	};

	class PagingEntry {

	public:
		uint64 raw_table;

		void Initialize(uint64 _address, PagingType _type, PagingCacheType _cashtype = PagingCacheType::WriteBack, bool _present = true, bool _writable = true, bool _user_valid = true, bool _excutable = true);

		bool IsPresent() const;
		bool IsWritable() const;
		bool IsExecutable() const;
		bool IsUserAccessible() const;

		PagingType GetType() const;
		PagingCacheType GetCacheType() const;
		uint64 GetAddress() const;

		void SetPresent(bool enable);
		void SetWritable(bool enable);
		void SetExecutable(bool enable);
		void SetUserAccessible(bool enable);

		void SetType(PagingType type);
		void SetCacheType(PagingCacheType type);
		void SetAddress(uint64 address);
	};

	class PagingBlock {
	private:
		PagingEntry* pagingEntry;
		uint64 pagingSize;

	public:
		PagingBlock(PagingEntry& paging_entry, PagingSize paging_size);

		void Map(uint64 block_number);
		PagingEntry* GetPagingEntry();
	};

	class PML4 {
	private:

	private:
		PagingSize block_size;
		PagingEntry* Level_tables[4]; // 0 is PML4, 1 is PDTE .....
		uint64 size;

	public:

		uint64 Size();
		void PML4::Initialize(uint64 raw_address, uint64 block_count, PagingSize block_size = PagingSize::_2MB, PagingCacheType _cashtype = PagingCacheType::WriteBack, bool _present = true, bool _writable = true, bool _user_valid = false, bool _excutable = true);
		void Load();

		PagingBlock operator[](uint64 index) {
			PagingBlock pb = PagingBlock(Level_tables[(int)block_size][index], block_size);
			return pb;
		}
	};
}