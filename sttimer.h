/*++

Copyright (c) 2014 Microsoft Corporation

Module Name:

sttimer.h

Timer class

--*/

#ifndef _STTIMER_H
#define _STTIMER_H
#include "pch.h"

class STTIMER
{
public:

    STTIMER()
        : _pTimer(NULL)
    {
    }

    virtual
        ~STTIMER()
    {
        if (_pTimer)
        {
            CancelTimer();

            CloseThreadpoolTimer(_pTimer);

            _pTimer = NULL;
        }
    }

    HRESULT
        InitializeTimer(
            PTP_TIMER_CALLBACK   pfnCallback,
            VOID               * pContext,
            DWORD                dwInitialWait = 0,
            DWORD                dwPeriod = 0
        )
    {
        _pTimer = CreateThreadpoolTimer(pfnCallback,
            pContext,
            NULL);

        if (!_pTimer)
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        if (dwInitialWait)
        {
            SetTimer(dwInitialWait,
                dwPeriod);
        }

        return S_OK;
    }

    VOID
        SetTimer(
            DWORD dwInitialWait,
            DWORD dwPeriod = 0
        )
    {
        FILETIME ftInitialWait;

        if (dwInitialWait == 0 && dwPeriod == 0)
        {
            //
            // Special case.  We are preventing new callbacks
            // from being queued.  Any existing callbacks in the
            // queue will still run.
            //
            // This effectively disables the timer.  It can be
            // re-enabled by setting non-zero initial wait or
            // period values.
            //

            SetThreadpoolTimer(_pTimer, NULL, 0, 0);
            return;
        }

        InitializeRelativeFileTime(&ftInitialWait, dwInitialWait);

        SetThreadpoolTimer(_pTimer,
            &ftInitialWait,
            dwPeriod,
            0);
    }

    VOID
        CancelTimer()
    {
        //
        // Disable the timer
        //

        SetTimer(0);

        //
        // Wait until any callbacks queued prior to disabling
        // have completed.
        //

        WaitForThreadpoolTimerCallbacks(_pTimer, TRUE);
    }

private:

    VOID
        InitializeRelativeFileTime(
            FILETIME * pft,
            DWORD      dwMilliseconds
        )
    {
        LARGE_INTEGER li;

        //
        // The pftDueTime parameter expects the time to be
        // expressed as the number of 100 nanosecond intervals
        // times -1.
        //
        // To convert from milliseconds, we'll multiply by
        // -10000
        //

        li.QuadPart = (LONGLONG)dwMilliseconds * -10000;

        pft->dwHighDateTime = li.HighPart;
        pft->dwLowDateTime = li.LowPart;
    };

    TP_TIMER * _pTimer;
};

#endif // _STTIMER_H
