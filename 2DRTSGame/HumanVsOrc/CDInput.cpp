
/*------------------------------------------------------
|                  CDInput.cpp
|          CDInput��ĳ�Ա�����������
|             (c)  jk&ljq, 2010
--------------------------------------------------------*/

#include<windows.h>
#include"CDInput.h"
#include<tchar.h>

//�������ݸ�ʽ�Ļ�������СΪ16
#define BUFFER_SIZE     16

// �������ݸ�ʽ���̰���
#define	KBKEYDOWN(name,key)  \
    if(name[key] & 0x80)\
    {\
        KeyKB[0].Key = key;\
        return S_OK;\
    }

// �������ݸ�ʽ���̰���
#define KBBUFFERKEYDOWN(x)  \
    if( didod[ x ].dwData & 0x80 )\
    {\
        KeyKB[0].Key  = didod[ x ].dwOfs;\
        KeyKB[0].Time = didod[ x ].dwTimeStamp;\
        return S_OK;\
    }

// �������ݸ�ʽ��갴��
#define	MOUSEKEYDOWN(name,key)  \
    if(name & 0x80)\
    {\
        KeyMouse[0].Key = key;\
        return S_OK;\
    }

// �������ݸ�ʽ��갴��
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
* ������ : CDInput(...)
*   ���� : ���캯������ʼ����Ա����
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

    // ��ʼ�����λ��
    GetCursorPos( &KeyMouse[0].Pos );
    GetCursorPos( &KeyMouse[1].Pos );
    KeyMouse[0].lZ = MouseMin_Z;
    KeyMouse[1].lZ = MouseMin_Z;

}

/***********************************************************
* ������ : CDInput(...)
*   ���� : ���캯������ʼ����Ա����������
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

    // ��ʼ�����λ��
    GetCursorPos( &KeyMouse[0].Pos );
    GetCursorPos( &KeyMouse[1].Pos );
    KeyMouse[0].lZ = MouseMin_Z;
    KeyMouse[1].lZ = MouseMin_Z;

}

/***********************************************************
* ������ : ~CDInput(...)
*   ���� : �����������ͷ�DirectInput����
*             (c)  jk&ljq, 2010
************************************************************/
CDInput::~CDInput()
{
    FreeDirectInput( 3 );
}

/***********************************************************
* ������ : InitDInput(...)
*   ���� : ����DirectInput����
*             (c)  jk&ljq, 2010
************************************************************/
void CDInput::InitDInput( HWND hWnd )
{
	//��ʼ�����̻���
	KeyboardInit( hWnd );

	//��ʼ����껷��
	MouseInit( hWnd );
}

/***********************************************************
* ������ : GetDInputData(...)
*   ���� : ��ȡ����״̬
*             (c)  jk&ljq, 2010
************************************************************/
void CDInput::GetDInputData( void )
{
    if( bImmediate ) {
        //��ȡ����״̬(��������)
        GetKeyboardImmediateData();

        //��ȡ���״̬(��������)
        GetMouseImmediateData();
    }
    else {
        //��ȡ����״̬(��������)
        GetKeyboardBufferData();

        //��ȡ���״̬(��������)
        GetMouseBufferData();
    }
}

/***********************************************************
* ������ : FreeDirectInput(...)
*   ���� : �ͷ����д��ڵ�DirectInput����
*   ����˵��:  id ��ֵ��Χ��1,2,3
*               1  ��ʾ�ͷ�
*             (c)  jk&ljq, 2010
************************************************************/
void CDInput::FreeDirectInput( UINT id )
{
	if(lpDI){

        // �ͷż����豸
		if( lpdiKeyboard ){
            if( id==1 || id==3 ){
                // Unacquire()����Release()֮ǰ����
                // ���ȶ��豸���ͷſռ�
                lpdiKeyboard->Unacquire();
                lpdiKeyboard->Release();
                lpdiKeyboard = NULL;
            }
		}

        // �ͷ�����豸
		if( lpdiMouse ){
            if( id==2 || id==3 ){
                // Unacquire()����Release()֮ǰ����
                // ���ȶ��豸���ͷſռ�
                lpdiMouse->Unacquire();
                lpdiMouse->Release();
                lpdiMouse = NULL;
            }
		}

        // �ͷ�DirectInput����
        lpDI->Release();
        lpDI = NULL;
	}
}


/***********************************************************
* ������ : KeyboardInit(...)
*   ���� : ��ʼ��DirectInput�����豸
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::KeyboardInit( HWND hWnd )
{
    HRESULT hr;
    DWORD   dwCoopFlags;

    // ���ͷ���ǰ�����ļ����豸
    FreeDirectInput( 1 );

    // ��ȡЭ���ȼ�
    if( bExclusive )
        dwCoopFlags = DISCL_EXCLUSIVE;//��ռģʽ
    else
        dwCoopFlags = DISCL_NONEXCLUSIVE;//�Ƕ�ռģʽ
    if( bForeGround )
        dwCoopFlags |= DISCL_FOREGROUND;//ǰ̨����
    else
        dwCoopFlags |= DISCL_BACKGROUND;//��̨����

    // ��һ�� : ����DirectInput����
	if(lpDI==NULL){
        HINSTANCE hInst = (HINSTANCE)GetWindowLong( hWnd, GWL_HINSTANCE );
        hr = DirectInput8Create( hInst,
                                 DIRECTINPUT_VERSION, 
                                 IID_IDirectInput8,
                                 ( VOID** )&lpDI, NULL );
        if( FAILED( hr ) ){
            MessageBox( hWnd, _T("����DirectInput�������\n"),
                              _T("DirectInput8Create"), MB_OK );
        	return hr;
        }
    }

    // �ڶ��� : ����ϵͳ�����豸
    hr = lpDI->CreateDevice( GUID_SysKeyboard, &lpdiKeyboard, NULL );
    if( FAILED( hr ) ) {
        MessageBox( hWnd, _T("����ϵͳ�����豸����\n"),
                          _T("CreateDevice"), MB_OK );
        return hr;
    }

    // ������ : ���ü������ݸ�ʽ 
    hr = lpdiKeyboard->SetDataFormat( &c_dfDIKeyboard );
    if( FAILED( hr ) ) {
        MessageBox( hWnd, _T("���ü������ݸ�ʽʧ��\n"),
                          _T("SetDataFormat"), MB_OK );
        return hr;
    }

    // ���Ĳ� : ����DirectInput��Э���ȼ�
    hr = lpdiKeyboard->SetCooperativeLevel( hWnd, dwCoopFlags );
    if( hr == DIERR_UNSUPPORTED )
    {
        FreeDirectInput( 1 );
        MessageBox( hWnd, _T("ϵͳ�����豸��֧�����õ�Э���ȼ�\n"),
                          _T("SetCooperativeLevel"), MB_OK );
        return S_OK;
    }
    else if( FAILED(hr) ) {
        FreeDirectInput( 1 );
        MessageBox( hWnd, _T("����ϵͳ�����豸��Э���ȼ�ʧ��\n"),
                          _T("SetCooperativeLevel"), MB_OK );
        return hr;
    }

    // ���岽(��ѡ) : ���ü��̻������ݷ�ʽ�Ļ�������С
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
            MessageBox( hWnd, _T("���ü��̻������ݷ�ʽ�Ļ�������Сʧ��\n"),
                              _T("SetProperty"), MB_OK );
            return hr;
        }
    }

    // ������ : ��ȡ�����豸
    lpdiKeyboard->Acquire();

    return S_OK;
}

/***********************************************************
* ������ : GetKeyboardImmediateData(...)
*   ���� : ��ȡ���̼�ʱ��������
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::GetKeyboardImmediateData( void )
{
    // �����豸��Ч�򷵻�
    if( lpdiKeyboard==NULL ) 
        return S_OK;

    HRESULT hr;

    // ��ż�������״̬�Ļ�����
    BYTE    Buffer[256];
    ZeroMemory( Buffer, sizeof(Buffer) );

    // ��ȡ�����豸������״̬
    hr = lpdiKeyboard->GetDeviceState( sizeof(Buffer), Buffer );
    if( FAILED( hr ) )  {
        // ��ʧ�˼����豸�������»�ȡ
        hr = lpdiKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = lpdiKeyboard->Acquire();
        return hr;
    }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // ������һ�εİ���״̬
	KeyKB[1].Key = KeyKB[0].Key;

    // ���հ���״̬
	KBKEYDOWN( Buffer,DIK_ESCAPE )      // "Esc"��������
	KBKEYDOWN( Buffer,DIK_GRAVE )       // "`"��������
	KBKEYDOWN( Buffer,DIK_F1 )          // F1��������
	KBKEYDOWN( Buffer,DIK_F2 )          // F2��������
	KBKEYDOWN( Buffer,DIK_F3 )          // F3��������
	KBKEYDOWN( Buffer,DIK_F4 )          // F4��������
	KBKEYDOWN( Buffer,DIK_F5 )          // F5��������
	KBKEYDOWN( Buffer,DIK_F6 )          // F6��������
	KBKEYDOWN( Buffer,DIK_F7 )          // F7��������
	KBKEYDOWN( Buffer,DIK_F8 )          // F8��������
	KBKEYDOWN( Buffer,DIK_F9 )          // F9��������
	KBKEYDOWN( Buffer,DIK_F10 )         // F10��������
	KBKEYDOWN( Buffer,DIK_F11 )         // F11��������
	KBKEYDOWN( Buffer,DIK_F12 )         // F12��������
	KBKEYDOWN( Buffer,DIK_F13 )         // F13��������
	KBKEYDOWN( Buffer,DIK_F14 )         // F14��������
	KBKEYDOWN( Buffer,DIK_F15 )         // F15��������
	KBKEYDOWN( Buffer,DIK_1 )           // "1"��������
	KBKEYDOWN( Buffer,DIK_1 )           // "1"��������
	KBKEYDOWN( Buffer,DIK_2 )           // "2"��������
	KBKEYDOWN( Buffer,DIK_3 )           // "3"��������
	KBKEYDOWN( Buffer,DIK_4 )           // "4"��������
	KBKEYDOWN( Buffer,DIK_5 )           // "5"��������
	KBKEYDOWN( Buffer,DIK_6 )           // "6"��������
	KBKEYDOWN( Buffer,DIK_7 )           // "7"��������
	KBKEYDOWN( Buffer,DIK_8 )           // "8"��������
	KBKEYDOWN( Buffer,DIK_9 )           // "9"��������
	KBKEYDOWN( Buffer,DIK_0 )           // "0"��������
	KBKEYDOWN( Buffer,DIK_A )           // "A"��������
	KBKEYDOWN( Buffer,DIK_B )           // "B"��������
	KBKEYDOWN( Buffer,DIK_C )           // "C"��������
	KBKEYDOWN( Buffer,DIK_D )           // "D"��������
	KBKEYDOWN( Buffer,DIK_E )           // "E"��������
	KBKEYDOWN( Buffer,DIK_F )           // "F"��������
	KBKEYDOWN( Buffer,DIK_G )           // "G"��������
	KBKEYDOWN( Buffer,DIK_H )           // "H"��������
	KBKEYDOWN( Buffer,DIK_I )           // "I"��������
	KBKEYDOWN( Buffer,DIK_J )           // "J"��������
	KBKEYDOWN( Buffer,DIK_K )           // "K"��������
	KBKEYDOWN( Buffer,DIK_L )           // "L"��������
	KBKEYDOWN( Buffer,DIK_M )           // "M"��������
	KBKEYDOWN( Buffer,DIK_N )           // "N"��������
	KBKEYDOWN( Buffer,DIK_O )           // "O"��������
	KBKEYDOWN( Buffer,DIK_P )           // "P"��������
	KBKEYDOWN( Buffer,DIK_Q )           // "Q"��������
	KBKEYDOWN( Buffer,DIK_R )           // "R"��������
	KBKEYDOWN( Buffer,DIK_S )           // "S"��������
	KBKEYDOWN( Buffer,DIK_T )           // "T"��������
	KBKEYDOWN( Buffer,DIK_U )           // "U"��������
	KBKEYDOWN( Buffer,DIK_V )           // "V"��������
	KBKEYDOWN( Buffer,DIK_W )           // "W"��������
	KBKEYDOWN( Buffer,DIK_X )           // "X"��������
	KBKEYDOWN( Buffer,DIK_Y )           // "Y"��������
	KBKEYDOWN( Buffer,DIK_Z )           // "Z"��������
	KBKEYDOWN( Buffer,DIK_LBRACKET )    // "["��������
	KBKEYDOWN( Buffer,DIK_RBRACKET )    // "]"��������
	KBKEYDOWN( Buffer,DIK_LSHIFT )      // ��"Shift"��������
	KBKEYDOWN( Buffer,DIK_RSHIFT )      // ��"Shift"��������
	KBKEYDOWN( Buffer,DIK_LCONTROL )    // ��"Ctrl"��������
	KBKEYDOWN( Buffer,DIK_RCONTROL )    // ��"Ctrl"��������
	KBKEYDOWN( Buffer,DIK_LMENU )       // ��"Alt"��������
	KBKEYDOWN( Buffer,DIK_RMENU )       // ��"Alt"��������
	KBKEYDOWN( Buffer,DIK_MINUS )       // "_"��������
	KBKEYDOWN( Buffer,DIK_EQUALS )      // "="��������
	KBKEYDOWN( Buffer,DIK_COMMA )       // ","��������
	KBKEYDOWN( Buffer,DIK_PERIOD )      // "."��������
	KBKEYDOWN( Buffer,DIK_SLASH )       // "/"��������
	KBKEYDOWN( Buffer,DIK_RETURN )      // �س���������
	KBKEYDOWN( Buffer,DIK_SPACE )       // �ո��������
	KBKEYDOWN( Buffer,DIK_TAB )         // "Tab"��������
	KBKEYDOWN( Buffer,DIK_BACK )        // "Back Space"��������

	KBKEYDOWN( Buffer,DIK_WAKE )        // "Wake Up"��������
	KBKEYDOWN( Buffer,DIK_POWER )       // "Power"��������
	KBKEYDOWN( Buffer,DIK_SLEEP )       // "Sleep"��������
	KBKEYDOWN( Buffer,DIK_SCROLL )      // "Scroll Lock"��������
	KBKEYDOWN( Buffer,DIK_PAUSE )       // "Pause Break"��������
	KBKEYDOWN( Buffer,DIK_INSERT )      // "Insert"��������
	KBKEYDOWN( Buffer,DIK_DELETE )      // "Delete"��������
	KBKEYDOWN( Buffer,DIK_HOME )        // "Home"��������
	KBKEYDOWN( Buffer,DIK_END )         // "End"��������
	KBKEYDOWN( Buffer,DIK_PGUP )       // "Page Up"��������
	KBKEYDOWN( Buffer,DIK_PGDN )        // "Page Down"��������
	KBKEYDOWN( Buffer,DIK_UP )          // "��"��������
	KBKEYDOWN( Buffer,DIK_DOWN )        // "��"��������
	KBKEYDOWN( Buffer,DIK_LEFT )        // "��"��������
	KBKEYDOWN( Buffer,DIK_RIGHT )       // "��"��������

	// С���̰���״̬
	KBKEYDOWN( Buffer,DIK_NUMLOCK )     // "NumLock"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD0 )     // С����"0"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD1 )     // С����"1"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD2 )     // С����"2"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD3 )     // С����"3"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD4 )     // С����"4"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD5 )     // С����"5"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD6 )     // С����"6"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD7 )     // С����"7"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD8 )     // С����"8"��������
	KBKEYDOWN( Buffer,DIK_NUMPAD9 )     // С����"9"��������
	KBKEYDOWN( Buffer,DIK_SUBTRACT )    // С����"-"��������
	KBKEYDOWN( Buffer,DIK_ADD )         // С����"+"��������
	KBKEYDOWN( Buffer,DIK_DIVIDE )      // С����"/"��������
	KBKEYDOWN( Buffer,DIK_MULTIPLY )    // С����"*"��������
	KBKEYDOWN( Buffer,DIK_DECIMAL )     // С����"."��������
	KBKEYDOWN( Buffer,DIK_NUMPADENTER ) // С����"Enter"��������

    //û���κν�������
	KeyKB[0].Key = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	return S_OK;
}

/***********************************************************
* ������ : GetKeyboardBufferData(...)
*   ���� : �Ի������ݷ�ʽ��ȡ��������״̬
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::GetKeyboardBufferData( void )
{
    // �����豸��Ч�򷵻�
    if( lpdiKeyboard == NULL ) 
        return S_OK;

    // ��ż�������״̬�Ļ�����
    DIDEVICEOBJECTDATA didod[ BUFFER_SIZE ];
    // �������ݻ�������Ԫ�ظ���
    DWORD              dwElements = BUFFER_SIZE;

    HRESULT            hr;

    // ��ȡ���̻��������ݣ���ˢ�»�����������
    hr = lpdiKeyboard->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ),
                                     didod, &dwElements, 0 );
    if( FAILED( hr ) ) 
    {
        // ��ʧ�˼����豸�������»�ȡ
        hr = lpdiKeyboard->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = lpdiKeyboard->Acquire();

        return hr; 
    }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // ������һ�εİ���״̬
	KeyKB[1].Key = KeyKB[0].Key;

    // ��ȡ��������״̬
    for( DWORD i = 0; i < dwElements; i++ ) 
    {
        KBBUFFERKEYDOWN( i )
    }

    // û���κμ�������
	KeyKB[0].Key = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return S_OK;
}

/***********************************************************
* ������: MouseInit(...)
*   ����: ��ʼ��DirectInput���Ļ���
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::MouseInit( HWND hWnd )
{
    HRESULT hr;
    DWORD   dwCoopFlags;

    // ���ͷ���ǰ����������豸
    FreeDirectInput( 2 );

    // ��ȡЭ���ȼ�
    if( bExclusive )
        dwCoopFlags = DISCL_EXCLUSIVE;//��ռģʽ
    else
        dwCoopFlags = DISCL_NONEXCLUSIVE;//�Ƕ�ռģʽ
    if( bForeGround )
        dwCoopFlags |= DISCL_FOREGROUND;//ǰ̨����
    else
        dwCoopFlags |= DISCL_BACKGROUND;//��̨����

    // ��һ�� : ����DirectInput����
	if( lpDI == NULL ) {
        HINSTANCE hInst = (HINSTANCE)GetWindowLong( hWnd, GWL_HINSTANCE );
        hr = DirectInput8Create( hInst,
                                 DIRECTINPUT_VERSION, 
                                 IID_IDirectInput8,
                                 ( VOID** )&lpDI, NULL );
        if( FAILED( hr ) ){
            MessageBox( hWnd, _T("����DirectInput�������\n"),
                              _T("DirectInput8Create"), MB_OK );
        	return hr;
        }
    }

    // �ڶ��� : ����ϵͳ����豸�Ľӿ�
    hr = lpDI->CreateDevice( GUID_SysMouse, &lpdiMouse, NULL );
    if( FAILED( hr ) ) {
        MessageBox( hWnd, _T("��������豸����\n"),
                          _T("CreateDevice"), MB_OK );
        return hr;
    }

    // ������ : ����������ݸ�ʽ
    hr = lpdiMouse->SetDataFormat( &c_dfDIMouse2 );
    if( FAILED( hr ) ) {
        MessageBox( hWnd, _T("����������ݸ�ʽʧ��\n"),
                          _T("SetDataFormat"), MB_OK );
        return hr;
    }

    // ���Ĳ� : ����DirectInput��Э���ȼ�
    hr = lpdiMouse->SetCooperativeLevel( hWnd, dwCoopFlags );
    if( hr == DIERR_UNSUPPORTED )
    {
        FreeDirectInput( 2 );
        MessageBox( hWnd, _T("ϵͳ����豸��֧�����õ�Э���ȼ�\n"),
                          _T("SetCooperativeLevel"), MB_OK );
        return S_OK;
    }
    else if( FAILED(hr) ) {
        FreeDirectInput( 2 );
        MessageBox( hWnd, _T("����ϵͳ����豸��Э���ȼ�ʧ��\n"),
                          _T("SetCooperativeLevel"), MB_OK );
        return hr;
    }

    // ���岽(��ѡ) : ������껺�����ݷ�ʽ�Ļ�������С
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
            MessageBox( hWnd, _T("������껺�����ݷ�ʽ�Ļ�������Сʧ��\n"),
                              _T("SetProperty"), MB_OK );
            return hr;
        }
    }

	// ������ : ��ȡ����豸
    lpdiMouse->Acquire();

    return S_OK;
}

/***********************************************************
* ������ : GetMouseImmediateData(...)
*   ���� : ���������ݷ�ʽ��ȡ�������״̬
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::GetMouseImmediateData( void )
{
    // ����豸��Ч�򷵻�
    if( lpdiMouse == NULL ) 
        return S_OK;

    // ���������ݵĽṹ�ռ�
    DIMOUSESTATE2 dims2;
    ZeroMemory( &dims2, sizeof(dims2) );

    HRESULT       hr;

    // ��ȡ����豸������״̬
    hr = lpdiMouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), &dims2 );
    if( FAILED(hr) )  {
        // ��ʧ������豸�������»�ȡ
        hr = lpdiMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = lpdiMouse->Acquire();
        return S_OK; 
    }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // ������һ�εİ��������״̬
	KeyMouse[1].Key = KeyMouse[0].Key;
	KeyMouse[1].Pos.x = KeyMouse[0].Pos.x;
	KeyMouse[1].Pos.y = KeyMouse[0].Pos.y;
	KeyMouse[1].lZ = KeyMouse[0].lZ;

    // ��ȡ�������״̬
    if( bFullScr ) {
        KeyMouse[0].Pos.x = (LONG)(KeyMouse[0].Pos.x + MouseSpeed*dims2.lX);
        KeyMouse[0].Pos.y = (LONG)(KeyMouse[0].Pos.y + MouseSpeed*dims2.lY);
    }
    else
        GetCursorPos( &KeyMouse[0].Pos );
    KeyMouse[0].lZ    = (LONG)(KeyMouse[0].lZ + M_Z_Speed*dims2.lZ);

    // ������λ�ñ߽��޶�
    if( KeyMouse[0].Pos.x > MouseMax_X )
        KeyMouse[0].Pos.x = MouseMax_X;
    else if( KeyMouse[0].Pos.x < MouseMin_X )
        KeyMouse[0].Pos.x = MouseMin_X;

    // �������λ�ñ߽��޶�
    if( KeyMouse[0].Pos.y > MouseMax_Y )
        KeyMouse[0].Pos.y = MouseMax_Y;
    else if( KeyMouse[0].Pos.y < MouseMin_Y )
        KeyMouse[0].Pos.y = MouseMin_Y;

    // ���Z�᷽��λ�ñ߽��޶�
    if( KeyMouse[0].lZ > MouseMax_Z )
        KeyMouse[0].lZ = MouseMin_Z;
    else if( KeyMouse[0].lZ < MouseMin_Z )
        KeyMouse[0].lZ = MouseMax_Z;

    // ��갴��״̬
    MOUSEKEYDOWN( dims2.rgbButtons[0],DIMOFS_BUTTON0 )//��ס���
	MOUSEKEYDOWN( dims2.rgbButtons[1],DIMOFS_BUTTON1 )//��ס�Ҽ�
	MOUSEKEYDOWN( dims2.rgbButtons[2],DIMOFS_BUTTON2 )//��ס�м�

    // û�а�ס�κ�����
    KeyMouse[0].Key = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return S_OK;
}

/***********************************************************
* ������ : GetMouseBufferData(...)
*   ���� : ���������ݷ�ʽ��ȡ�������״̬
*             (c)  jk&ljq, 2010
************************************************************/
HRESULT CDInput::GetMouseBufferData( void )
{
    // ����豸��Ч�򷵻�
    if( lpdiMouse == NULL ) 
        return S_OK;

    // ����������״̬�Ļ�����
    DIDEVICEOBJECTDATA didod[ BUFFER_SIZE ];
    // �������ݻ�������Ԫ�ظ���
    DWORD              dwElements = BUFFER_SIZE;

    HRESULT            hr;

    // ��ȡ������ݻ����������ݣ���ˢ�»�����������
    hr = lpdiMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                     didod, &dwElements, 0 );
    if( FAILED( hr ) ) {
        // ��ʧ������豸�������»�ȡ
        hr = lpdiMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = lpdiMouse->Acquire();

        return S_OK; 
    }

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // ������һ�εİ��������״̬
	KeyMouse[1].Key = KeyMouse[0].Key;
	KeyMouse[1].Pos.x = KeyMouse[0].Pos.x;
	KeyMouse[1].Pos.y = KeyMouse[0].Pos.y;
	KeyMouse[1].lZ = KeyMouse[0].lZ;

    // ��ȡ����״̬
    for( DWORD i = 0; i < dwElements; i++ ) 
    {
        // ������״̬
        if( bFullScr && didod[i].dwOfs ==  DIMOFS_X ) { // X ��
            LONG Data = didod[i].dwData;
            KeyMouse[0].Pos.x = (LONG)(KeyMouse[0].Pos.x + MouseSpeed*Data);
        }
        if( bFullScr && didod[i].dwOfs ==  DIMOFS_Y ) { // Y ��
            LONG Data = didod[i].dwData;
            KeyMouse[0].Pos.y = (LONG)(KeyMouse[0].Pos.y + MouseSpeed*Data);
        }
        if( didod[i].dwOfs ==  DIMOFS_Z ) { // Z ��
            LONG Data = didod[i].dwData;
            KeyMouse[0].lZ = (LONG)(KeyMouse[0].lZ + M_Z_Speed*Data);
        }

        // ������ť״̬
        MOUSEBUFFERKEYDOWN( i, DIMOFS_BUTTON0 ) // ���
        MOUSEBUFFERKEYDOWN( i, DIMOFS_BUTTON1 ) // �Ҽ�
        MOUSEBUFFERKEYDOWN( i, DIMOFS_BUTTON2 ) // �м�
    }
    if( bFullScr==0 )
        GetCursorPos( &KeyMouse[0].Pos );

    // ������λ�ñ߽��޶�
    if( KeyMouse[0].Pos.x > MouseMax_X )
        KeyMouse[0].Pos.x = MouseMax_X;
    else if( KeyMouse[0].Pos.x < MouseMin_X )
        KeyMouse[0].Pos.x = MouseMin_X;

    // �������λ�ñ߽��޶�
    if( KeyMouse[0].Pos.y > MouseMax_Y )
        KeyMouse[0].Pos.y = MouseMax_Y;
    else if( KeyMouse[0].Pos.y < MouseMin_Y )
        KeyMouse[0].Pos.y = MouseMin_Y;

    // ���Z�᷽��λ�ñ߽��޶�
    if( KeyMouse[0].lZ > MouseMax_Z )
        KeyMouse[0].lZ = MouseMin_Z;
    else if( KeyMouse[0].lZ < MouseMin_Z )
        KeyMouse[0].lZ = MouseMax_Z;

    // û�а�ס�κ�����
    KeyMouse[0].Key = NULL;
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    return S_OK;
}

/***********************************************************
* ������ : KeyboardEnumObjectCallBack(...)
*   ���� : �����оٺ����Ļص�����
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
* ������ : KeyboardEnumObject(...)
*   ���� : ����������key->xָ���İ����Ƿ����
*          �ü�������key->y��ֵΪ1,����Ϊ0
*             (c)  jk&ljq, 2010
************************************************************/
void CDInput::KeyboardEnumObject( POINT *key )
{
    key->y = 0;

    lpdiKeyboard->EnumObjects( KeyboardEnumObjectCallBack,
                            key, DIDFT_ALL );

}

/***********************************************************
* ������ : MouseEnumObjectsCallBack(...)
*   ���� : ����оٺ����Ļص�����
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
* ������ : MouseEnumObjects(...)
*   ���� : �о�����ϵĿ��õ������豸( ������Ͱ�ť )
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
