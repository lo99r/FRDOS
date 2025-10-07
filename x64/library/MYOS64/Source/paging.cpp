#include <MYOS64>

#define PRESENT_BIT     (1ULL << 0)
#define WRITABLE_BIT    (1ULL << 1)
#define USER_BIT        (1ULL << 2)
#define PWT_BIT         (1ULL << 3)
#define PCD_BIT         (1ULL << 4)
#define PAGE_SIZE_BIT   (1ULL << 7)
#define XD_BIT          (1ULL << 63)
#define ADDRESS_MASK    0x000FFFFFFFFFF000ULL

#pragma region PagingEntry functions

void PagingEntry::SetPresent(bool enable) {
	if (enable) raw_table |= PRESENT_BIT;
	else        raw_table &= ~PRESENT_BIT;
}

void PagingEntry::SetWritable(bool enable) {
	if (enable) raw_table |= WRITABLE_BIT;
	else        raw_table &= ~WRITABLE_BIT;
}

void PagingEntry::SetUserAccessible(bool enable) {
	if (enable) raw_table |= USER_BIT;
	else        raw_table &= ~USER_BIT;
}

void PagingEntry::SetExecutable(bool enable) {
	if (!enable) raw_table |= XD_BIT;  // ���� ����
	else         raw_table &= ~XD_BIT;
}

void PagingEntry::SetCacheType(PagingCacheType type) {
	// ĳ�� ���� ��Ʈ �ʱ�ȭ
	raw_table &= ~(PWT_BIT | PCD_BIT);

	switch (type) {
	case PagingCacheType::WriteThrough:
		raw_table |= PWT_BIT;
		break;
	case PagingCacheType::Disable:
		raw_table |= PCD_BIT;
		break;
	default:
		break; // WriteBack�� ��Ʈ ���� ����
	}
}

void PagingEntry::SetType(PagingType type) {
	if (type == PagingType::PhysicalAddress)
		raw_table |= PAGE_SIZE_BIT;
	else
		raw_table &= ~PAGE_SIZE_BIT;
}

void PagingEntry::SetAddress(uint64 address) {
	raw_table &= ~ADDRESS_MASK; // ���� �ּ� ����
	raw_table |= (address & ADDRESS_MASK); // �� �ּ� ����
}

void PagingEntry::Initialize(uint64 _address, PagingType _type, PagingCacheType _cashtype = PagingCacheType::WriteBack, bool _present = true, bool _writable = true, bool _user_valid = true, bool _excutable = true) {
	uint64 entry = 0;

	// �ʼ� �÷��� ����
	if (_present)    entry |= PRESENT_BIT;
	if (_writable)   entry |= WRITABLE_BIT;
	if (_user_valid) entry |= USER_BIT;

	// ĳ�� �Ӽ� ����
	switch (_cashtype) {
	case PagingCacheType::WriteThrough: entry |= PWT_BIT; break;
	case PagingCacheType::Disable:      entry |= PCD_BIT; break;
	case PagingCacheType::WriteBack:    /* �⺻�� - �ƹ� �͵� �� �� */ break;
	}

	// ������ Ÿ��: TablePointer or PhysicalAddress
	if (_type == PagingType::PhysicalAddress) {
		entry |= PAGE_SIZE_BIT; // ������ ũ�� ��� �ø� ����
	}

	// ���� ���� ��Ʈ (XD)
	if (!_excutable)
		entry |= XD_BIT;

	// �ּ� ����
	entry |= (_address & ADDRESS_MASK);

	raw_table = entry;
}

bool PagingEntry::IsPresent() const {
	return (raw_table & PRESENT_BIT);
}

bool PagingEntry::IsWritable() const {
	return (raw_table & WRITABLE_BIT);
}

bool PagingEntry::IsUserAccessible() const {
	return (raw_table & USER_BIT);
}

bool PagingEntry::IsExecutable() const {
	return !(raw_table & XD_BIT);
}

PagingType PagingEntry::GetType() const {
	if (raw_table & PAGE_SIZE_BIT)
		return PagingType::PhysicalAddress;
	else
		return PagingType::TablePointer;
}

PagingCacheType PagingEntry::GetCacheType() const {
	bool pwt = (raw_table & PWT_BIT);
	bool pcd = (raw_table & PCD_BIT);

	if (!pwt && !pcd) return PagingCacheType::WriteBack;
	if (pwt && !pcd)  return PagingCacheType::WriteThrough;
	if (!pwt && pcd)  return PagingCacheType::Disable;

	// ������ ���� (�� �� set)�� ��쿡�� Disable�� ó��
	return PagingCacheType::Disable;
}

uint64 PagingEntry::GetAddress() const {
	return (raw_table & ADDRESS_MASK);
}

#pragma endregion

#pragma region PML4 functions

void PML4::Initialize(uint64 raw_address, uint64 block_count, PagingSize block_size_ = PagingSize::_2MB, PagingCacheType _cashtype = PagingCacheType::WriteBack, bool _present = true, bool _writable = true, bool _user_valid = false, bool _excutable = true) {
	size = block_count;
	block_size = block_size_;

	uint64 level_counts[4] = { 0, 0, 0, 0 };
	uint64 address_multiply[4] = { 0, 0x40000000, 0x200000, 0x1000 };
	level_counts[(int)block_size] = max(1, block_count);
	
	for (int i = (int)block_size - 1; i >= 0; i--) {
		level_counts[i] = (max(0, level_counts[i + 1] - 1) >> 9) + 1;
	}
	
	Level_tables[0] = (PagingEntry*)raw_address;
	
	for (int i = 1; i <= (int)block_size; i++) {
		Level_tables[i] = (PagingEntry*)((uint64)(Level_tables[i - 1] + align_size(level_counts[i - 1] , 0x1000)));

		uint64 now_size = align_size(level_counts[i], 0x1000);
		for (int j = 0; j < now_size; j++) Level_tables[i][j].raw_table = 0;
	}

	for (int i = 0; i <= (int)block_size; i++) {
		for (int j = 0; j < level_counts[i]; j++) {

			if (i == (int)block_size) {
				Level_tables[i][j].Initialize((uint64)address_multiply[(int)block_size] * j, PagingType::PhysicalAddress, _cashtype, _present, _writable, _user_valid, _excutable);
			}

			else {
				Level_tables[i][j].Initialize((uint64)&Level_tables[i + 1][j * 512], PagingType::TablePointer);
			}
		}
	}



}

uint64 PML4::Size() {
	return size;
}

void PML4::Load() {
	Registers regs;

	regs.cr4 = regs.cr4 | (1 << 5);
	regs.cr3 = (uint32)Level_tables[0];
	regs.cr0 = regs.cr0 | (1 << 31);
}

#pragma endregion

#pragma region PagingBlock functions 

PagingBlock::PagingBlock(PagingEntry& paging_entry, PagingSize paging_size) {
	pagingEntry = &paging_entry;
	uint64 sizes[4] = { 512_GB, 1_GB, 2_MB, 4_KB };
	pagingSize = sizes[(int)paging_size];
}

void PagingBlock::Map(uint64 block_number) {
	pagingEntry->SetAddress(block_number * pagingSize);
}

PagingEntry* PagingBlock::GetPagingEntry() {
	return pagingEntry;
}

#pragma endregion