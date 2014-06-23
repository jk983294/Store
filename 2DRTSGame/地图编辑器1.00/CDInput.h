/*------------------------------------------------------
|                  CDInput.h
|         DirectInput�������ش�����
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
    // DirectInput����
    LPDIRECTINPUT8          lpDI;
    // ����豸
    LPDIRECTINPUTDEVICE8    lpdiMouse;
    // �����豸
    LPDIRECTINPUTDEVICE8    lpdiKeyboard;
    // �Ƿ���ȫ��ģʽ��
    BOOL    bFullScr;
    // DirectInput�Ƿ��Ƕ�ռģʽ
    BOOL    bExclusive;
    // DirectInput�Ƿ���ǰ̨ģʽ
    BOOL    bForeGround;
    // DirectInput�Ƿ��Ǽ�ʱ����
    BOOL    bImmediate;
    // ���̰�����¼
    KEY     KeyKB[2];
    // ��갴����¼
    KEY     KeyMouse[2];

    // ����ƶ���Χ��Ա
    LONG    MouseMin_X;
    LONG    MouseMax_X;
    LONG    MouseMin_Y;
    LONG    MouseMax_Y;
    LONG    MouseMin_Z;
    LONG    MouseMax_Z;

    // ����ƶ����ʳ�Ա
    double   MouseSpeed;
    double   M_Z_Speed;

public:
    // ���캯��
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

    // ��������
    ~CDInput();

    //>>>>>>> ����DirectInput�������� >>>>>>>>>>>>>
    // �����Ƿ���ȫ��ģʽ
    void Set_bFullScr( BOOL bFullScreen){ bFullScr = bFullScreen;}
    // �����Ƿ��Ƕ�ռģʽ
    void Set_bExclusive( BOOL bE )      { bExclusive = bE; }
    // �����Ƿ���ǰ̨ģʽ
    void Set_bForeGround( BOOL bF )     { bForeGround = bF; }
    // �����Ƿ����������ݸ�ʽ
    void Set_bImmediate( BOOL bI )      { bImmediate = bI; }

    // �������ָ���ƶ���Χ��Ա����
    void Set_MouseMin_X( LONG MMin_X )  { MouseMin_X = MMin_X; }
    void Set_MouseMax_X( LONG MMax_X )  { MouseMax_X = MMax_X; }
    void Set_MouseMin_Y( LONG MMin_Y )  { MouseMin_Y = MMin_Y; }
    void Set_MouseMax_Y( LONG MMax_Y )  { MouseMax_Y = MMax_Y; }
    void Set_MouseMin_Z( LONG MMin_Z )  { MouseMin_Z = MMin_Z; }
    void Set_MouseMax_Z( LONG MMax_Z )  { MouseMax_Z = MMax_Z; }

    // �������ָ���ƶ�����
    void Set_MouseSpeed( double MSpeed ) { MouseSpeed = MSpeed; }
    void Set_M_Z_Speed( double MZSpeed ) { M_Z_Speed = MZSpeed; }

    //>>>>>>> DirectInput���� >>>>>>>>>>>>>
    // ��ʼ��DirectInput����
    void InitDInput( HWND hWnd );
    // ��ȡ���̺��������
    void GetDInputData( void );
    // �ͷ�DirectInput
    void FreeDirectInput( UINT id );

    //>>>>>>> �����豸�����Ա >>>>>>>>>>>>>
    // ��ʼ�����̳�Ա����
    HRESULT KeyboardInit( HWND hWnd );
    // ��ȡ������������
    HRESULT GetKeyboardImmediateData( void );
    // ��ȡ���̻�������
    HRESULT GetKeyboardBufferData( void );

    //>>>>>>> ����豸�����Ա >>>>>>>>>>>>>
    // ��ʼ������Ա����
    HRESULT MouseInit( HWND hWnd );
    // ��ȡ�����������
    HRESULT GetMouseImmediateData( void );
    // ��ȡ��껺������
    HRESULT GetMouseBufferData( void );

    //>>>>>>> ��������豸�����ѯ >>>>>>>>>>>>>
    // ��ѯ���Ŀ��õ��������(��ť����)
    DIMOUSESTATE2 MouseEnumObjects( void );
    // ��ѯ���̵���key->xָ���İ����Ƿ����
    void KeyboardEnumObject( POINT *key );

    //>>>>>>> �ⲿ���ó�Ա���� >>>>>>>>>>>>>
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