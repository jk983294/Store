/*------------------------------------------------------
|                  CDInput.h
|         DirectInput对象的相关处理功能
|            (c)  jk&ljq, 2010
--------------------------------------------------------*/

#include <dinput.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

typedef struct _KEY {
    LONG  Time;
    POINT Pos;
    LONG  lZ;
    DWORD Key;
} KEY, *PKEY;

class CDInput
{
protected:
    // DirectInput对象
    LPDIRECTINPUT8          lpDI;
    // 鼠标设备
    LPDIRECTINPUTDEVICE8    lpdiMouse;
    // 键盘设备
    LPDIRECTINPUTDEVICE8    lpdiKeyboard;
    // 是否在全屏模式下
    BOOL    bFullScr;
    // DirectInput是否是独占模式
    BOOL    bExclusive;
    // DirectInput是否是前台模式
    BOOL    bForeGround;
    // DirectInput是否是即时数据
    BOOL    bImmediate;
    // 键盘按键记录
    KEY     KeyKB[2];
    // 鼠标按键记录
    KEY     KeyMouse[2];

    // 鼠标移动范围成员
    LONG    MouseMin_X;
    LONG    MouseMax_X;
    LONG    MouseMin_Y;
    LONG    MouseMax_Y;
    LONG    MouseMin_Z;
    LONG    MouseMax_Z;

    // 鼠标移动速率成员
    double   MouseSpeed;
    double   M_Z_Speed;

public:
    // 构造函数
    CDInput( BOOL bEx,
             BOOL bFor,
             BOOL bImm,
             LONG Mouse_Min_X,
             LONG Mouse_Max_X,
             LONG Mouse_Min_Y, 
             LONG Mouse_Max_Y,
             LONG Mouse_Min_Z, 
             LONG Mouse_Max_Z );
    CDInput( void );

    // 析构函数
    ~CDInput();

    //>>>>>>> 设置DirectInput基本参数 >>>>>>>>>>>>>
    // 设置是否是全屏模式
    void Set_bFullScr( BOOL bFullScreen){ bFullScr = bFullScreen;}
    // 设置是否是独占模式
    void Set_bExclusive( BOOL bE )      { bExclusive = bE; }
    // 设置是否是前台模式
    void Set_bForeGround( BOOL bF )     { bForeGround = bF; }
    // 设置是否是立即数据格式
    void Set_bImmediate( BOOL bI )      { bImmediate = bI; }

    // 设置鼠标指针移动范围成员函数
    void Set_MouseMin_X( LONG MMin_X )  { MouseMin_X = MMin_X; }
    void Set_MouseMax_X( LONG MMax_X )  { MouseMax_X = MMax_X; }
    void Set_MouseMin_Y( LONG MMin_Y )  { MouseMin_Y = MMin_Y; }
    void Set_MouseMax_Y( LONG MMax_Y )  { MouseMax_Y = MMax_Y; }
    void Set_MouseMin_Z( LONG MMin_Z )  { MouseMin_Z = MMin_Z; }
    void Set_MouseMax_Z( LONG MMax_Z )  { MouseMax_Z = MMax_Z; }

    // 设置鼠标指针移动速率
    void Set_MouseSpeed( double MSpeed ) { MouseSpeed = MSpeed; }
    void Set_M_Z_Speed( double MZSpeed ) { M_Z_Speed = MZSpeed; }

    //>>>>>>> DirectInput对象 >>>>>>>>>>>>>
    // 初始化DirectInput环境
    void InitDInput( HWND hWnd );
    // 读取键盘和鼠标数据
    void GetDInputData( void );
    // 释放DirectInput
    void FreeDirectInput( UINT id );

    //>>>>>>> 键盘设备处理成员 >>>>>>>>>>>>>
    // 初始化键盘成员函数
    HRESULT KeyboardInit( HWND hWnd );
    // 读取键盘立即数据
    HRESULT GetKeyboardImmediateData( void );
    // 读取键盘缓冲数据
    HRESULT GetKeyboardBufferData( void );

    //>>>>>>> 鼠标设备处理成员 >>>>>>>>>>>>>
    // 初始化鼠标成员函数
    HRESULT MouseInit( HWND hWnd );
    // 读取鼠标立即数据
    HRESULT GetMouseImmediateData( void );
    // 读取鼠标缓冲数据
    HRESULT GetMouseBufferData( void );

    //>>>>>>> 键盘鼠标设备对象查询 >>>>>>>>>>>>>
    // 查询鼠标的可用的输入对象(按钮、轴)
    DIMOUSESTATE2 MouseEnumObjects( void );
    // 查询键盘的由key->x指定的按键是否可用
    void KeyboardEnumObject( POINT *key );

    //>>>>>>> 外部调用成员变量 >>>>>>>>>>>>>
    LPDIRECTINPUT8 Get_lpDI( void )               { return lpDI;}
    LPDIRECTINPUTDEVICE8 Get_lpdiMouse( void )    { return lpdiMouse;}
    LPDIRECTINPUTDEVICE8 Get_lpdiKeyboard( void ) { return lpdiKeyboard;}
    BOOL Get_bFullScr( void )                     { return bFullScr;}
    BOOL Get_bExclusive( void )                   { return bExclusive;}
    BOOL Get_bForeGround( void )                  { return bForeGround;}
    BOOL Get_bImmediate( void )                   { return bImmediate;}
    KEY * Get_KeyKB( void )                       { return KeyKB;}
    KEY * Get_KeyMouse( void )                    { return KeyMouse;}
    LONG Get_MouseMin_X( void )                   { return MouseMin_X;}
    LONG Get_MouseMax_X( void )                   { return MouseMax_X;}
    LONG Get_MouseMin_Y( void )                   { return MouseMin_Y;}
    LONG Get_MouseMax_Y( void )                   { return MouseMax_Y;}
    LONG Get_MouseMin_Z( void )                   { return MouseMin_Z;}
    LONG Get_MouseMax_Z( void )                   { return MouseMax_Z;}
    double Get_MouseSpeed( void )                 { return MouseSpeed;}
    double Get_M_Z_Speed( void )                  { return M_Z_Speed;}

};