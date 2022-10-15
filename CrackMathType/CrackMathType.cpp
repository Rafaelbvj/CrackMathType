#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include "framework.h"
#include "CrackMathType.h"
#include <commdlg.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
#define MAX_LOADSTRING 100

// Variáveis Globais:
HINSTANCE hInst;                                 
WCHAR szTitle[MAX_LOADSTRING];                   
WCHAR szWindowClass[MAX_LOADSTRING];           
HBITMAP bmp_lg;
 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CRACKMATHTYPE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    bmp_lg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LDYGAGA));
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CRACKMATHTYPE));

    MSG msg;

    // Loop de mensagem principal:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

 
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(0x00FFFAEA);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CRACKMATHTYPE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    return RegisterClassExW(&wcex);
}
 
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;  

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU,
      CW_USEDEFAULT, CW_USEDEFAULT, 400, 150, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

 
HWND edit,botao_1,botao_2, estatico;
HBRUSH bkg;
HDC hdc, mainhdc , bmp_hdc;
OPENFILENAME pf;
LPWSTR arquivo;
WCHAR buffer[MAX_PATH];
size_t tam_arq = MAX_PATH * 2;
BYTE code_look[] = { 0xFC, 0x00,0x50,0x8B,0xCF,0xE8,0x2A,0xFB,0xFF,0xFF };
BYTE code_insert[] = { 0xc6,0x45,0xc9,0x30,0x90,0x90,0x90,0x90 };           //codigo mov [ebp-37],0x30
COLORREF colors[] = { RGB(0xFF,0,0), RGB(0,0xFF,0), RGB(0,0,0xFF), RGB(0x01,0xFF,0x1F), RGB (0xE3,0xff,0x00) , RGB(0xff,0x9a,0x00) };
int counter = 0;

void init_ofn(OPENFILENAME* ofn, HWND owner,LPWSTR init_dir,  LPWSTR path, size_t path_size) {
    ZeroMemory(ofn, sizeof(OPENFILENAME));
    ofn->lStructSize = sizeof(OPENFILENAME);
    ofn->hwndOwner = owner;
    ofn->Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
    ofn->lpstrFilter = L"Exe files\0*.exe\0\0";
    ofn->lpstrFile = path;
    ofn->nMaxFile = path_size;
    ofn->lpstrInitialDir = init_dir;
    

}
bool crack(HWND hwnd,LPWSTR filepath) {
    HANDLE file = CreateFile(filepath, GENERIC_READ, 0,0,OPEN_EXISTING,0,0 );

    if (file == INVALID_HANDLE_VALUE) {
        MessageBox(hwnd, L"Arquivo não pode ser aberto. Verifique se ele está em uso por outro programa.", L"Erro", MB_OK | MB_ICONERROR);
        return false;
    }
    DWORD size_file,read;
    size_file = GetFileSize(file, NULL);
    BYTE* data = new BYTE[size_file];
    
    if (ReadFile(file, data, size_file, &read, 0) && read == size_file) {
        bool reto = false;
        size_t i = 0 , e=0; 
        for (e=0, i =0;i<size_file && e < sizeof(code_look) / sizeof(BYTE);i++) {
            if (data[i] == code_look[e]) {    
                e++;
                reto = true;
            }
            else {
                if (reto) {
                    e = 0;
                    reto = false;
                }
            }

        }
        if (e == sizeof(code_look) / sizeof(BYTE)) {
            memcpy(&data[i], code_insert, sizeof(code_insert));
            CloseHandle(file);
            file = CreateFile(arquivo, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
            if (file == INVALID_HANDLE_VALUE) {
                MessageBox(hwnd, L"Arquivo nao pode ser escrito. Execute como administrador!", L"Erro", MB_OK | MB_ICONERROR);
                return false;
            }
            DWORD written;
            if (WriteFile(file, data, size_file, &written, 0) && written == size_file) {
                CloseHandle(file);
                return true;
            }

        }

    }
    MessageBox(hwnd, L"Executavel nao reconhecido. Certifique-se que o arquivo MathType.exe foi selecionado.", L"ERRO", MB_OK | MB_ICONERROR);
    CloseHandle(file);
    return false;
    
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 10004, 1000, NULL);
        PlaySound(MAKEINTRESOURCE(IDR_SONG), hInst, SND_LOOP | SND_RESOURCE | SND_ASYNC);

        estatico = CreateWindowEx(0, L"static", L"Local do executavel (MathType.exe):", WS_VISIBLE | WS_CHILD, 20, 10, 300, 20, hWnd, (HMENU)10001, 0, 0);
        botao_1 = CreateWindowEx(0, L"button", L"Abrir...", WS_VISIBLE | WS_CHILD, 250, 40, 100, 20, hWnd, (HMENU)10001, 0, 0);
        botao_2 = CreateWindowEx(0, L"button", L"Crackear!", WS_VISIBLE|WS_CHILD|WS_DISABLED, 20, 70, 100, 20, hWnd, (HMENU)10002, 0, 0);
        edit = CreateWindowEx(0, L"edit", L"", WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL, 20, 40, 200, 20, hWnd, 0, 0, 0);
        
        arquivo = (LPWSTR)calloc(tam_arq, sizeof(WCHAR));
        GetEnvironmentVariable(L"programfiles(x86)", buffer, MAX_PATH);
        lstrcatW(buffer, L"\\MathType");
        init_ofn(&pf, hWnd, buffer, arquivo, tam_arq);
        break;
    case WM_TIMER:
    {   
        InvalidateRect(hWnd, 0, false);
        SendMessageW(hWnd, WM_CTLCOLORSTATIC, (WPARAM)GetDC(estatico), (LPARAM)hWnd);
    }
    return 0;
    case WM_CTLCOLORSTATIC:
    {
        hdc = (HDC)wParam;
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, colors[counter]);
        
        if(counter < 6){
            counter++;
        }
        else {
            counter = 0;
        }

        return (INT_PTR)(HBRUSH)GetStockObject(NULL_BRUSH);
    }
    case WM_COMMAND:
        ZeroMemory(arquivo, tam_arq);
        if (wParam == 10002) {
            SendMessage(edit, WM_GETTEXT, tam_arq, (LPARAM)arquivo);
            if (crack(hWnd,arquivo)) {
                MessageBox(hWnd, L"Crackeado!", L"INFO", MB_OK | MB_ICONINFORMATION);
            }
            else {
                EnableWindow(botao_2, false);
            }
        }
        if (wParam == 10001) {
            if (GetOpenFileName(&pf)) {
                SendMessage(edit, WM_SETTEXT, 0, (LPARAM)arquivo);
                EnableWindow(botao_2, true);
            }
            
        }
        break;
    case WM_PAINT: 
    {
        PAINTSTRUCT pts;
        RECT r;
        BITMAP bp;
        mainhdc = BeginPaint(hWnd, &pts);
        
        bmp_hdc = CreateCompatibleDC(mainhdc);
        GetObject(bmp_lg, sizeof(bp), &bp);
        SelectObject(bmp_hdc, bmp_lg);

        GetClientRect(hWnd, &r);
        StretchBlt(mainhdc, 0, 0, r.right - r.left, r.bottom - r.top, bmp_hdc, 0, 0, bp.bmWidth, bp.bmHeight, SRCCOPY);
        DeleteDC(mainhdc);
        EndPaint(hWnd, &pts);
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

 