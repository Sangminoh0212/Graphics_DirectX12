#pragma once
class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync(); // fence를 이용해서 대기

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return _cmdList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return _resCmdList; }
private:
	// CommandQueue : DX12에 등장
	// 외주를 요청할 때, 하나씩 요청하면 비효율적
	// [외주 목록]에 일감을 차곡차곡 기록했다가 한 방에 요청하는 것
	// ID3D12CommandAllocator : 일감 자체를 넣어주기 위한 메모리공간을 할당하는 allocater, 메모리를 계속해서 관리
	// ID3D12GraphicsCommandList : 일감 list
	ComPtr<ID3D12CommandQueue>			_cmdQueue;
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	// 리소스 업로드 용 CmdList
	ComPtr<ID3D12CommandAllocator>		_resCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_resCmdList;

	// Fence : 울타리(?)
	// CPU / GPU 동기화를 위한 간단한 도구
	// 맡긴 부탁이 결과물 반환할 때, 결과물 반환 전까지 기다리기 위해 사용
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>			_swapChain;
};

