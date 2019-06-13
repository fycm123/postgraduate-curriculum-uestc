#include<windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam ); 

int CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
	WNDCLASS wndClass;
	wndClass.style=CS_HREDRAW|CS_VREDRAW;
	wndClass.lpfnWndProc=WindowProc;
	wndClass.cbClsExtra=0;
	wndClass.cbWndExtra=0;
	wndClass.hInstance=hInstance;
	wndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wndClass.lpszMenuName=NULL;
	wndClass.lpszClassName="主程序界面";
	RegisterClass(&wndClass);
	HWND hwnd=CreateWindow(
		wndClass.lpszClassName,
		TEXT("逆向工程-201822090316-杨庆"),
		WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		400,
		300,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	ShowWindow(hwnd,SW_SHOW);
	UpdateWindow(hwnd);
	MSG msg;
	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam ){
	static HWND hEditUserName;
	static HWND hEditPassword;
	static HWND hButtonLogin;
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	switch(uMsg){
		case WM_CREATE:
			hButtonLogin=CreateWindow(TEXT("button"),TEXT("确认"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
			170,130,60,40,hwnd,NULL,((LPCREATESTRUCT)lParam)->hInstance,NULL);
		hEditUserName=CreateWindow(TEXT("edit"),NULL,WS_CHILD|WS_VISIBLE|ES_LEFT|WS_BORDER,
		130,55,150,25,hwnd,NULL,((LPCREATESTRUCT)lParam)->hInstance,NULL);
		hEditPassword=CreateWindow(TEXT("edit"),NULL,WS_CHILD|WS_VISIBLE|ES_LEFT|WS_BORDER|ES_PASSWORD,
		130,85,150,25,hwnd,NULL,((LPCREATESTRUCT)lParam)->hInstance,NULL);
		return 0;
		case WM_PAINT:
			hdc=BeginPaint(hwnd,&ps);
		rect.left=150;
		rect.top=100;
		rect.right=50;
		rect.bottom=30;
		DrawText(hdc,TEXT("破解账号："),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		rect.left=150;
		rect.top=160;
		rect.right=50;
		rect.bottom=30;
		DrawText(hdc,TEXT("破解密码："),-1,&rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		return 0;
		case WM_COMMAND:
			if((HWND)lParam==hButtonLogin){
			char userName[20];
			char password[20];
			GetWindowText(hEditUserName,userName,20);
			GetWindowText(hEditPassword,password,20);
			if(strcmp(userName,"杨庆")==0&&strcmp(password,"201822090316")==0){
				MessageBox(hwnd,TEXT("破解成功！"),TEXT("恭喜您："),MB_OK);
			}else{
				MessageBox(hwnd,TEXT("破解失败！"),TEXT("很抱歉："),MB_OK);
			}
		}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc (hwnd, uMsg, wParam, lParam) ;
}
