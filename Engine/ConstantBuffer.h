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
	// GPU RAM의 Buffer. 버퍼 하나가 아니라 버퍼의 모음집
	ComPtr<ID3D12Resource>	_cbvBuffer;
	// CPU->GPU 데이터 밀어 넣을 때 사용하는 버퍼
	BYTE* _mappedBuffer = nullptr;
	// 각 요소의 크기, 개수 (buffer를 몇 개 만들 것이냐?)
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	// const buffer view(DH)
	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	// 시작하는 handle
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	// 핸들 별 간격. 다음 주소를 위해 몇 칸을 띄워야 하는지 나타냄
	uint32								_handleIncrementSize = 0;
	// ex) i 번째 handle= _cpuHandleBegin + (_handleIncrementSize * i)


	// 사용하고 있는 버퍼의 index 추적
	uint32					_currentIndex = 0;

	CBV_REGISTER			_reg = {};
};

