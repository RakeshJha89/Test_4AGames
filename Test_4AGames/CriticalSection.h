#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

//--------Critical Section

#define CRITICAL_SEC_OBJ m_cs
#define DEF_CRIT_OBJ CRITICAL_SECTION CRITICAL_SEC_OBJ;

#define INIT_CRITICAL_SEC(x) InitializeCriticalSection(&x);

#define ENTER_CRITICAL_SEC(x) EnterCriticalSection(&x);
#define LEAVE_CRITICAL_SEC(x) LeaveCriticalSection(&x);

#define DEL_CRITICAL_SEC(x) DeleteCriticalSection(&x);
//-------------------------


#endif