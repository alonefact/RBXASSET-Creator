#include "Clipboard.h"

namespace Clipboard {

    void Copy(const std::wstring& text) {
        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(wchar_t));
        if (!hGlob) {
            return;
        }

        wchar_t* pText = static_cast<wchar_t*>(GlobalLock(hGlob));
        if (!pText) {
            GlobalFree(hGlob);
            return;
        }

        wcscpy_s(pText, text.size() + 1, text.c_str());
        GlobalUnlock(hGlob);

        if (OpenClipboard(NULL)) {
            EmptyClipboard();
            if (SetClipboardData(CF_UNICODETEXT, hGlob) == NULL) {
                GlobalFree(hGlob);
            }
            CloseClipboard();
        }
        else {
            GlobalFree(hGlob);
        }
    }

}
