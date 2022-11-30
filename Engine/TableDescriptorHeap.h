#pragma once


class TableDescriptorHeap
{
public:
	void Init(uint32 count);

	void Clear();
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void CommitTable();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32 reg);

private:
	 
	// DH 자체
	ComPtr<ID3D12DescriptorHeap> _descHeap;
	// handle 넘어갈 때 넘어갈 칸 (=handleIncrementSize)
	uint64					_handleSize = 0;
	// DH의 size (1 DH = group)
	// groupSize = handle 크기 * 데이터 개수
	uint64					_groupSize = 0;
	// DH의 개수 (device!=commandList 실행 속도 차이 때문에 여러개 만들 예정)
	uint64					_groupCount = 0;

	uint32					_currentGroupIndex = 0;
};

