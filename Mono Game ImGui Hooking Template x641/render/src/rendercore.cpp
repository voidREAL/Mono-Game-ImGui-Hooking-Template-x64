#include "../include/rendercore.h"
#include "../include/dx.h"

Render render;
LONG_PTR Render::originalWndProc = NULL;
HWND Render::window = NULL;

bool Render::init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	//2 biến quản lý trạng thái lõi của ImGui là khởi tạo và showMenu
	state.isImguiInit = false;
	state.showMenu = false;

	//Bật chế độ thích nghi DPI, nghĩa là hình ảnh sẽ được nhân với hệ số nhân tùy thuộc kích thước màn hình
	ImGui_ImplWin32_EnableDpiAwareness();
	float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

	IMGUI_CHECKVERSION();
	//Tạo ngữ cảnh cho ImGui
	ImGui::CreateContext();
	//Cấu hình ImGui trước khi Init
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	//Chỉnh theme thành màu tối
	ImGui::StyleColorsDark();

	//Chỉnh style của ImGui để thích nghi với DPI, nghĩa là scale theo DPI
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;

	//Init ImGui backend
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, deviceContext);

	return true;
}

bool Render::loop(void(*renderCallback)())
{
	//Bắt đầu 1 frame và render
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	renderCallback();

	ImGui::EndFrame();
	ImGui::Render();

	//Set render target để ImGui biết nơi mà vẽ frame hiện tại
	d3d11.deviceContext->OMSetRenderTargets(1, &d3d11.renderTarget, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return true;
}

bool Render::cleanup()
{
	//Cleanup imgui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	return true;
}
//Chuyển tiếp khai báo hàm để có thể sử dụng
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//Hàm hook WndProc
LRESULT __stdcall Render::WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Kiểm tra xem ImGui có handle input không
	if (Render::handleImGuiInput(hWnd, uMsg, wParam, lParam)) {
		return true;
	}

	//Gọi hàm xử lí thông điệp gốc của game
	return CallWindowProc((WNDPROC)originalWndProc, hWnd, uMsg, wParam, lParam);
}

bool Render::handleImGuiInput(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!render.state.isImguiInit) {
		return CallWindowProc((WNDPROC)originalWndProc, hWnd, uMsg, wParam, lParam);
	}
	//Gọi hàm xử lí thông điệp của ImGui
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
		return true;
	}
	//Check xem ImGui có muốn xử lí chuột và bàn phím không hay chuyển tiếp cho hàm gốc của game
	ImGuiIO& io = ImGui::GetIO();
	if (render.state.showMenu && (io.WantCaptureMouse || io.WantCaptureKeyboard))
		return true;
}

bool Render::setHookWndProc(HWND handleWindow, LONG_PTR wndProc /*= (LONG_PTR)WndProcHook*/)
{
	if (!handleWindow) {
		return false;
	}
	window = handleWindow;
	//Set hàm xử lí thông điệp của window nhận vào thành wndProc ( mặc định chính là WndProcHook)
	originalWndProc = SetWindowLongPtr(window, GWLP_WNDPROC, wndProc);
	return true;
}

bool Render::setWndProcToOriginal()
{
	//Set lại hàm xử lí thông điệp của Window thành hàm gốc
	if (window && originalWndProc) {
		SetWindowLongPtr(window, GWLP_WNDPROC, originalWndProc);
		return true;
	}
	else {
		return false;
	}
}
