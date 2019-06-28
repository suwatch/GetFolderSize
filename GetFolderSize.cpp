// GetFolderSize.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "sttimer.h"

// #define HEALTH_CHECK_TIMER_INTERVAL_SECONDS 5 * 60 // 5 mins
#define HEALTH_CHECK_TIMER_INTERVAL_SECONDS 5

PTP_TIMER sm_pHealthTimer;
FILETIME sm_ftTimeoutInterval;

//static
VOID
HealthTimerCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Context,
    PTP_TIMER             Timer
)
{
    std::cout << "Begin HealthTimerCallback\n";

    std::cout << "SetThreadpoolTimer\n";
    SetThreadpoolTimer(sm_pHealthTimer,
        &sm_ftTimeoutInterval,
        0, // dueTime
        0);

    std::cout << "End HealthTimerCallback\n";
}

VOID
IdleTimerCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Context,
    PTP_TIMER             Timer
)
{
    std::cout << "Begin IdleTimerCallback\n";

    std::cout << "End IdleTimerCallback\n";
}

int main()
{
    HRESULT hr;
    ULARGE_INTEGER ulDueTime;
    PVOID Context = NULL;

    // CoInitializeEx(NULL, COINIT_MULTITHREADED);

    STTIMER stIdleTimer;

    std::cout << "InitializeTimer\n";
    stIdleTimer.InitializeTimer((PTP_TIMER_CALLBACK)IdleTimerCallback, Context, 5000, 2000);

    Sleep(20000);

    /*
    std::cout << "CreateThreadpoolTimer\n"; 

    sm_pHealthTimer = CreateThreadpoolTimer((PTP_TIMER_CALLBACK)HealthTimerCallback,
        Context,
        NULL);
    if (sm_pHealthTimer == NULL)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto Finished;
    }

    //
    // Setup the fileTime object (for timer)
    //

    ulDueTime.QuadPart = (LONGLONG)HEALTH_CHECK_TIMER_INTERVAL_SECONDS
        * (-1 * 10 * 1000 * 1000);

    sm_ftTimeoutInterval.dwHighDateTime = ulDueTime.HighPart;
    sm_ftTimeoutInterval.dwLowDateTime = ulDueTime.LowPart;

    //
    // Set timer to fire.
    //

    std::cout << "SetThreadpoolTimer\n";
    SetThreadpoolTimer(sm_pHealthTimer,
        &sm_ftTimeoutInterval,
        0, // dueTime
        0);

    std::cout << "Sleep start\n";
    Sleep(20000);
    std::cout << "Sleep done\n";
    */

Finished:

    std::cout << "CancelTimer\n";
    stIdleTimer.CancelTimer();

    if (sm_pHealthTimer != NULL)
    {
        std::cout << "SetThreadpoolTimer NULL\n";
        SetThreadpoolTimer(sm_pHealthTimer,
            NULL,
            0,
            0);

        std::cout << "WaitForThreadpoolTimerCallbacks\n";
        WaitForThreadpoolTimerCallbacks(sm_pHealthTimer, TRUE);

        std::cout << "CloseThreadpoolTimer\n";
        CloseThreadpoolTimer(sm_pHealthTimer);
        sm_pHealthTimer = NULL;
    }

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
