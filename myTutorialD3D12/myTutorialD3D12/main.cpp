#include <windows.h>


//below 4 code are used to use console and windows tgt without DOS window ,
//also, output printf in program to Windows console
#pragma comment( linker, "/subsystem:windows /entry:WinMainCRTStartup" )
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

#pragma comment( linker, "/subsystem:console /entry:mainCRTStartup" )
#pragma comment( linker, "/subsystem:console /entry:WinMainCRTStartup" )



//LPCWSTR is a unicode with 2 char 
//a pointer points to wchar not char
LPCWSTR m_applicationName;

//handle application instance
HINSTANCE m_hinstance;

//define a global handle window, handle main window

//handle window, HWND is basic type like int
//like a id
HWND m_hwnd;

//if ESC pressed or not
bool bexit = false;

//initalize window, create application window
void InitializeWindows(int& screenWidth, int& screenHeight);

//call funtions of initialized windows and accept other port var
bool Initialize();

//manage message loop
void Run();

//close the window
void ShutdownWindows();

//funtion that call-back(call the funtion again) windows
//LRESULT = return long(<--type) result
//HWND = handle windows
//UINT = unsign int
//WPARAM = control identifier
//LPARAM = notification messages

//conclusion: funtion(windowshandle,msg,msg_car,msgvar)
//handle msgs
static LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//if fullscreen or not
const bool FULL_SCREEN = false;

//windows app are event-driven
//WinMain is entrance funtion of app
//HInstance = adress, load the excutable program image to process address space
//HPreInstance = should be NULL,keep for 16-bit windows
//pSCmdLine = pointer points to cmd get command without execute file name
//nCmdShow = SW_HIDE?SW_MINIMIZE?,default SW_SHOW

//application main funtion
//initialize -> set windows height,width
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	Initialize();
	Run();
	ShutdownWindows();
	return 0;

}

//get windows initialize funtion
//only get initialize windows funtion in this e.g.
bool Initialize() {
	int screenWidth = 0, screenHeight = 0;

	//initialize windows
	InitializeWindows(screenWidth, screenHeight);

	return true;

}

void InitializeWindows(int& screenWidth, int& screenHeight) {

	//WNDCLASS = windows kind stat
	WNDCLASSEX wc;
	//DEVMODE = initialize kind and printer environment msg
	DEVMODE dmScreenSetting;
	int posX, posY;

	//get instance handle
	m_hinstance = GetModuleHandle(NULL);

	//application name
	m_applicationName = L"Engine";

	//set windows type var
	//CS_HREDRAW = horizontal width change, redraw window
	//CS_VREDRAW = vertical width change, redraw window
	//CS_OWNDC = own the device discription list
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	//after set the call-back vars,
	//windows will auto call-back them,
	//to process msgs
	wc.lpfnWndProc = WndProc; //pointer points to windows process
	wc.cbClsExtra = 0;//after construct(create) windows type,extra bytes,defalut 0
	wc.cbWndExtra = 0;//after instant, extra bytes, defalut 0, if use WNDCLASS, cbWndExtra must = DLGWNDOWEXTRA
	wc.hInstance = m_hinstance;//include handle instant, include windows process
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;//set icon,if NULL the windows provide
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);//mouse pointer handler,must select pointer resources handler,if NULL,app must set the pointer type
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//set black a windows default bg color
	wc.lpszMenuName = NULL;//pointer points to \0 end which discribe menu, if NULL, no menu
	wc.lpszClassName = m_applicationName;//pointer points to \0 end,discribe class type name
	wc.cbSize = sizeof(WNDCLASSEX);//define size
	
	//register window type
	RegisterClassEx(&wc);

	//get windows resolution
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//see if full screen and give a different resolution
	if (FULL_SCREEN) {
		DEVMODE dmScreenSettings;//device mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSetting));//condition memory
		dmScreenSettings.dmSize = sizeof(dmScreenSetting);//DEVMODE's size
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;//width
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;//height
		dmScreenSettings.dmBitsPerPel = 32;//every pixels depth of color
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;//try to set the display mode and return

		//temporary set device as full screen, warn:after app exit,return to system default
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//set window left hand corner as (0,0)
		posX = posY = 0;
	}
	else {

		//windows set 800*600
		screenWidth = 800;
		screenHeight = 600;

		//set windows left hand corner (posX,posY)
		//Get windows width and height
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

	}

	//full screen and windows use different vars
	/*CreateWindowEx(Dword dwExStyle, extend window style
					 LPCTSTR lpClassName, pointer registered class name
					 LPCTSTR lpWindowName,pointer to window name
					 DWORD dwStyle, window style
					 int x, horizontal position
					 int y, vertical position
					 int nWidth, window width
					 int nHiehgt, window height
					 HWND hWndParent, handle parent or owner window
					 HMENU hMenu, handle to munu, or child-window identifier
					 HINSTANCE hInstance, handle application instance
					 LPVOID lpParam, pointer to window-creation data
					 
					 
					 */

	if (FULL_SCREEN) {
		m_hwnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			m_applicationName,
			m_applicationName,
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX,
			posY,
			screenWidth,
			screenHeight,
			NULL,
			NULL,
			m_hinstance,
			NULL);

	}
	else {
		m_hwnd = CreateWindowEx(
			WS_EX_APPWINDOW,
			m_applicationName,
			m_applicationName,
			WS_OVERLAPPEDWINDOW,
			posX,
			posY,
			screenWidth,
			screenHeight,
			NULL,
			NULL,
			m_hinstance,
			NULL);

	}

	//show window and focus
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//hiden cursor
	ShowCursor(false);

	return;

};

//we use Run() to response and dispatch windows msg
//and also call render()
void Run() {
	MSG msg;
	bool done, result = 1;

	//initialize msg structure
	ZeroMemory(&msg, sizeof(MSG));
	
	//loop to handle msg
	done = false;
	while (!done) {
		//handle windows msg
		//PeekMessage(pointer to msg,windows name, msg range, msg range
		//if 2,3,4 = NULL/0,return all msg, 
		//if 4 is PM_REMOVE,del msg
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			//translate msg = keyboard to char
			TranslateMessage(&msg);
			//dispatch msg = put translate msg to msg handle function, 
			//the function is pointed, when windows registered
			DispatchMessage(&msg);

		}

		//if receive WM_QUIT, quit app
		if (msg.message == WM_QUIT) {
			
			done = true;

		}
		else {
			//if ESC was pressed, will exit too
			result = bexit;

			//our render or other process put here
			//...
			//...

			if (result) {

				done = true;
			
			}

		}

	}

	return;

}

//WndProc() is windows call-back function, msg handles here
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	
	switch (umessage){

	//windows destroy msg
		case WM_DESTROY: {
	
		//PostQuitMessage(exit code)
		PostQuitMessage(0);
		return 0;
	
		}

		default: {
		
			return MessageHandler(hwnd, umessage, wparam, lparam);
	
		}
	}
}

LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {

	switch (umsg) {

		//detect key press msg
	case WM_KEYDOWN: {
		//VK_ESCAPE = ESC
		if (wparam == VK_ESCAPE) {

			bexit = true;
		
		}

		return 0;
	
	}

		default: {

		return DefWindowProc(hwnd, umsg, wparam, lparam);
	
		}

	}

}

//ShutdownWindows() release some resources after program shutdown

void ShutdownWindows() {

	//show cursor
	ShowCursor(true);

	//restore default diplay setting
	if (FULL_SCREEN) {

		ChangeDisplaySettings(NULL, 0);

	}

	//release windows handle
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//release application instance
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	return;
}
