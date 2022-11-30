#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	// 5개의 레지스터 사용하기 떄문에 Heap 5개 제작

	// NumDescriptors : 만들어야 하는 view의 개수
	// 5개의 레지스터(한 Heap안에 view 5개) * Heap 개수 5개 = 25개 view 필요
	// flag : SHADER_VISIBLE = GPU register에 상주 가능
	// 용도 : CBV
	desc.NumDescriptors = count * REGISTER_COUNT;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	// DH 생성
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// 한 heap의 사이즈
	_groupSize = _handleSize * REGISTER_COUNT;
}

// 매 프레임마다 초기화
void TableDescriptorHeap::Clear()
{
	_currentGroupIndex = 0;
}

// srcHandle에 있는 내용은 destHandle에 복사
void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	// destHandle의 위치 get
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	// constantBufer로 만든 CBV의 각 핸들(srcHandle)들을 꺼내 새로 만드는 DH handle(destHandle)에 옮김
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

// DH에서 GPU Register에 알맞는 Heap(table)를 올리는 작업
void TableDescriptorHeap::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	// 현재 내 groupIndex에 일치하는 table을 올림(=현재 사용하고 있는 칸의 주소를 구하여)
	handle.ptr += _currentGroupIndex * _groupSize;
	CMD_LIST->SetGraphicsRootDescriptorTable(0, handle);
	// 사용하였으니 다음 groupIndex로 이동
	_currentGroupIndex++;
}


// GetCpuHandle : DH의 _currentGroupIndex번째 group, reg번째 handle을 반환
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint32>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint32 reg)
{
	// 시작 handle을 꺼내와서
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	// group만큼 이동(group index, 몇동)
	handle.ptr += _currentGroupIndex * _groupSize;
	// 레지스터 만큼 이동(handle index, 몇호)
	handle.ptr += reg * _handleSize;
	return handle;
}