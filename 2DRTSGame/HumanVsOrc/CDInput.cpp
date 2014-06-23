
/*------------------------------------------------------
|                  CDInput.cpp
|          CDInput类的成员函数定义代码
|             (c)  jk&ljq, 2010
--------------------------------------------------------*/

#include<windows.h>
#include"CDInput.h"
#include<tchar.h>

//缓冲数据格式的缓冲区大小为16
#define BUFFER_SIZE     16

// 立即数据格式键盘按键
#define	KBKEYDOWN(name,key)  \
    if(name[key] & 0x80)\
    {\
        KeyKB[0].Key = key;\
        return S_OK;\
    }

// 缓冲数据格式键盘按键
#define KBBUFFERKEYDOWN(x)  \
    if( didod[ x ].dwData & 0x80 )\
    {\
        KeyKB[0].Key  = didod[ x ].dwOfs;\
        KeyKB[0].Time = didod[ x ].dwTimeStamp;\
        return S_OK;\
    }

// 立即数据格式鼠标按键
#define	MOUSEKEYDOWN(name,key)  \
    if(name & 0x80)\
    {\
        KeyMouse[0].Key = key;\
        return S_OK;\
    }

// 缓冲数据格式鼠标按键
#define	MOUSEBUFFERKEYDOWN(x,B)  \
    if( didod[ x ].dwOfs == B )\
    {\
        if( didod[ x ].dwData & 0x80 )\
        {\
            KeyMouse[0].Key  = didod[ x ].dwOfs;\
            KeyMouse[0].Time = didod[ x ].dwTimeStamp;\
            return S_OK;\
        }\
    }

/***********************************************************
* 函数名 : CDInput(...)
*   功能 : 构造函数，初始化成员变量
*             (c)  jk&ljq, 2010
************************************************************/
CDInput::CDInput( BOOL bEx,
                  BOOL bFor,
                  BOOL bImm,
                  LONG Mouse_Min_X,
                  LONG Mouse_Max_X,
                  LONG Mouse_Min_Y, 
                  LONG Mouse_Max_Y,
                  LONG Mouse_Min_Z, 
                  LONG Mouse_Max_Z )
{
    lpDI         = NULL;
    lpdiMouse    = NULL;
    lpdiKeyboard = NULL;
    bFullScr     = TRUE;

    bExclusive   = bEx;
    bForeGround  = bFor;
    bImmediate   = bImm;

    MouseMin_X   = Mouse_Min_X;
    MouseMax_X   = Mouse_Max_X;

    MouseMin_Y   = Mouse_Min_Y;
    MouseMax_Y   = Mouse_Max_Y;

    MouseMin_Z   = Mouse_Min_Z;
    MouseMax_Z   = Mouse_Max_Z;

    MouseSpeed   = 1;
    M_Z_Speed    = 0.1;

    // 初始化鼠标位置
    GetCursorPos( &KeyMouse[0].Pos );
    GetCursorPos( &KeyMouse[1].Pos );
    KeyMouse[0].lZ = MouseMin_Z;
    KeyMouse[1].lZ = MouseMin_Z;

}

/***********************************************************
* 函数名 : CDInput(...)
*   功能 : 构造函数，初始化成员变量。重载
*             (c)  jk&ljq, 2010
************************************************************/
CDInput::CDInput( void )
{
    lpDI         = NULL;
    lpdiMouse    = NULL;
    lpdiKeyboard = NULL;
    bFullScr     = TRUE;

    bExclusive   = FALSE;
    bForeGround  = TRUE;
    bImmediate   = TRUE;

    MouseMin_X   = 0;
    MouseMax_X   = GetSystemMetrics( SM_CXSCREEN );

    MouseMin_Y   = 0;
    MouseMax_Y   = GetSystemMetrics( SM_CYSCREEN );

    MouseMin_Z   = 0;
    MouseMax_Z   = 360;

    MouseSpeed   = 1;
    M_Z_Speed    = 0.1;

    // 初始化鼠标位置
    GetCursorPos( &KeyMouse[0].Pos );
    GetCursorPos( &KeyMouse[1].Pos );
    KeyMouse[0].lZ = MouseMin_Z;
    KeyMouse[1].lZ = MouseMin_Z;

}

/***********************************************************
* 函数名 : ~CDInput(...)
*   功能 : 析构函数，释放DirectInput对象
*             (c)  jk&ljq, 2010
************************************************************/
CDInput::~CDInput()
{
    FreeDirectInput( 3 );
}

/***********************************************************
* 函数名 : InitDInput(...)
*   功能 : 建立DirectInput环境
*             (c)  jk&ljq, 2010
************************************************************/
void CDInput::InitDInput( HWND hWnd )
{
	//初始化键盘环境
	KeyboardInit( hWnd );

	//初始化鼠标环境
	MouseInit( hWnd );
}

/***********************************************************
* 函数名 : GetDInputData(...)
*   功能 : 获取按键状态
*             (c)  jk&ljq, 2010
************************************************************/
void CDInput::GetDInputData( void )
{
    if( bImmediate ) {
        //获取键盘状态(立即数据)
        GetKeyboardImmediateData();

        //获取鼠标状态(立即数据)
        GetMouseImmediateData();
    }
    else {
        //获取键盘状态(缓冲数据)
        GetKeyboardBufferData();

        //获取鼠标状态(缓冲数据)
        GetMouseBufferData();
    }
}

/***********************************************************
* 函数名 : FreeDirectInput(...)
*   功能 : 释放所有存在的DirectInput对象
*   参数说明:  id 的值范围是1,2,3
*               1  表示释放
*             (c)  jk&ljq, 2010
************************************************************/
void CDInput::FreeDirectInput( UINT id )
{
	if(lpDI){

        // 释放键盘设备
		if( lpdiKeyboard ){
            if( id==1 || id==3 ){
                // Unacquire()总在Release()之前运行
                // 即先丢设备再释放空间
                lpdiKeyboard->Unacquire();
                lpdiKeyboard->Release();
                lpdiKeyboard = NULL;
            }
		}

        // 释放鼠标设备
		if( lpdiMouse ){
            if( id==2 || id==3 ){
                // Unacquire()总在Release()之前运行
                // 即先丢设备再释放空间
                lpdiMouse->Unacquire();
                lpdiMouse->Release();
                lpdiMouse = NULL;
            }
		}

        // 释放DirectInput对象
        lpDI->Release();
        lpDI = NULL;
	}
}


/***********************************************************
* 函数名 : KeyboardInit(...)
*   功能 : 初始化DirectInput键盘设备
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::KeyboardInit( HWND hWnd )
{
    HRESULT hr;
    DWORD   dwCoopFlags;

    // 先释放以前创建的键盘设备
    FreeDirectInput( 1 );

    // 获取协作等级
    if( bExclusive )
        dwCoopFlags = DISCL_EXCLUSIVE;//独占模式
    else
        dwCoopFlags = DISCL_NONEXCLUSIVE;//非独占模式
    if( bForeGround )
        dwCoopFlags |= DISCL_FOREGROUND;//前台操作
    else
        dwCoopFlags |= DISCL_BACKGROUND;//后台操作

    // 第一步 : 创建DirectInput对象
	if(lpDI==NULL){
        HINSTANCE hInst = (HINSTANCE)GetWindowLong( hWnd, GWL_HINSTANCE );
        hr = DirectInput8Create( hInst,
                                 DIRECTINPUT_VERSION, 
                                 IID_IDirectInput8,
                                 ( VOID** )&lpDI, NULL );
        if( FAILED( hr ) ){
            MessageBox( hWnd, _T("创建DirectInput对象出错\n"),
                              _T("DirectInput8Create"), MB_OK );
        	return hr;
        }
    }

    // 第二步 : 创建系统键盘设备
    hr = lpDI->CreateDevice( GUID_SysKeyboard, &lpdiKeyboard, NULL );
    if( FAILED( hr ) ) {
        MessageBox( hWnd, _T("创建系统键盘设备出错\n"),
                          _T("CreateDevice"), MB_OK );
        return hr;
    }

    // 第三步 : 设置键盘数据格式 
    hr = lpdiKeyboard->SetDataFormat( &c_dfDIKeyboard );
    if( FAILED( hr ) ) {
        MessageBox( hWnd, _T("设置键盘数据格式失败\n"),
                          _T("SetDataFormat"), MB_OK );
        return hr;
    }

    // 第四步 : 设置DirectInput的协作等级
    hr = lpdiKeyboard->SetCooperativeLevel( hWnd, dwCoopFlags );
    if( hr == DIERR_UNSUPPORTED )
    {
        FreeDirectInput( 1 );
        MessageBox( hWnd, _T("系统键盘设备不支持设置的协作等级\n"),
                          _T("SetCooperativeLevel"), MB_OK );
        return S_OK;
    }
    else if( FAILED(hr) ) {
        FreeDirectInput( 1 );
        MessageBox( hWnd, _T("设置系统键盘设备的协作等级失败\n"),
                          _T("SetCooperativeLevel"), MB_OK );
        return hr;
    }

    // 第五步(可选) : 设置键盘缓冲数据方式的缓冲区大小
    if( !bImmediate )
    {
        DIPROPDWORD dipdw;
        dipdw.diph.dwSize       = sizeof( DIPROPDWORD );
        dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = BUFFER_SIZE;

        hr = lpdiKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
        if( FAILED( hr ) ){
            MessageBox( hWnd, _T("设置键盘缓冲数据方式的缓冲区大小失败\n"),
                              _T("SetProperty"), MB_OK );
            return hr;
        }
    }

    // 第六步 : 获取键盘设备
    lpdiKeyboard->Acquire();

    return S_OK;
}

/***********************************************************
* 函数名 : GetKeyboardImmediateData(...)
*   功能 : 获取键盘即时输入数据
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::GetKeyboardImmediateData( void )
{
    // 键盘设备无效则返回
    if( lpdiKeyboard==NULL ) 
        return S_OK;

    HRESULT hr;

    // 存放键盘输入状态的缓冲区
    BYTE    Buffer[256];
    ZeroMemory( Buffer, sizeof(Buffer) );

    // 获取键盘设备的输入状态
    hr = lpdiKeyboard->GetDeviceState( sizeof(Buffer), Buffer );
    if( FAILED( hr ) )  {
        // 丢失了键盘设备，则重新获取
        hr = lpdiKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = lpdiKeyboard->Acquire();
        return hr;
    }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // 保存上一次的按键状态
	KeyKB[1].Key = KeyKB[0].Key;

    // 接收按键状态
	KBKEYDOWN( Buffer,DIK_ESCAPE )      // "Esc"键被按下
	KBKEYDOWN( Buffer,DIK_GRAVE )       // "`"键被按下
	KBKEYDOWN( Buffer,DIK_F1 )          // F1键被按下
	KBKEYDOWN( Buffer,DIK_F2 )          // F2键被按下
	KBKEYDOWN( Buffer,DIK_F3 )          // F3键被按下
	KBKEYDOWN( Buffer,DIK_F4 )          // F4键被按下
	KBKEYDOWN( Buffer,DIK_F5 )          // F5键被按下
	KBKEYDOWN( Buffer,DIK_F6 )          // F6键被按下
	KBKEYDOWN( Buffer,DIK_F7 )          // F7键被按下
	KBKEYDOWN( Buffer,DIK_F8 )          // F8键被按下
	KBKEYDOWN( Buffer,DIK_F9 )          // F9键被按下
	KBKEYDOWN( Buffer,DIK_F10 )         // F10键被按下
	KBKEYDOWN( Buffer,DIK_F11 )         // F11键被按下
	KBKEYDOWN( Buffer,DIK_F12 )         // F12键被按下
	KBKEYDOWN( Buffer,DIK_F13 )         // F13键被按下
	KBKEYDOWN( Buffer,DIK_F14 )         // F14键被按下
	KBKEYDOWN( Buffer,DIK_F15 )         // F15键被按下
	KBKEYDOWN( Buffer,DIK_1 )           // "1"键被按下
	KBKEYDOWN( Buffer,DIK_1 )           // "1"键被按下
	KBKEYDOWN( Buffer,DIK_2 )           // "2"键被按下
	KBKEYDOWN( Buffer,DIK_3 )           // "3"键被按下
	KBKEYDOWN( Buffer,DIK_4 )           // "4"键被按下
	KBKEYDOWN( Buffer,DIK_5 )           // "5"键被按下
	KBKEYDOWN( Buffer,DIK_6 )           // "6"键被按下
	KBKEYDOWN( Buffer,DIK_7 )           // "7"键被按下
	KBKEYDOWN( Buffer,DIK_8 )           // "8"键被按下
	KBKEYDOWN( Buffer,DIK_9 )           // "9"键被按下
	KBKEYDOWN( Buffer,DIK_0 )           // "0"键被按下
	KBKEYDOWN( Buffer,DIK_A )           // "A"键被按下
	KBKEYDOWN( Buffer,DIK_B )           // "B"键被按下
	KBKEYDOWN( Buffer,DIK_C )           // "C"键被按下
	KBKEYDOWN( Buffer,DIK_D )           // "D"键被按下
	KBKEYDOWN( Buffer,DIK_E )           // "E"键被按下
	KBKEYDOWN( Buffer,DIK_F )           // "F"键被按下
	KBKEYDOWN( Buffer,DIK_G )           // "G"键被按下
	KBKEYDOWN( Buffer,DIK_H )           // "H"键被按下
	KBKEYDOWN( Buffer,DIK_I )           // "I"键被按下
	KBKEYDOWN( Buffer,DIK_J )           // "J"键被按下
	KBKEYDOWN( Buffer,DIK_K )           // "K"键被按下
	KBKEYDOWN( Buffer,DIK_L )           // "L"键被按下
	KBKEYDOWN( Buffer,DIK_M )           // "M"键被按下
	KBKEYDOWN( Buffer,DIK_N )           // "N"键被按下
	KBKEYDOWN( Buffer,DIK_O )           // "O"键被按下
	KBKEYDOWN( Buffer,DIK_P )           // "P"键被按下
	KBKEYDOWN( Buffer,DIK_Q )           // "Q"键被按下
	KBKEYDOWN( Buffer,DIK_R )           // "R"键被按下
	KBKEYDOWN( Buffer,DIK_S )           // "S"键被按下
	KBKEYDOWN( Buffer,DIK_T )           // "T"键被按下
	KBKEYDOWN( Buffer,DIK_U )           // "U"键被按下
	KBKEYDOWN( Buffer,DIK_V )           // "V"键被按下
	KBKEYDOWN( Buffer,DIK_W )           // "W"键被按下
	KBKEYDOWN( Buffer,DIK_X )           // "X"键被按下
	KBKEYDOWN( Buffer,DIK_Y )           // "Y"键被按下
	KBKEYDOWN( Buffer,DIK_Z )           // "Z"键被按下
	KBKEYDOWN( Buffer,DIK_LBRACKET )    // "["键被按下
	KBKEYDOWN( Buffer,DIK_RBRACKET )    // "]"键被按下
	KBKEYDOWN( Buffer,DIK_LSHIFT )      // 左"Shift"键被按下
	KBKEYDOWN( Buffer,DIK_RSHIFT )      // 右"Shift"键被按下
	KBKEYDOWN( Buffer,DIK_LCONTROL )    // 左"Ctrl"键被按下
	KBKEYDOWN( Buffer,DIK_RCONTROL )    // 右"Ctrl"键被按下
	KBKEYDOWN( Buffer,DIK_LMENU )       // 左"Alt"键被按下
	KBKEYDOWN( Buffer,DIK_RMENU )       // 右"Alt"键被按下
	KBKEYDOWN( Buffer,DIK_MINUS )       // "_"键被按下
	KBKEYDOWN( Buffer,DIK_EQUALS )      // "="键被按下
	KBKEYDOWN( Buffer,DIK_COMMA )       // ","键被按下
	KBKEYDOWN( Buffer,DIK_PERIOD )      // "."键被按下
	KBKEYDOWN( Buffer,DIK_SLASH )       // "/"键被按下
	KBKEYDOWN( Buffer,DIK_RETURN )      // 回车键被按下
	KBKEYDOWN( Buffer,DIK_SPACE )       // 空格键被按下
	KBKEYDOWN( Buffer,DIK_TAB )         // "Tab"键被按下
	KBKEYDOWN( Buffer,DIK_BACK )        // "Back Space"键被按下

	KBKEYDOWN( Buffer,DIK_WAKE )        // "Wake Up"键被按下
	KBKEYDOWN( Buffer,DIK_POWER )       // "Power"键被按下
	KBKEYDOWN( Buffer,DIK_SLEEP )       // "Sleep"键被按下
	KBKEYDOWN( Buffer,DIK_SCROLL )      // "Scroll Lock"键被按下
	KBKEYDOWN( Buffer,DIK_PAUSE )       // "Pause Break"键被按下
	KBKEYDOWN( Buffer,DIK_INSERT )      // "Insert"键被按下
	KBKEYDOWN( Buffer,DIK_DELETE )      // "Delete"键被按下
	KBKEYDOWN( Buffer,DIK_HOME )        // "Home"键被按下
	KBKEYDOWN( Buffer,DIK_END )         // "End"键被按下
	KBKEYDOWN( Buffer,DIK_PGUP )       // "Page Up"键被按下
	KBKEYDOWN( Buffer,DIK_PGDN )        // "Page Down"键被按下
	KBKEYDOWN( Buffer,DIK_UP )          // "↑"键被按下
	KBKEYDOWN( Buffer,DIK_DOWN )        // "↓"键被按下
	KBKEYDOWN( Buffer,DIK_LEFT )        // "←"键被按下
	KBKEYDOWN( Buffer,DIK_RIGHT )       // "→"键被按下

	// 小键盘按键状态
	KBKEYDOWN( Buffer,DIK_NUMLOCK )     // "NumLock"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD0 )     // 小键盘"0"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD1 )     // 小键盘"1"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD2 )     // 小键盘"2"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD3 )     // 小键盘"3"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD4 )     // 小键盘"4"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD5 )     // 小键盘"5"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD6 )     // 小键盘"6"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD7 )     // 小键盘"7"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD8 )     // 小键盘"8"键被按下
	KBKEYDOWN( Buffer,DIK_NUMPAD9 )     // 小键盘"9"键被按下
	KBKEYDOWN( Buffer,DIK_SUBTRACT )    // 小键盘"-"键被按下
	KBKEYDOWN( Buffer,DIK_ADD )         // 小键盘"+"键被按下
	KBKEYDOWN( Buffer,DIK_DIVIDE )      // 小键盘"/"键被按下
	KBKEYDOWN( Buffer,DIK_MULTIPLY )    // 小键盘"*"键被按下
	KBKEYDOWN( Buffer,DIK_DECIMAL )     // 小键盘"."键被按下
	KBKEYDOWN( Buffer,DIK_NUMPADENTER ) // 小键盘"Enter"键被按下

    //没有任何建被按下
	KeyKB[0].Key = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	return S_OK;
}

/***********************************************************
* 函数名 : GetKeyboardBufferData(...)
*   功能 : 以缓冲数据方式获取键盘输入状态
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::GetKeyboardBufferData( void )
{
    // 键盘设备无效则返回
    if( lpdiKeyboard == NULL ) 
        return S_OK;

    // 存放键盘输入状态的缓冲区
    DIDEVICEOBJECTDATA didod[ BUFFER_SIZE ];
    // 键盘数据缓冲区的元素个数
    DWORD              dwElements = BUFFER_SIZE;

    HRESULT            hr;

    // 获取键盘缓冲区数据，并刷新缓冲区的数据
    hr = lpdiKeyboard->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ),
                                     didod, &dwElements, 0 );
    if( FAILED( hr ) ) 
    {
        // 丢失了键盘设备，则重新获取
        hr = lpdiKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = lpdiKeyboard->Acquire();

        return hr; 
    }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // 保存上一次的按键状态
	KeyKB[1].Key = KeyKB[0].Key;

    // 获取键盘输入状态
    for( DWORD i = 0; i < dwElements; i++ ) 
    {
        KBBUFFERKEYDOWN( i )
    }

    // 没有任何键被按下
	KeyKB[0].Key = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return S_OK;
}

/***********************************************************
* 函数名: MouseInit(...)
*   功能: 初始化DirectInput鼠标的环境
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::MouseInit( HWND hWnd )
{
    HRESULT hr;
    DWORD   dwCoopFlags;

    // 先释放以前创建的鼠标设备
    FreeDirectInput( 2 );

    // 获取协作等级
    if( bExclusive )
        dwCoopFlags = DISCL_EXCLUSIVE;//独占模式
    else
        dwCoopFlags = DISCL_NONEXCLUSIVE;//非独占模式
    if( bForeGround )
        dwCoopFlags |= DISCL_FOREGROUND;//前台操作
    else
        dwCoopFlags |= DISCL_BACKGROUND;//后台操作

    // 第一步 : 创建DirectInput对象
	if( lpDI == NULL ) {
        HINSTANCE hInst = (HINSTANCE)GetWindowLong( hWnd, GWL_HINSTANCE );
        hr = DirectInput8Create( hInst,
                                 DIRECTINPUT_VERSION, 
                                 IID_IDirectInput8,
                                 ( VOID** )&lpDI, NULL );
        if( FAILED( hr ) ){
            MessageBox( hWnd, _T("创建DirectInput对象出错\n"),
                              _T("DirectInput8Create"), MB_OK );
        	return hr;
        }
    }

    // 第二步 : 创建系统鼠标设备的接口
    hr = lpDI->CreateDevice( GUID_SysMouse, &lpdiMouse, NULL );
    if( FAILED( hr ) ) {
        MessageBox( hWnd, _T("创建鼠标设备出错\n"),
                          _T("CreateDevice"), MB_OK );
        return hr;
    }

    // 第三步 : 设置鼠标数据格式
    hr = lpdiMouse->SetDataFormat( &c_dfDIMouse2 );
    if( FAILED( hr ) ) {
        MessageBox( hWnd, _T("设置鼠标数据格式失败\n"),
                          _T("SetDataFormat"), MB_OK );
        return hr;
    }

    // 第四步 : 设置DirectInput的协作等级
    hr = lpdiMouse->SetCooperativeLevel( hWnd, dwCoopFlags );
    if( hr == DIERR_UNSUPPORTED )
    {
        FreeDirectInput( 2 );
        MessageBox( hWnd, _T("系统鼠标设备不支持设置的协作等级\n"),
                          _T("SetCooperativeLevel"), MB_OK );
        return S_OK;
    }
    else if( FAILED(hr) ) {
        FreeDirectInput( 2 );
        MessageBox( hWnd, _T("设置系统鼠标设备的协作等级失败\n"),
                          _T("SetCooperativeLevel"), MB_OK );
        return hr;
    }

    // 第五步(可选) : 设置鼠标缓冲数据方式的缓冲区大小
    if( !bImmediate )
    {
        DIPROPDWORD dipdw;
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = BUFFER_SIZE;

        hr = lpdiMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
        if( FAILED( hr ) ){
            MessageBox( hWnd, _T("设置鼠标缓冲数据方式的缓冲区大小失败\n"),
                              _T("SetProperty"), MB_OK );
            return hr;
        }
    }

	// 第六步 : 获取鼠标设备
    lpdiMouse->Acquire();

    return S_OK;
}

/***********************************************************
* 函数名 : GetMouseImmediateData(...)
*   功能 : 以立即数据方式获取鼠标输入状态
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::GetMouseImmediateData( void )
{
    // 鼠标设备无效则返回
    if( lpdiMouse == NULL ) 
        return S_OK;

    // 存放鼠标数据的结构空间
    DIMOUSESTATE2 dims2;
    ZeroMemory( &dims2, sizeof(dims2) );

    HRESULT       hr;

    // 获取鼠标设备的输入状态
    hr = lpdiMouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), &dims2 );
    if( FAILED(hr) )  {
        // 丢失了鼠标设备，则重新获取
        hr = lpdiMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = lpdiMouse->Acquire();
        return S_OK; 
    }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // 保存上一次的按键和轴的状态
	KeyMouse[1].Key = KeyMouse[0].Key;
	KeyMouse[1].Pos.x = KeyMouse[0].Pos.x;
	KeyMouse[1].Pos.y = KeyMouse[0].Pos.y;
	KeyMouse[1].lZ = KeyMouse[0].lZ;

    // 获取鼠标各轴的状态
    if( bFullScr ) {
        KeyMouse[0].Pos.x = (LONG)(KeyMouse[0].Pos.x + MouseSpeed*dims2.lX);
        KeyMouse[0].Pos.y = (LONG)(KeyMouse[0].Pos.y + MouseSpeed*dims2.lY);
    }
    else
        GetCursorPos( &KeyMouse[0].Pos );
    KeyMouse[0].lZ    = (LONG)(KeyMouse[0].lZ + M_Z_Speed*dims2.lZ);

    // 鼠标横向位置边界限定
    if( KeyMouse[0].Pos.x > MouseMax_X )
        KeyMouse[0].Pos.x = MouseMax_X;
    else if( KeyMouse[0].Pos.x < MouseMin_X )
        KeyMouse[0].Pos.x = MouseMin_X;

    // 鼠标纵向位置边界限定
    if( KeyMouse[0].Pos.y > MouseMax_Y )
        KeyMouse[0].Pos.y = MouseMax_Y;
    else if( KeyMouse[0].Pos.y < MouseMin_Y )
        KeyMouse[0].Pos.y = MouseMin_Y;

    // 鼠标Z轴方向位置边界限定
    if( KeyMouse[0].lZ > MouseMax_Z )
        KeyMouse[0].lZ = MouseMin_Z;
    else if( KeyMouse[0].lZ < MouseMin_Z )
        KeyMouse[0].lZ = MouseMax_Z;

    // 鼠标按键状态
    MOUSEKEYDOWN( dims2.rgbButtons[0],DIMOFS_BUTTON0 )//按住左键
	MOUSEKEYDOWN( dims2.rgbButtons[1],DIMOFS_BUTTON1 )//按住右键
	MOUSEKEYDOWN( dims2.rgbButtons[2],DIMOFS_BUTTON2 )//按住中键

    // 没有按住任何鼠标键
    KeyMouse[0].Key = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return S_OK;
}

/***********************************************************
* 函数名 : GetMouseBufferData(...)
*   功能 : 以立即数据方式获取鼠标输入状态
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::GetMouseBufferData( void )
{
    // 鼠标设备无效则返回
    if( lpdiMouse == NULL ) 
        return S_OK;

    // 存放鼠标输入状态的缓冲区
    DIDEVICEOBJECTDATA didod[ BUFFER_SIZE ];
    // 键盘数据缓冲区的元素个数
    DWORD              dwElements = BUFFER_SIZE;

    HRESULT            hr;

    // 获取鼠标数据缓冲区的数据，并刷新缓冲区的数据
    hr = lpdiMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                     didod, &dwElements, 0 );
    if( FAILED( hr ) ) {
        // 丢失了鼠标设备，则重新获取
        hr = lpdiMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = lpdiMouse->Acquire();

        return S_OK; 
    }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // 保存上一次的按键和轴的状态
	KeyMouse[1].Key = KeyMouse[0].Key;
	KeyMouse[1].Pos.x = KeyMouse[0].Pos.x;
	KeyMouse[1].Pos.y = KeyMouse[0].Pos.y;
	KeyMouse[1].lZ = KeyMouse[0].lZ;

    // 获取按键状态
    for( DWORD i = 0; i < dwElements; i++ ) 
    {
        // 鼠标各轴状态
        if( bFullScr && didod[i].dwOfs ==  DIMOFS_X ) { // X 轴
            LONG Data = didod[i].dwData;
            KeyMouse[0].Pos.x = (LONG)(KeyMouse[0].Pos.x + MouseSpeed*Data);
        }
        if( bFullScr && didod[i].dwOfs ==  DIMOFS_Y ) { // Y 轴
            LONG Data = didod[i].dwData;
            KeyMouse[0].Pos.y = (LONG)(KeyMouse[0].Pos.y + MouseSpeed*Data);
        }
        if( didod[i].dwOfs ==  DIMOFS_Z ) { // Z 轴
            LONG Data = didod[i].dwData;
            KeyMouse[0].lZ = (LONG)(KeyMouse[0].lZ + M_Z_Speed*Data);
        }

        // 鼠标各按钮状态
        MOUSEBUFFERKEYDOWN( i, DIMOFS_BUTTON0 ) // 左键
        MOUSEBUFFERKEYDOWN( i, DIMOFS_BUTTON1 ) // 右键
        MOUSEBUFFERKEYDOWN( i, DIMOFS_BUTTON2 ) // 中键
    }
    if( bFullScr==0 )
        GetCursorPos( &KeyMouse[0].Pos );

    // 鼠标横向位置边界限定
    if( KeyMouse[0].Pos.x > MouseMax_X )
        KeyMouse[0].Pos.x = MouseMax_X;
    else if( KeyMouse[0].Pos.x < MouseMin_X )
        KeyMouse[0].Pos.x = MouseMin_X;

    // 鼠标纵向位置边界限定
    if( KeyMouse[0].Pos.y > MouseMax_Y )
        KeyMouse[0].Pos.y = MouseMax_Y;
    else if( KeyMouse[0].Pos.y < MouseMin_Y )
        KeyMouse[0].Pos.y = MouseMin_Y;

    // 鼠标Z轴方向位置边界限定
    if( KeyMouse[0].lZ > MouseMax_Z )
        KeyMouse[0].lZ = MouseMin_Z;
    else if( KeyMouse[0].lZ < MouseMin_Z )
        KeyMouse[0].lZ = MouseMax_Z;

    // 没有按住任何鼠标键
    KeyMouse[0].Key = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return S_OK;
}

/***********************************************************
* 函数名 : KeyboardEnumObjectCallBack(...)
*   功能 : 键盘列举函数的回调函数
*             (c)  jk&ljq, 2010
************************************************************/
BOOL CALLBACK KeyboardEnumObjectCallBack( 
                   LPCDIDEVICEOBJECTINSTANCE pdidoi,
                   LPVOID pContext )
{
    POINT * key = ( POINT* )pContext;

    if ( pdidoi->dwOfs == (DWORD)key->x )
    {
        key->y = TRUE;
        return DIENUM_STOP;
    }

    return DIENUM_CONTINUE;
}

/***********************************************************
* 函数名 : KeyboardEnumObject(...)
*   功能 : 检查键盘上由key->x指定的按键是否可用
*          该键可用则key->y的值为1,否则为0
*             (c)  jk&ljq, 2010
************************************************************/
void CDInput::KeyboardEnumObject( POINT *key )
{
    key->y = 0;

    lpdiKeyboard->EnumObjects( KeyboardEnumObjectCallBack,
                            key, DIDFT_ALL );

}

/***********************************************************
* 函数名 : MouseEnumObjectsCallBack(...)
*   功能 : 鼠标列举函数的回调函数
*             (c)  jk&ljq, 2010
************************************************************/
BOOL CALLBACK MouseEnumObjectsCallBack( 
                   LPCDIDEVICEOBJECTINSTANCE pdidoi,
                   LPVOID pContext )
{
    DIMOUSESTATE2* MouseObject = (DIMOUSESTATE2*)pContext;
    if ( pdidoi->guidType == GUID_XAxis )
        MouseObject->lX = TRUE;

    if ( pdidoi->guidType == GUID_YAxis )
        MouseObject->lY = TRUE;

    if ( pdidoi->guidType == GUID_ZAxis )
        MouseObject->lZ = TRUE;

    if ( pdidoi->guidType == GUID_Button )
        ++MouseObject->rgbButtons[0];

    return DIENUM_CONTINUE;
}

/***********************************************************
* 函数名 : MouseEnumObjects(...)
*   功能 : 列举鼠标上的可用的输入设备( 各个轴和按钮 )
*             (c)  jk&ljq, 2010
************************************************************/
DIMOUSESTATE2 CDInput::MouseEnumObjects( void )
{
    DIMOUSESTATE2 MouseObjects;
    ZeroMemory( &MouseObjects, sizeof( MouseObjects ) );

    lpdiMouse->EnumObjects( MouseEnumObjectsCallBack,
                            &MouseObjects, DIDFT_ALL );
    return MouseObjects;

}
