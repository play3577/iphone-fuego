//----------------------------------------------------------------------------
/** @file SgPlatform.cpp */
//----------------------------------------------------------------------------

#include "SgSystem.h"
#include "SgPlatform.h"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#ifdef HW_PHYSMEM
#include <sys/sysctl.h>
#endif

//----------------------------------------------------------------------------

std::size_t SgPlatform::TotalMemory()
{
#if defined WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (! GlobalMemoryStatusEx(&status))
        return 0;
    return static_cast<size_t>(status.ullTotalPhys);
#elif defined _SC_PHYS_PAGES
    long pages = sysconf(_SC_PHYS_PAGES);
    if (pages < 0)
        return 0;
    long pageSize = sysconf(_SC_PAGE_SIZE);
    if (pageSize < 0)
        return 0;
    return static_cast<size_t>(pages) * static_cast<size_t>(pageSize);
#elif defined HW_PHYSMEM
    // Mac OSX, BSD
    unsigned int mem;
    size_t len = sizeof mem;
    int mib[2] = { CTL_HW, HW_PHYSMEM };
    if (sysctl(mib, 2, &mem, &len, 0, 0) != 0 || len != sizeof mem)
        return 0;
    else
        return mem;
#else
    return 0;
#endif
}

//----------------------------------------------------------------------------

