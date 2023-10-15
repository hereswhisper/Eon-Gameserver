#pragma once
#include "ue.h"

void (*ProcessEventOG)(void*,void*,void*);
void ProcessEventHook(UObject* Obj, UFunction* Func, void* Params)
{
	if (!Obj->GetName().contains("Vehicle") && Func->GetName().contains("Overlap")/* || Func->GetName().contains("Hit")*/)
	{
		cout << Func->GetName() << endl;
		cout << Obj->GetName() << endl;
		cout << __int64(*(void**)(__int64(Func) + 0xF0)) - __int64(GetModuleHandleW(0)) << endl;
		cout << __int64((*(void***)Obj->Class->DefaultObject)) - __int64(GetModuleHandleW(0)) << endl;
	}

	return ProcessEventOG(Obj, Func, Params);
}