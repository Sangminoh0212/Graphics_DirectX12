#pragma once
enum class CONSTANT_BUFFER_TYPE : uint8
{
	GLOBAL,
	TRANSFORM,
	MATERIAL,
	END
};

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg, uint32 size, uint32 count);

	void Clear();
	void PushGraphicsData(void* buffer, uint32 size);
	void SetGraphicsGlobalData(void* buffer, uint32 size);
	void PushComputeData(void* buffer, uint32 size);


	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	// GPU RAM�� Buffer. ���� �ϳ��� �ƴ϶� ������ ������
	ComPtr<ID3D12Resource>	_cbvBuffer;
	// CPU->GPU ������ �о� ���� �� ����ϴ� ����
	BYTE* _mappedBuffer = nullptr;
	// �� ����� ũ��, ���� (buffer�� �� �� ���� ���̳�?)
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	// const buffer view(DH)
	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	// �����ϴ� handle
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	// �ڵ� �� ����. ���� �ּҸ� ���� �� ĭ�� ����� �ϴ��� ��Ÿ��
	uint32								_handleIncrementSize = 0;
	// ex) i ��° handle= _cpuHandleBegin + (_handleIncrementSize * i)


	// ����ϰ� �ִ� ������ index ����
	uint32					_currentIndex = 0;

	CBV_REGISTER			_reg = {};
};

