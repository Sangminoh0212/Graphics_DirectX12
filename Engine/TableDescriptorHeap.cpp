#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	_groupCount = count;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	// 5���� �������� ����ϱ� ������ Heap 5�� ����

	// NumDescriptors : ������ �ϴ� view�� ����
	// 5���� ��������(�� Heap�ȿ� view 5��) * Heap ���� 5�� = 25�� view �ʿ�
	// flag : SHADER_VISIBLE = GPU register�� ���� ����
	// �뵵 : CBV
	desc.NumDescriptors = count * REGISTER_COUNT;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	// DH ����
	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// �� heap�� ������
	_groupSize = _handleSize * REGISTER_COUNT;
}

// �� �����Ӹ��� �ʱ�ȭ
void TableDescriptorHeap::Clear()
{
	_currentGroupIndex = 0;
}

// srcHandle�� �ִ� ������ destHandle�� ����
void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	// destHandle�� ��ġ get
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	// constantBufer�� ���� CBV�� �� �ڵ�(srcHandle)���� ���� ���� ����� DH handle(destHandle)�� �ű�
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

// DH���� GPU Register�� �˸´� Heap(table)�� �ø��� �۾�
void TableDescriptorHeap::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	// ���� �� groupIndex�� ��ġ�ϴ� table�� �ø�(=���� ����ϰ� �ִ� ĭ�� �ּҸ� ���Ͽ�)
	handle.ptr += _currentGroupIndex * _groupSize;
	CMD_LIST->SetGraphicsRootDescriptorTable(0, handle);
	// ����Ͽ����� ���� groupIndex�� �̵�
	_currentGroupIndex++;
}


// GetCpuHandle : DH�� _currentGroupIndex��° group, reg��° handle�� ��ȯ
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint32>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint32 reg)
{
	// ���� handle�� �����ͼ�
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	// group��ŭ �̵�(group index, �)
	handle.ptr += _currentGroupIndex * _groupSize;
	// �������� ��ŭ �̵�(handle index, ��ȣ)
	handle.ptr += reg * _handleSize;
	return handle;
}