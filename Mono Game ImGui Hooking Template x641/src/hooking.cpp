#include <iostream>

#include "../include/hooking.h"
#include "../render/include/d3d11_vmt_indices.h"
#include "../render/include/rendercore.h"
#include "../memory/include/mem.h"
#include "../render/include/hackrender.h"
#include "../render/include/dx.h"
#include "../MinHook/include/MinHook.h"
#include "../render/include/onrender.h"

HRESULT STDMETHODCALLTYPE Hooking::hookPresent(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags)
{
	//Init swapChain, device, device Context khi bắt đầu hook
	if (!d3d11.swapChain || !d3d11.device) {
		d3d11.initD3DDraw(swapChain);
	}

	//Check xem nếu mất renderTarget thì tạo lại tham chiếu
	if (d3d11.renderTarget == nullptr)
		d3d11.createRenderTarget();

	//Check xem nếu renderTarget != null thì SetRenderTarget cho Frame này
	if (d3d11.renderTarget != nullptr)
		d3d11.deviceContext->OMSetRenderTargets(0, &d3d11.renderTarget, nullptr);
	
	//Khởi tạo ImGui nếu lần đầu hook vào hàm
	if (!render.state.isImguiInit) {
		render.init(d3d11.window, d3d11.device, d3d11.deviceContext);

		render.state.isImguiInit = true;
	}

	//Bật Menu
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		bool& showMenu = render.state.showMenu;
		showMenu = !showMenu;

		//Nếu bật Menu, hook WndProc của game, window được lấy từ OutputWindow của SwapChain hiện tại, tức là SwapChain mà game sử dụng
		//Mục đích cho Menu ImGui cũng nhận được thông điệp từ người dùng, để có thể tương tác
		if (showMenu) {
			render.setHookWndProc(d3d11.window);
		}
		//Nếu không bật Menu, set lại hàm xử lí thông điệp gốc
		else {
			render.setWndProcToOriginal();
		}
	}

	//Hàm main hack
	onRender.onPresent();

	//Vòng lặp, hàm loop() nhận 1 callback void(*)() để render
	if (render.state.showMenu) {
		render.loop(HackRender::render);
	}

	return d3d11.presentOriginal(swapChain, syncInterval, flags);
}

HRESULT STDMETHODCALLTYPE Hooking::hookResizeBuffer(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
{
	//Giải phóng toàn bộ resource cũ của ImGui trước khi reset Buffer
	ImGui_ImplDX11_InvalidateDeviceObjects();
	//Gọi hàm resizeBuffer
	HRESULT hr = d3d11.resizeBufferOriginal(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
	//Nếu thành công, lấy lại tham chiếu tới Back buffer với độ phân giải mới
	if (SUCCEEDED(hr)) {
		d3d11.createRenderTarget();
	}
	//Tạo lại các resource của ImGui
	ImGui_ImplDX11_CreateDeviceObjects();
	return hr;
}

HRESULT STDMETHODCALLTYPE Hooking::hookCreateSwapChain(IDXGIFactory* pThis, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC pDesc, IDXGISwapChain** ppSwapChain)
{
	//Trước khi tạo lại swapChain, giải phóng buffer mà swapChain quản lý, nếu không sẽ lỗi
	d3d11.renderTarget->Release();
	d3d11.renderTarget = NULL;

	//Tạo swapChain
	HRESULT hr = d3d11.createSwapChainOriginal(pThis, pDevice, pDesc, ppSwapChain);

	//Nếu tạo thành công, gán lấy cắp SwapChain mới này và tiếp tục lấy Buffer từ nó để vẽ
	if (SUCCEEDED(hr) && *ppSwapChain)
	{
		d3d11.swapChain = *ppSwapChain;
	}
	return hr;
}

bool Hooking::implementHooking()
{
	//Lấy Vtable của SwapChain, Factory, Device, DeviceContext
	if (!d3d11.getDummyDeviceAndVtable()) {
		return false;
	}

	//Lấy địa chỉ hàm theo index, IXGISwapChainVMT là 1 enum
	d3d11.present = (D3DX11::fnPresent)(d3d11.pVMT[(UINT)IDXGISwapChainVMT::Present]);
	d3d11.resizeBuffer = (D3DX11::fnResizeBuffer)(d3d11.pVMT[(UINT)IDXGISwapChainVMT::ResizeBuffers]);
	d3d11.createSwapChain = (D3DX11::fnCreateSwapChain)d3d11.factoryVMT[10];

	//Cài đặt hook dùng MinHook
	MH_Initialize();
	MH_CreateHook(d3d11.present, &hookPresent, reinterpret_cast<void**>(&d3d11.presentOriginal));
	MH_CreateHook(d3d11.resizeBuffer, &hookResizeBuffer, reinterpret_cast<void**>(&d3d11.resizeBufferOriginal));
	MH_CreateHook(d3d11.createSwapChain, &hookCreateSwapChain, reinterpret_cast<void**>(&d3d11.createSwapChainOriginal));
	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

bool Hooking::unHook()
{
	//Giải phóng các hook
	MH_DisableHook(d3d11.present);
	MH_DisableHook(d3d11.resizeBuffer);
	MH_DisableHook(d3d11.createSwapChain);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	return true;
}

bool Hooking::freeCOM()
{
	//Dereferent counting các COM
	if (d3d11.device) {
		d3d11.device->Release();
		d3d11.device = nullptr;
	}

	if (d3d11.deviceContext) {
		d3d11.deviceContext->Release();
		d3d11.deviceContext = nullptr;
	}

	if (d3d11.swapChain) {
		d3d11.swapChain->Release();
		d3d11.swapChain = nullptr;
	}

	if (d3d11.renderTarget) {
		d3d11.renderTarget->Release();
		d3d11.renderTarget = nullptr;
	}

	return true;
}

bool Hooking::freeGateway()
{
	//if (d3d11.present) {
	//	if (!VirtualFree(d3d11.present, 0, MEM_RELEASE)) return false;
	//}

	//if (d3d11.resizeBuffer) {
	//	if (!VirtualFree(d3d11.resizeBuffer, 0, MEM_RELEASE)) return false;
	//}

	return true;
}
