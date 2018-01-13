#pragma once

#include "ComponentManager.h"
#include <iostream>
#include <signal.h>
#include <atomic>

#ifdef SHAPE_PLATFORM_POSIX
#   include <dlfcn.h>
#else
#   include <Windows.h>
#endif

namespace shape {

  static volatile std::atomic<bool> exiting(false);

  //catch CTRL-C to exit
  void signalHandler(int signal)
  {
    switch (signal)
    {
    case SIGINT:
    case SIGTERM:
    case SIGABRT:
#ifndef _WIN32
    case SIGQUIT:
    case SIGKILL:
    case SIGSTOP:
    case SIGHUP:
#endif
      if (!exiting) {
        exiting = true;
        ComponentManager::getInst().exit();
      }
      break;

    default:
      // ...
      break;
    }
  }

#ifdef _WIN32
  BOOL WINAPI consoleHandlerRoutine(DWORD dwCtrlType)
  {
    if (dwCtrlType == CTRL_CLOSE_EVENT)
    {
      if (!exiting) {
        exiting = true;
        ComponentManager::getInst().exit(true);
      }
      return TRUE;
    }
    else if (exiting) {
      return TRUE;
    }
    else
      return FALSE;
  }
#endif

  int initSignals()
  {
    int retval = -1;

#if defined(_WIN32) && defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    if (SIG_ERR == signal(SIGINT, signalHandler))
    {
      std::cerr << "\nERROR: Could not set control handler for SIGINT";
      return -1;
    }
    if (SIG_ERR == signal(SIGTERM, signalHandler))
    {
      std::cerr << "\nERROR: Could not set control handler for SIGTERM";
      return -1;
    }
#ifndef _WIN32
    if (SIG_ERR == signal(SIGQUIT, signalHandler))
    {
      std::cerr << "\nERROR: Could not set control handler for SIGQUIT";
      return -1;
    }
#endif
    if (SIG_ERR == signal(SIGABRT, signalHandler))
    {
      std::cerr << "\nERROR: Could not set control handler for SIGABRT";
      return -1;
    }

#ifdef _WIN32
    if (!SetConsoleCtrlHandler(consoleHandlerRoutine, TRUE))
    {
      std::cerr << "\nERROR: Failed SetConsoleCtrlHandler(ConsoleHandlerRoutine) " << GetLastError();
    }
#endif
    retval = 0; //init OK
    return retval;
  }

}
