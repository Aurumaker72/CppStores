// CppStores.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "framework.h"
#include "CppStoresDemo.h"
#include "CppStores.h"

Writable<int> something(0);
std::function<void()> unsubscribe;

HINSTANCE hInst;


INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		something.subscribe([hDlg](int value) {
			wchar_t text[200] = { 0 };
			wsprintf(text, L"%d", value);
			SetDlgItemText(hDlg, IDC_VALUE, text);
		});
		break;
	}
	case WM_CLOSE:
		DestroyWindow(hDlg);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_INCREMENT)
		{
			something.update([](int value) {
				return value + 1;
				});
		}
		if (LOWORD(wParam) == IDC_DECREMENT)
		{
			something.update([](int value) {
				return value - 1;
				});
		}
		break;

	}
	return (INT_PTR)FALSE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	hInst = hInstance; // Instanzenhandle in der globalen Variablen speichern

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, About);

	return 0;
}