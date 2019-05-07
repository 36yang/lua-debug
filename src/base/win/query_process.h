#pragma once

#include <Windows.h>
#include <functional>

namespace base { namespace win {

	typedef struct _LSA_UNICODE_STRING
	{
		USHORT  Length;
		USHORT  MaximumLength;
		PWSTR   Buffer;
	}LSA_UNICODE_STRING, *PLSA_UNICODE_STRING;
	typedef LSA_UNICODE_STRING UNICODE_STRING, *PUNICODE_STRING;

	typedef struct _VM_COUNTERS
	{
		ULONG PeakVirtualSize;                  //����洢��ֵ��С��
		ULONG VirtualSize;                      //����洢��С��
		ULONG PageFaultCount;                   //ҳ������Ŀ��
		ULONG PeakWorkingSetSize;               //��������ֵ��С��
		ULONG WorkingSetSize;                   //��������С��
		ULONG QuotaPeakPagedPoolUsage;          //��ҳ��ʹ������ֵ��
		ULONG QuotaPagedPoolUsage;              //��ҳ��ʹ����
		ULONG QuotaPeakNonPagedPoolUsage;       //�Ƿ�ҳ��ʹ������ֵ��
		ULONG QuotaNonPagedPoolUsage;           //�Ƿ�ҳ��ʹ����
		ULONG PagefileUsage;                    //ҳ�ļ�ʹ�������
		ULONG PeakPagefileUsage;                //ҳ�ļ�ʹ�÷�ֵ��
	}VM_COUNTERS, *PVM_COUNTERS;

	typedef LONG KPRIORITY;

	typedef struct _SYSTEM_PROCESS_INFORMATION {
		ULONG                     NextEntryOffset;
		ULONG                     NumberOfThreads;
		LARGE_INTEGER             Reserved[3];
		LARGE_INTEGER             CreateTime;
		LARGE_INTEGER             UserTime;
		LARGE_INTEGER             KernelTime;
		UNICODE_STRING            ImageName;
		KPRIORITY                 BasePriority;
		HANDLE                    ProcessId;
		HANDLE                    InheritedFromProcessId;
		ULONG                     HandleCount;
		ULONG                     Reserved2[2];
		ULONG                     PrivatePageCount;
		VM_COUNTERS               VirtualMemoryCounters;
		IO_COUNTERS               IoCounters;
		//SYSTEM_THREAD_INFORMATION Threads[0];
	} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

	bool query_process(std::function<bool(const SYSTEM_PROCESS_INFORMATION*)> NtCBProcessInformation);
}}
