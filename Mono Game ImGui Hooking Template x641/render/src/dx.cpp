#include "../include/dx.h"
#include "../include/d3d11_vmt_indices.h"
#include <iostream>

D3DX11 d3d11;

bool D3DX11::getDummyDeviceAndVtable()
{
	//Cấu hình để có thể tạo Swap Chain
	DXGI_SWAP_CHAIN_DESC sd;

	//Set toàn bộ cấu trúc thành 0 cho đỡ phải khởi tạo từng cái
	ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
	//Tạo 1 buffer
	sd.BufferCount = 1;
	//Báo cho backbuffer biết mục đích của nó dùng để làm gì, trong trường hợp này là để RenderTarget sử dụng để render
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//Định dạng màu cho mỗi pixel trong buffer
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//Lấy HWND của DesktopWindow, dùng ở đây vì cần 1 HWND hợp lệ nhưng không có HWND của game, để kiểu tượng trưng cho dummy
	sd.OutputWindow = GetDesktopWindow();
	//Cờ windowed để set cửa sổ cho window
	sd.Windowed = TRUE;
	//Báo cho BackBuffer biết là sau khi Render xong thì nó sẽ được làm gì, trong trường hợp này là Discard, tức là render xong thì vứt luôn ( hoặc ghi đè), còn có các cái khác
	//Như flip là đảo con trỏ giữa 2 buffer
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//Số lần lấy mẫu để khử răng cưa, 1 là không khử răng cưa
	sd.SampleDesc.Count = 1;

	//2 biến tạm để tạo Device, SwapChain
	ID3D11Device* tmpDevice = nullptr;
	IDXGISwapChain* tmpSwapChain = nullptr;
	//Tạo Device, SwapChain, Context ở đây không được tạo vì mục đích tổng thể của hàm này là lấy Vtable của SwapChain
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &tmpSwapChain, &tmpDevice, nullptr, nullptr);
	if (GetLastError() == 0x594) {
		SetLastError(0);
	}

	if (FAILED(hr)) {
		return false;
	}

	IDXGIFactory* factory = nullptr;
	
	//Lấy VMT của Factory ( tức là toàn bộ Vtable)
	{
		//Tạo con trỏ tới Interface DXGIDevice
		IDXGIDevice* pDXGIDevice = nullptr;
		//Đảo Interface từ D3D11Device sang IDXGIDevice, nó là 1 cây kế thừa, chỉ là ép kiểu từ lớp con lên lớp cha hoặc ngược lại, nhưng về bản chất cùng 1 vùng nhớ
		tmpDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
		//Tạo con trỏ tới Interface DXGIAdapter
		IDXGIAdapter* pAdapter = nullptr;
		//Lấy Adapter, bên trong nó là QueryInterface, về bản chất là đổi Interface thôi, nhưng là lưu vào con trỏ khác chứ không phải device
		pDXGIDevice->GetAdapter(&pAdapter);
		//Lấy Interface IDXGIFactory từ Adapter
		pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory);
		//Copy vtable của factory vào mảng các con trỏ hàm
		memcpy(factoryVMT, *(void***)factory, sizeof(factoryVMT));

		//Giải phóng tài nguyên
		SAFE_RELEASE(pAdapter);
		SAFE_RELEASE(pDXGIDevice);
		SAFE_RELEASE(factory);
	}
	
	//Lấy VMT từ Swapchain
	{
		void** tempVMT = *(void***)tmpSwapChain;
		memcpy(pVMT, tempVMT, sizeof(pVMT));

		SAFE_RELEASE(tmpDevice);
		SAFE_RELEASE(tmpSwapChain);
	}

	return true;
}

bool D3DX11::getDeviceContextRenderTarget()
{
	//Lấy Interface Device
	HRESULT hr = swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
	if (FAILED(hr)) {
		return false;
	}
	//Lấy handle window của game, mà lấy từ SwapChain của game, vì SwapChain có OutputWindow trong cấu hình
	window = getHandleWindow();

	//Lấy Device Context
	device->GetImmediateContext(&deviceContext);
	//Tạo render target nếu chưa có
	if (!createRenderTarget()) {
		return false;
	}

	return true;
}

bool D3DX11::createRenderTarget()
{
	//Tạo 1 con trỏ tới Interface Texture2D
	ID3D11Texture2D* backBuffer = nullptr;
	//Lấy backBuffer do SwapChain của game quản lí
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(hr)) {
		return false;
	}

	//Tạo 1 tham chiếu tới backBuffer đó, gọi là Render Target ( Nơi mà thiết bị sẽ render vào )
	hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTarget);
	//Giảm tham chiếu tới backBuffer đi 1, chứ không phải giải phóng nó trừ khi tham chiếu <= 0
	backBuffer->Release();
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

HWND D3DX11::getHandleWindow()
{
	if (!swapChain) {
		return nullptr;
	}

	//Tạo 1 cấu trúc miêu tả cấu hình của SwapChain
	DXGI_SWAP_CHAIN_DESC sd;
	//Lấy cấu hình của SwapChain hiện tại
	HRESULT hr = swapChain->GetDesc(&sd);
	if (FAILED(hr)) {
		return nullptr;
	}
	//Trong cấu hình có bao gồm window nó đang quản lý và vẽ lên, ta lấy cái đó
	return sd.OutputWindow;
}

bool D3DX11::initD3DDraw(IDXGISwapChain* pSwapChain)
{
	//Gán con trỏ swapChain ta đánh cắp được bằng SwapChain của Present ( bản chất nó là this của Present )
	swapChain = pSwapChain;
	return getDeviceContextRenderTarget();
}
