#ifndef __TRACYQUEUE_HPP__
#define __TRACYQUEUE_HPP__

#include <cassert>
#include <stddef.h>
#include <stdint.h>
#include "TracyEndian.hpp"

namespace tracy
{

enum class QueueType : uint8_t
{
    ZoneText,
    ZoneName,
    Message,
    MessageColor,
    MessageCallstack,
    MessageColorCallstack,
    MessageAppInfo,
    ZoneBeginAllocSrcLoc,
    ZoneBeginAllocSrcLocCallstack,
    CallstackSerial,
    Callstack,
    CallstackAlloc,
    CallstackSample,
    CallstackSampleContextSwitch,
    FrameImage,
    ZoneBegin,
    ZoneBeginCallstack,
    ZoneEnd,
    LockWait,
    LockObtain,
    LockRelease,
    LockSharedWait,
    LockSharedObtain,
    LockSharedRelease,
    LockName,
    MemAlloc,
    MemAllocNamed,
    MemFree,
    MemFreeNamed,
    MemAllocCallstack,
    MemAllocCallstackNamed,
    MemFreeCallstack,
    MemFreeCallstackNamed,
    MemDiscard,
    MemDiscardCallstack,
    GpuZoneBegin,
    GpuZoneBeginCallstack,
    GpuZoneBeginAllocSrcLoc,
    GpuZoneBeginAllocSrcLocCallstack,
    GpuZoneEnd,
    GpuZoneBeginSerial,
    GpuZoneBeginCallstackSerial,
    GpuZoneBeginAllocSrcLocSerial,
    GpuZoneBeginAllocSrcLocCallstackSerial,
    GpuZoneEndSerial,
    PlotDataInt,
    PlotDataFloat,
    PlotDataDouble,
    ContextSwitch,
    ThreadWakeup,
    GpuTime,
    GpuContextName,
    GpuAnnotationName,
    CallstackFrameSize,
    SymbolInformation,
    ExternalNameMetadata,
    SymbolCodeMetadata,
    SourceCodeMetadata,
    FiberEnter,
    FiberLeave,
    Terminate,
    KeepAlive,
    ThreadContext,
    GpuCalibration,
    GpuTimeSync,
    Crash,
    CrashReport,
    ZoneValidation,
    ZoneColor,
    ZoneValue,
    FrameMarkMsg,
    FrameMarkMsgStart,
    FrameMarkMsgEnd,
    FrameVsync,
    SourceLocation,
    LockAnnounce,
    LockTerminate,
    LockMark,
    MessageLiteral,
    MessageLiteralColor,
    MessageLiteralCallstack,
    MessageLiteralColorCallstack,
    GpuNewContext,
    CallstackFrame,
    SysTimeReport,
    SysPowerReport,
    TidToPid,
    HwSampleCpuCycle,
    HwSampleInstructionRetired,
    HwSampleCacheReference,
    HwSampleCacheMiss,
    HwSampleBranchRetired,
    HwSampleBranchMiss,
    PlotConfig,
    ParamSetup,
    AckServerQueryNoop,
    AckSourceCodeNotAvailable,
    AckSymbolCodeNotAvailable,
    CpuTopology,
    SingleStringData,
    SecondStringData,
    MemNamePayload,
    ThreadGroupHint,
    GpuZoneAnnotation,
    StringData,
    ThreadName,
    PlotName,
    SourceLocationPayload,
    CallstackPayload,
    CallstackAllocPayload,
    FrameName,
    FrameImageData,
    ExternalName,
    ExternalThreadName,
    SymbolCode,
    SourceCode,
    FiberName,
    NUM_TYPES
};

#pragma pack( push, 1 )

struct QueueThreadContext
{
    uint32_t thread;
};

struct QueueZoneBeginLean
{
    int64_t time;
};

struct QueueZoneBegin : public QueueZoneBeginLean
{
    uint64_t srcloc;    // ptr
};

struct QueueZoneBeginThread : public QueueZoneBegin
{
    uint32_t thread;
};

struct QueueZoneEnd
{
    int64_t time;
};

struct QueueZoneEndThread : public QueueZoneEnd
{
    uint32_t thread;
};

struct QueueZoneValidation
{
    uint32_t id;
};

struct QueueZoneValidationThread : public QueueZoneValidation
{
    uint32_t thread;
};

struct QueueZoneColor
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct QueueZoneColorThread : public QueueZoneColor
{
    uint32_t thread;
};

struct QueueZoneValue
{
    uint64_t value;
};

struct QueueZoneValueThread : public QueueZoneValue
{
    uint32_t thread;
};

struct QueueStringTransfer
{
    uint64_t ptr;
};

struct QueueFrameMark
{
    int64_t time;
    uint64_t name;      // ptr
};

struct QueueFrameVsync
{
    int64_t time;
    uint32_t id;
};

struct QueueFrameImage
{
    uint32_t frame;
    uint16_t w;
    uint16_t h;
    uint8_t flip;
};

struct QueueFrameImageFat : public QueueFrameImage
{
    uint64_t image;     // ptr
};

struct QueueSourceLocation
{
    uint64_t name;
    uint64_t function;  // ptr
    uint64_t file;      // ptr
    uint32_t line;
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct QueueZoneTextFat
{
    uint64_t text;      // ptr
    uint16_t size;
};

struct QueueZoneTextFatThread : public QueueZoneTextFat
{
    uint32_t thread;
};

enum class LockType : uint8_t
{
    Lockable,
    SharedLockable
};

struct QueueLockAnnounce
{
    uint32_t id;
    int64_t time;
    uint64_t lckloc;    // ptr
    LockType type;
};

struct QueueFiberEnter
{
    int64_t time;
    uint64_t fiber;     // ptr
    uint32_t thread;
    int32_t groupHint;
};

struct QueueFiberLeave
{
    int64_t time;
    uint32_t thread;
};

struct QueueLockTerminate
{
    uint32_t id;
    int64_t time;
};

struct QueueLockWait
{
    uint32_t thread;
    uint32_t id;
    int64_t time;
};

struct QueueLockObtain
{
    uint32_t thread;
    uint32_t id;
    int64_t time;
};

struct QueueLockRelease
{
    uint32_t id;
    int64_t time;
};

struct QueueLockReleaseShared : public QueueLockRelease
{
    uint32_t thread;
};

struct QueueLockMark
{
    uint32_t thread;
    uint32_t id;
    uint64_t srcloc;    // ptr
};

struct QueueLockName
{
    uint32_t id;
};

struct QueueLockNameFat : public QueueLockName
{
    uint64_t name;      // ptr
    uint16_t size;
};

struct QueuePlotDataBase
{
    uint64_t name;      // ptr
    int64_t time;
};

struct QueuePlotDataInt : public QueuePlotDataBase
{
    int64_t val;
};

struct QueuePlotDataFloat : public QueuePlotDataBase
{
    float val;
};

struct QueuePlotDataDouble : public QueuePlotDataBase
{
    double val;
};

struct QueueMessage
{
    int64_t time;
};

struct QueueMessageColor : public QueueMessage
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct QueueMessageLiteral : public QueueMessage
{
    uint64_t text;      // ptr
};

struct QueueMessageLiteralThread : public QueueMessageLiteral
{
    uint32_t thread;
};

struct QueueMessageColorLiteral : public QueueMessageColor
{
    uint64_t text;      // ptr
};

struct QueueMessageColorLiteralThread : public QueueMessageColorLiteral
{
    uint32_t thread;
};

struct QueueMessageFat : public QueueMessage
{
    uint64_t text;      // ptr
    uint16_t size;
};

struct QueueMessageFatThread : public QueueMessageFat
{
    uint32_t thread;
};

struct QueueMessageColorFat : public QueueMessageColor
{
    uint64_t text;      // ptr
    uint16_t size;
};

struct QueueMessageColorFatThread : public QueueMessageColorFat
{
    uint32_t thread;
};

// Don't change order, only add new entries at the end, this is also used on trace dumps!
enum class GpuContextType : uint8_t
{
    Invalid,
    OpenGl,
    Vulkan,
    OpenCL,
    Direct3D12,
    Direct3D11,
    Metal,
    Custom,
    CUDA,
    Rocprof
};

enum GpuContextFlags : uint8_t
{
    GpuContextCalibration   = 1 << 0
};

struct QueueGpuNewContext
{
    int64_t cpuTime;
    int64_t gpuTime;
    uint32_t thread;
    float period;
    uint8_t context;
    GpuContextFlags flags;
    GpuContextType type;
};

struct QueueGpuZoneBeginLean
{
    int64_t cpuTime;
    uint32_t thread;
    uint16_t queryId;
    uint8_t context;
};

struct QueueGpuZoneBegin : public QueueGpuZoneBeginLean
{
    uint64_t srcloc;
};

struct QueueGpuZoneEnd
{
    int64_t cpuTime;
    uint32_t thread;
    uint16_t queryId;
    uint8_t context;
};

struct QueueGpuZoneAnnotation
{
    int64_t noteId;
    double value;
    uint32_t thread;
    uint16_t queryId;
    uint8_t context;
};

struct QueueGpuTime
{
    int64_t gpuTime;
    uint16_t queryId;
    uint8_t context;
};

struct QueueGpuCalibration
{
    int64_t gpuTime;
    int64_t cpuTime;
    int64_t cpuDelta;
    uint8_t context;
};

struct QueueGpuTimeSync
{
    int64_t gpuTime;
    int64_t cpuTime;
    uint8_t context;
};

struct QueueGpuContextName
{
    uint8_t context;
};

struct QueueGpuContextNameFat : public QueueGpuContextName
{
    uint64_t ptr;
    uint16_t size;
};

struct QueueGpuAnnotationName
{
    int64_t noteId;
    uint8_t context;
};

struct QueueGpuAnnotationNameFat : public QueueGpuAnnotationName
{
    uint64_t ptr;
    uint16_t size;
};

struct QueueMemNamePayload
{
    uint64_t name;
};

struct QueueThreadGroupHint
{
    uint32_t thread;
    int32_t groupHint;
};

struct QueueMemAlloc
{
    int64_t time;
    uint32_t thread;
    uint64_t ptr;
    char size[6];
};

struct QueueMemFree
{
    int64_t time;
    uint32_t thread;
    uint64_t ptr;
};

struct QueueMemDiscard
{
    int64_t time;
    uint32_t thread;
    uint64_t name;
};

struct QueueCallstackFat
{
    uint64_t ptr;
};

struct QueueCallstackFatThread : public QueueCallstackFat
{
    uint32_t thread;
};

struct QueueCallstackAllocFat
{
    uint64_t ptr;
    uint64_t nativePtr;
};

struct QueueCallstackAllocFatThread : public QueueCallstackAllocFat
{
    uint32_t thread;
};

struct QueueCallstackSample
{
    int64_t time;
    uint32_t thread;
};

struct QueueCallstackSampleFat : public QueueCallstackSample
{
    uint64_t ptr;
};

struct QueueCallstackFrameSize
{
    uint64_t ptr;
    uint8_t size;
};

struct QueueCallstackFrameSizeFat : public QueueCallstackFrameSize
{
    uint64_t data;
    uint64_t imageName;
};

struct QueueCallstackFrame
{
    uint32_t line;
    uint64_t symAddr;
    uint32_t symLen;
};

struct QueueSymbolInformation
{
    uint32_t line;
    uint64_t symAddr;
};

struct QueueSymbolInformationFat : public QueueSymbolInformation
{
    uint64_t fileString;
    uint8_t needFree;
};

struct QueueCrashReport
{
    int64_t time;
    uint64_t text;      // ptr
};

struct QueueCrashReportThread
{
    uint32_t thread;
};

struct QueueSysTime
{
    int64_t time;
    float sysTime;
};

struct QueueSysPower
{
    int64_t time;
    uint64_t delta;
    uint64_t name;  // ptr
};

struct QueueContextSwitch
{
    int64_t time;
    uint32_t oldThread;
    uint32_t newThread;
    uint8_t cpu;
    uint8_t oldThreadWaitReason;
    uint8_t oldThreadState;
    uint8_t previousCState;
    int8_t newThreadPriority;
    int8_t oldThreadPriority;
};

struct QueueThreadWakeup
{
    int64_t time;
    uint32_t thread;
    uint8_t cpu;
    int8_t adjustReason;
    int8_t adjustIncrement;
};

struct QueueTidToPid
{
    uint64_t tid;
    uint64_t pid;
};

struct QueueHwSample
{
    uint64_t ip;
    int64_t time;
};

enum class PlotFormatType : uint8_t
{
    Number,
    Memory,
    Percentage
};

struct QueuePlotConfig
{
    uint64_t name;      // ptr
    uint8_t type;
    uint8_t step;
    uint8_t fill;
    uint32_t color;
};

struct QueueParamSetup
{
    uint32_t idx;
    uint64_t name;      // ptr
    uint8_t isBool;
    int32_t val;
};

struct QueueSourceCodeNotAvailable
{
    uint32_t id;
};

struct QueueCpuTopology
{
    uint32_t package;
    uint32_t die;
    uint32_t core;
    uint32_t thread;
};

struct QueueExternalNameMetadata
{
    uint64_t thread;
    uint64_t name;
    uint64_t threadName;
};

struct QueueSymbolCodeMetadata
{
    uint64_t symbol;
    uint64_t ptr;
    uint32_t size;
};

struct QueueSourceCodeMetadata
{
    uint64_t ptr;
    uint32_t size;
    uint32_t id;
};

struct QueueHeader
{
    union
    {
        QueueType type;
        uint8_t idx;
    };
};

struct QueueItem
{
    QueueHeader hdr;
    union
    {
        QueueThreadContext threadCtx;
        QueueZoneBegin zoneBegin;
        QueueZoneBeginLean zoneBeginLean;
        QueueZoneBeginThread zoneBeginThread;
        QueueZoneEnd zoneEnd;
        QueueZoneEndThread zoneEndThread;
        QueueZoneValidation zoneValidation;
        QueueZoneValidationThread zoneValidationThread;
        QueueZoneColor zoneColor;
        QueueZoneColorThread zoneColorThread;
        QueueZoneValue zoneValue;
        QueueZoneValueThread zoneValueThread;
        QueueStringTransfer stringTransfer;
        QueueFrameMark frameMark;
        QueueFrameVsync frameVsync;
        QueueFrameImage frameImage;
        QueueFrameImageFat frameImageFat;
        QueueSourceLocation srcloc;
        QueueZoneTextFat zoneTextFat;
        QueueZoneTextFatThread zoneTextFatThread;
        QueueLockAnnounce lockAnnounce;
        QueueLockTerminate lockTerminate;
        QueueLockWait lockWait;
        QueueLockObtain lockObtain;
        QueueLockRelease lockRelease;
        QueueLockReleaseShared lockReleaseShared;
        QueueLockMark lockMark;
        QueueLockName lockName;
        QueueLockNameFat lockNameFat;
        QueuePlotDataInt plotDataInt;
        QueuePlotDataFloat plotDataFloat;
        QueuePlotDataDouble plotDataDouble;
        QueueMessage message;
        QueueMessageColor messageColor;
        QueueMessageLiteral messageLiteral;
        QueueMessageLiteralThread messageLiteralThread;
        QueueMessageColorLiteral messageColorLiteral;
        QueueMessageColorLiteralThread messageColorLiteralThread;
        QueueMessageFat messageFat;
        QueueMessageFatThread messageFatThread;
        QueueMessageColorFat messageColorFat;
        QueueMessageColorFatThread messageColorFatThread;
        QueueGpuNewContext gpuNewContext;
        QueueGpuZoneBegin gpuZoneBegin;
        QueueGpuZoneBeginLean gpuZoneBeginLean;
        QueueGpuZoneEnd gpuZoneEnd;
        QueueGpuTime gpuTime;
        QueueGpuCalibration gpuCalibration;
        QueueGpuTimeSync gpuTimeSync;
        QueueGpuContextName gpuContextName;
        QueueGpuContextNameFat gpuContextNameFat;
        QueueGpuAnnotationName gpuAnnotationName;
        QueueGpuAnnotationNameFat gpuAnnotationNameFat;
        QueueMemAlloc memAlloc;
        QueueMemFree memFree;
        QueueMemDiscard memDiscard;
        QueueMemNamePayload memName;
        QueueThreadGroupHint threadGroupHint;
        QueueCallstackFat callstackFat;
        QueueCallstackFatThread callstackFatThread;
        QueueCallstackAllocFat callstackAllocFat;
        QueueCallstackAllocFatThread callstackAllocFatThread;
        QueueCallstackSample callstackSample;
        QueueCallstackSampleFat callstackSampleFat;
        QueueCallstackFrameSize callstackFrameSize;
        QueueCallstackFrameSizeFat callstackFrameSizeFat;
        QueueCallstackFrame callstackFrame;
        QueueSymbolInformation symbolInformation;
        QueueSymbolInformationFat symbolInformationFat;
        QueueCrashReport crashReport;
        QueueCrashReportThread crashReportThread;
        QueueSysTime sysTime;
        QueueSysPower sysPower;
        QueueContextSwitch contextSwitch;
        QueueThreadWakeup threadWakeup;
        QueueTidToPid tidToPid;
        QueueHwSample hwSample;
        QueuePlotConfig plotConfig;
        QueueParamSetup paramSetup;
        QueueCpuTopology cpuTopology;
        QueueExternalNameMetadata externalNameMetadata;
        QueueSymbolCodeMetadata symbolCodeMetadata;
        QueueSourceCodeMetadata sourceCodeMetadata;
        QueueSourceCodeNotAvailable sourceCodeNotAvailable;
        QueueFiberEnter fiberEnter;
        QueueFiberLeave fiberLeave;
        QueueGpuZoneAnnotation zoneAnnotation;
    };

	void convert_endian()
	{
		switch (hdr.type)
		{
			case QueueType::ThreadContext:
				threadCtx.thread = ::convert_endian(threadCtx.thread);
				break;
			case QueueType::ZoneBegin:
			case QueueType::ZoneBeginCallstack:
			case QueueType::ZoneBeginAllocSrcLoc:
			case QueueType::ZoneBeginAllocSrcLocCallstack:
				zoneBegin.time = ::convert_endian(zoneBegin.time);
				zoneBegin.srcloc = ::convert_endian(zoneBegin.srcloc);
				zoneBeginThread.thread = ::convert_endian(zoneBeginThread.thread);
				break;
			case QueueType::ZoneEnd:
				zoneEnd.time = ::convert_endian(zoneEnd.time);
				zoneEndThread.thread = ::convert_endian(zoneEndThread.thread);
				break;
			case QueueType::ZoneValidation:
				zoneValidation.id = ::convert_endian(zoneValidation.id);
				zoneValidationThread.thread = ::convert_endian(zoneValidationThread.thread);
				break;
			case QueueType::ZoneColor:
				zoneColor.b = ::convert_endian(zoneColor.b);
				zoneColor.g = ::convert_endian(zoneColor.g);
				zoneColor.r = ::convert_endian(zoneColor.r);
				zoneColorThread.thread = ::convert_endian(zoneColorThread.thread);
				break;
			case QueueType::ZoneValue:
				zoneValue.value = ::convert_endian(zoneValue.value);
				zoneValueThread.thread = ::convert_endian(zoneValueThread.thread);
				break;
			case QueueType::StringData:
			case QueueType::ThreadName:
			case QueueType::PlotName:
			case QueueType::FrameName:
			case QueueType::ExternalName:
			case QueueType::ExternalThreadName:
			case QueueType::FiberName:
			case QueueType::FrameImageData:
			case QueueType::SymbolCode:
			case QueueType::SourceCode:
			case QueueType::SourceLocationPayload:
			case QueueType::CallstackPayload:
			case QueueType::CallstackAllocPayload:
				stringTransfer.ptr = ::convert_endian(stringTransfer.ptr);
				break;
			case QueueType::FrameMarkMsgStart:
			case QueueType::FrameMarkMsgEnd:
			case QueueType::FrameMarkMsg:
				frameMark.name = ::convert_endian(frameMark.name);
				frameMark.time = ::convert_endian(frameMark.time);
				break;
			case QueueType::FrameVsync:
				frameVsync.id = ::convert_endian(frameVsync.id);
				frameVsync.time = ::convert_endian(frameVsync.time);
				break;
			case QueueType::FrameImage:
				frameImage.frame = ::convert_endian(frameImage.frame);
				frameImage.w = ::convert_endian(frameImage.w);
				frameImage.h = ::convert_endian(frameImage.h);
				frameImage.flip = ::convert_endian(frameImage.flip);
				frameImageFat.image = ::convert_endian(frameImageFat.image);
				break;
			case QueueType::SourceLocation:
				srcloc.name = ::convert_endian(srcloc.name);
				srcloc.file = ::convert_endian(srcloc.file);
				srcloc.function = ::convert_endian(srcloc.function);
				srcloc.line = ::convert_endian(srcloc.line);
				srcloc.b = ::convert_endian(srcloc.b);
				srcloc.g = ::convert_endian(srcloc.g);
				srcloc.r = ::convert_endian(srcloc.r);
				break;
			case QueueType::ZoneName:
			case QueueType::ZoneText:
				zoneTextFat.text = ::convert_endian(zoneTextFat.text);
				zoneTextFat.size = ::convert_endian(zoneTextFat.size);
				zoneTextFatThread.thread = ::convert_endian(zoneTextFatThread.thread);
				break;
			case QueueType::LockAnnounce:
				lockAnnounce.type = ::convert_endian(lockAnnounce.type);
				lockAnnounce.id = ::convert_endian(lockAnnounce.id);
				lockAnnounce.lckloc = ::convert_endian(lockAnnounce.lckloc);
				lockAnnounce.time = ::convert_endian(lockAnnounce.time);
				break;
			case QueueType::LockTerminate:
				lockTerminate.id = ::convert_endian(lockTerminate.id);
				lockTerminate.time = ::convert_endian(lockTerminate.time);
				break;
			case QueueType::LockWait:
			case QueueType::LockSharedWait:
				lockWait.id = ::convert_endian(lockWait.id);
				lockWait.thread = ::convert_endian(lockWait.thread);
				lockWait.time = ::convert_endian(lockWait.time);
				break;
			case QueueType::LockObtain:
			case QueueType::LockSharedObtain:
				lockObtain.id = ::convert_endian(lockObtain.id);
				lockObtain.thread = ::convert_endian(lockObtain.thread);
				lockObtain.time = ::convert_endian(lockObtain.time);
				break;
			case QueueType::LockRelease:
				lockRelease.id = ::convert_endian(lockRelease.id);
				lockRelease.time = ::convert_endian(lockRelease.time);
				break;
			case QueueType::LockSharedRelease:
				lockReleaseShared.id = ::convert_endian(lockReleaseShared.id);
				lockReleaseShared.time = ::convert_endian(lockReleaseShared.time);
				lockReleaseShared.thread = ::convert_endian(lockReleaseShared.thread);
				break;
			case QueueType::LockMark:
				lockMark.id = ::convert_endian(lockMark.id);
				lockMark.srcloc = ::convert_endian(lockMark.srcloc);
				lockMark.thread = ::convert_endian(lockMark.thread);
				break;
			case QueueType::LockName:
				lockNameFat.name = ::convert_endian(lockNameFat.name);
				lockNameFat.id = ::convert_endian(lockNameFat.id);
				lockNameFat.size = ::convert_endian(lockNameFat.size);
				break;
			case QueueType::PlotDataInt:
				plotDataInt.name = ::convert_endian(plotDataInt.name);
				plotDataInt.time = ::convert_endian(plotDataInt.time);
				plotDataInt.val = ::convert_endian(plotDataInt.val);
				break;
			case QueueType::PlotDataDouble:
				plotDataDouble.name = ::convert_endian(plotDataDouble.name);
				plotDataDouble.time = ::convert_endian(plotDataDouble.time);
				plotDataDouble.val = ::convert_endian(plotDataDouble.val);
				break;
			case QueueType::PlotDataFloat:
				plotDataFloat.name = ::convert_endian(plotDataFloat.name);
				plotDataFloat.time = ::convert_endian(plotDataFloat.time);
				plotDataFloat.val = ::convert_endian(plotDataFloat.val);
				break;
			case QueueType::Message:
			case QueueType::MessageAppInfo:
			case QueueType::MessageCallstack:
				messageFat.time = ::convert_endian(messageFat.time);
				messageFat.size = ::convert_endian(messageFat.size);
				messageFat.text = ::convert_endian(messageFat.text);
				messageFatThread.thread = ::convert_endian(messageFatThread.thread);
				break;
			case QueueType::MessageColor:
			case QueueType::MessageColorCallstack:
				messageColorFat.b = ::convert_endian(messageColorFat.b);
				messageColorFat.g = ::convert_endian(messageColorFat.g);
				messageColorFat.r = ::convert_endian(messageColorFat.r);
				messageColorFat.time = ::convert_endian(messageColorFat.time);
				messageColorFat.size = ::convert_endian(messageColorFat.size);
				messageColorFat.text = ::convert_endian(messageColorFat.text);
				messageColorFatThread.thread = ::convert_endian(messageColorFatThread.thread);
				break;
			case QueueType::MessageLiteral:
			case QueueType::MessageLiteralCallstack:
				messageLiteral.time = ::convert_endian(messageLiteral.time);
				messageLiteral.text = ::convert_endian(messageLiteral.text);
				messageLiteralThread.thread = ::convert_endian(messageLiteralThread.thread);
				break;
			case QueueType::MessageLiteralColor:
			case QueueType::MessageLiteralColorCallstack:
				messageColorLiteral.b = ::convert_endian(messageColorLiteral.b);
				messageColorLiteral.g = ::convert_endian(messageColorLiteral.g);
				messageColorLiteral.r = ::convert_endian(messageColorLiteral.r);
				messageColorLiteral.time = ::convert_endian(messageColorLiteral.time);
				messageColorLiteral.text = ::convert_endian(messageColorLiteral.text);
				messageColorLiteralThread.thread = ::convert_endian(messageColorLiteralThread.thread);
				break;
			case QueueType::GpuNewContext:
				gpuNewContext.context = ::convert_endian(gpuNewContext.context);
				gpuNewContext.type = ::convert_endian(gpuNewContext.type);
				gpuNewContext.flags = ::convert_endian(gpuNewContext.flags);
				gpuNewContext.cpuTime = ::convert_endian(gpuNewContext.cpuTime);
				gpuNewContext.gpuTime = ::convert_endian(gpuNewContext.gpuTime);
				gpuNewContext.period = ::convert_endian(gpuNewContext.period);
				gpuNewContext.thread = ::convert_endian(gpuNewContext.thread);
				break;
			case QueueType::GpuZoneBegin:
			case QueueType::GpuZoneBeginSerial:
			case QueueType::GpuZoneBeginCallstackSerial:
			case QueueType::GpuZoneBeginCallstack:
			case QueueType::GpuZoneBeginAllocSrcLoc:
			case QueueType::GpuZoneBeginAllocSrcLocCallstack:
			case QueueType::GpuZoneBeginAllocSrcLocSerial:
			case QueueType::GpuZoneBeginAllocSrcLocCallstackSerial:
				gpuZoneBegin.context = ::convert_endian(gpuZoneBegin.context);
				gpuZoneBegin.cpuTime = ::convert_endian(gpuZoneBegin.cpuTime);
				gpuZoneBegin.queryId = ::convert_endian(gpuZoneBegin.queryId);
				gpuZoneBegin.thread = ::convert_endian(gpuZoneBegin.thread);
				gpuZoneBegin.srcloc = ::convert_endian(gpuZoneBegin.srcloc);
				break;
			case QueueType::GpuZoneEnd:
			case QueueType::GpuZoneEndSerial:
				gpuZoneEnd.context = ::convert_endian(gpuZoneEnd.context);
				gpuZoneEnd.cpuTime = ::convert_endian(gpuZoneEnd.cpuTime);
				gpuZoneEnd.queryId = ::convert_endian(gpuZoneEnd.queryId);
				gpuZoneEnd.thread = ::convert_endian(gpuZoneEnd.thread);
				break;
			case QueueType::GpuTime:
				gpuTime.gpuTime = ::convert_endian(gpuTime.gpuTime);
				gpuTime.context = ::convert_endian(gpuTime.context);
				gpuTime.queryId = ::convert_endian(gpuTime.queryId);
				break;
			case QueueType::GpuCalibration:
				gpuCalibration.context = ::convert_endian(gpuCalibration.context);
				gpuCalibration.cpuDelta = ::convert_endian(gpuCalibration.cpuDelta);
				gpuCalibration.cpuTime = ::convert_endian(gpuCalibration.cpuTime);
				gpuCalibration.gpuTime = ::convert_endian(gpuCalibration.gpuTime);
				break;
			case QueueType::GpuTimeSync:
				gpuTimeSync.cpuTime = ::convert_endian(gpuTimeSync.cpuTime);
				gpuTimeSync.gpuTime = ::convert_endian(gpuTimeSync.gpuTime);
				gpuTimeSync.context = ::convert_endian(gpuTimeSync.context);
				break;
			case QueueType::GpuContextName:
				gpuContextNameFat.context = ::convert_endian(gpuContextNameFat.context);
				gpuContextNameFat.ptr = ::convert_endian(gpuContextNameFat.ptr);
				gpuContextNameFat.size = ::convert_endian(gpuContextNameFat.size);
				break;
			case QueueType::GpuAnnotationName:
				gpuAnnotationNameFat.context = ::convert_endian(gpuAnnotationNameFat.context);
				gpuAnnotationNameFat.noteId = ::convert_endian(gpuAnnotationNameFat.noteId);
				gpuAnnotationNameFat.size = ::convert_endian(gpuAnnotationNameFat.size);
				gpuAnnotationNameFat.ptr = ::convert_endian(gpuAnnotationNameFat.ptr);
				break;
			case QueueType::MemAlloc:
			case QueueType::MemAllocNamed:
			case QueueType::MemAllocCallstack:
			case QueueType::MemAllocCallstackNamed:
				memAlloc.ptr = ::convert_endian(memAlloc.ptr);
				::convert_endian(memAlloc.size);
				memAlloc.thread = ::convert_endian(memAlloc.thread);
				memAlloc.time = ::convert_endian(memAlloc.time);
				break;
			case QueueType::MemFree:
			case QueueType::MemFreeNamed:
			case QueueType::MemFreeCallstack:
			case QueueType::MemFreeCallstackNamed:
				memFree.ptr = ::convert_endian(memFree.ptr);
				memFree.thread = ::convert_endian(memFree.thread);
				memFree.time = ::convert_endian(memFree.time);
				break;
			case QueueType::MemDiscard:
			case QueueType::MemDiscardCallstack:
				memDiscard.name = ::convert_endian(memDiscard.name);
				memDiscard.thread = ::convert_endian(memDiscard.thread);
				memDiscard.time = ::convert_endian(memDiscard.time);
				break;
			case QueueType::MemNamePayload:
				memName.name = ::convert_endian(memName.name);
				break;
			case QueueType::ThreadGroupHint:
				threadGroupHint.groupHint = ::convert_endian(threadGroupHint.groupHint);
				threadGroupHint.thread = ::convert_endian(threadGroupHint.thread);
				break;
			case QueueType::Callstack:
			case QueueType::CallstackSerial:
				callstackFat.ptr = ::convert_endian(callstackFat.ptr);
				callstackFatThread.thread = ::convert_endian(callstackFatThread.thread);
				break;
			case QueueType::CallstackAlloc:
				callstackAllocFat.ptr = ::convert_endian(callstackAllocFat.ptr);
				callstackAllocFat.nativePtr = ::convert_endian(callstackAllocFat.nativePtr);
				callstackAllocFatThread.thread = ::convert_endian(callstackAllocFatThread.thread);
				break;
			case QueueType::CallstackSample:
			case QueueType::CallstackSampleContextSwitch:
				callstackSampleFat.thread = ::convert_endian(callstackSampleFat.thread);
				callstackSampleFat.time = ::convert_endian(callstackSampleFat.time);
				callstackSampleFat.ptr = ::convert_endian(callstackSampleFat.ptr);
				break;
			case QueueType::CallstackFrameSize:
				callstackFrameSizeFat.size = ::convert_endian(callstackFrameSizeFat.size);
				callstackFrameSizeFat.ptr = ::convert_endian(callstackFrameSizeFat.ptr);
				callstackFrameSizeFat.data = ::convert_endian(callstackFrameSizeFat.data);
				callstackFrameSizeFat.imageName = ::convert_endian(callstackFrameSizeFat.imageName);
				break;
			case QueueType::CallstackFrame:
				callstackFrame.line = ::convert_endian(callstackFrame.line);
				callstackFrame.symAddr = ::convert_endian(callstackFrame.symAddr);
				callstackFrame.symLen = ::convert_endian(callstackFrame.symLen);
				break;
			case QueueType::SymbolInformation:
				symbolInformationFat.line = ::convert_endian(symbolInformationFat.line);
				symbolInformationFat.symAddr = ::convert_endian(symbolInformationFat.symAddr);
				symbolInformationFat.fileString = ::convert_endian(symbolInformationFat.fileString);
				symbolInformationFat.needFree = ::convert_endian(symbolInformationFat.needFree);
				break;
			case QueueType::CrashReport:
				crashReport.text = ::convert_endian(crashReport.text);
				crashReport.time = ::convert_endian(crashReport.time);
				crashReportThread.thread = ::convert_endian(crashReportThread.thread);
				break;
			case QueueType::SysTimeReport:
				sysTime.sysTime = ::convert_endian(sysTime.sysTime);
				sysTime.time = ::convert_endian(sysTime.time);
				break;
			case QueueType::SysPowerReport:
				sysPower.delta = ::convert_endian(sysPower.delta);
				sysPower.name = ::convert_endian(sysPower.name);
				sysPower.time = ::convert_endian(sysPower.time);
				break;
			case QueueType::ContextSwitch:
				contextSwitch.cpu = ::convert_endian(contextSwitch.cpu);
				contextSwitch.newThread = ::convert_endian(contextSwitch.newThread);
				contextSwitch.newThreadPriority = ::convert_endian(contextSwitch.newThreadPriority);
				contextSwitch.oldThread = ::convert_endian(contextSwitch.oldThread);
				contextSwitch.oldThreadState = ::convert_endian(contextSwitch.oldThreadState);
				contextSwitch.oldThreadPriority = ::convert_endian(contextSwitch.oldThreadPriority);
				contextSwitch.oldThreadWaitReason = ::convert_endian(contextSwitch.oldThreadWaitReason);
				contextSwitch.time = ::convert_endian(contextSwitch.time);
				contextSwitch.previousCState = ::convert_endian(contextSwitch.previousCState);
				break;
			case QueueType::ThreadWakeup:
				threadWakeup.thread = ::convert_endian(threadWakeup.thread);
				threadWakeup.adjustIncrement = ::convert_endian(threadWakeup.adjustIncrement);
				threadWakeup.adjustReason = ::convert_endian(threadWakeup.adjustReason);
				threadWakeup.time = ::convert_endian(threadWakeup.time);
				threadWakeup.cpu = ::convert_endian(threadWakeup.cpu);
				break;
			case QueueType::TidToPid:
				tidToPid.pid = ::convert_endian(tidToPid.pid);
				tidToPid.tid = ::convert_endian(tidToPid.tid);
				break;
			case QueueType::HwSampleCpuCycle:
			case QueueType::HwSampleInstructionRetired:
			case QueueType::HwSampleCacheReference:
			case QueueType::HwSampleCacheMiss:
			case QueueType::HwSampleBranchRetired:
			case QueueType::HwSampleBranchMiss:
				hwSample.ip = ::convert_endian(hwSample.ip);
				hwSample.time = ::convert_endian(hwSample.time);
				break;
			case QueueType::PlotConfig:
				plotConfig.type = ::convert_endian(plotConfig.type);
				plotConfig.color = ::convert_endian(plotConfig.color);
				plotConfig.fill = ::convert_endian(plotConfig.fill);
				plotConfig.step = ::convert_endian(plotConfig.step);
				plotConfig.name = ::convert_endian(plotConfig.name);
				break;
			case QueueType::ParamSetup:
				paramSetup.idx = ::convert_endian(paramSetup.idx);
				paramSetup.isBool = ::convert_endian(paramSetup.isBool);
				paramSetup.name = ::convert_endian(paramSetup.name);
				paramSetup.val = ::convert_endian(paramSetup.val);
				break;
			case QueueType::CpuTopology:
				cpuTopology.core = ::convert_endian(cpuTopology.core);
				cpuTopology.die = ::convert_endian(cpuTopology.die);
				cpuTopology.thread = ::convert_endian(cpuTopology.thread);
				cpuTopology.package = ::convert_endian(cpuTopology.package);
				break;
			case QueueType::ExternalNameMetadata:
				externalNameMetadata.name = ::convert_endian(externalNameMetadata.name);
				externalNameMetadata.thread = ::convert_endian(externalNameMetadata.thread);
				externalNameMetadata.threadName = ::convert_endian(externalNameMetadata.threadName);
				break;
			case QueueType::SymbolCodeMetadata:
				symbolCodeMetadata.symbol = ::convert_endian(symbolCodeMetadata.symbol);
				symbolCodeMetadata.ptr = ::convert_endian(symbolCodeMetadata.ptr);
				symbolCodeMetadata.size = ::convert_endian(symbolCodeMetadata.size);
				break;
			case QueueType::SourceCodeMetadata:
				sourceCodeMetadata.id = ::convert_endian(sourceCodeMetadata.id);
				sourceCodeMetadata.ptr = ::convert_endian(sourceCodeMetadata.ptr);
				sourceCodeMetadata.size = ::convert_endian(sourceCodeMetadata.size);
				break;
			case QueueType::AckSourceCodeNotAvailable:
				sourceCodeNotAvailable.id = ::convert_endian(sourceCodeNotAvailable.id);
				break;
			case QueueType::FiberEnter:
				fiberEnter.fiber = ::convert_endian(fiberEnter.fiber);
				fiberEnter.groupHint = ::convert_endian(fiberEnter.groupHint);
				fiberEnter.thread = ::convert_endian(fiberEnter.thread);
				fiberEnter.time = ::convert_endian(fiberEnter.time);
				break;
			case QueueType::FiberLeave:
				fiberLeave.thread = ::convert_endian(fiberLeave.thread);
				fiberLeave.time = ::convert_endian(fiberLeave.time);
				break;
			case QueueType::GpuZoneAnnotation:
				zoneAnnotation.context = ::convert_endian(zoneAnnotation.context);
				zoneAnnotation.noteId = ::convert_endian(zoneAnnotation.noteId);
				zoneAnnotation.queryId = ::convert_endian(zoneAnnotation.queryId);
				zoneAnnotation.thread = ::convert_endian(zoneAnnotation.thread);
				zoneAnnotation.value = ::convert_endian(zoneAnnotation.value);
				break;
			case QueueType::Terminate:
			case QueueType::KeepAlive:
			case QueueType::Crash:
			case QueueType::AckServerQueryNoop:
			case QueueType::AckSymbolCodeNotAvailable:
			case QueueType::SingleStringData:
			case QueueType::SecondStringData:
				break;
			default:
				assert(false);
				break;
		}
	}
};
#pragma pack( pop )


enum { QueueItemSize = sizeof( QueueItem ) };

static constexpr size_t QueueDataSize[] = {
    sizeof( QueueHeader ),                                  // zone text
    sizeof( QueueHeader ),                                  // zone name
    sizeof( QueueHeader ) + sizeof( QueueMessage ),
    sizeof( QueueHeader ) + sizeof( QueueMessageColor ),
    sizeof( QueueHeader ) + sizeof( QueueMessage ),         // callstack
    sizeof( QueueHeader ) + sizeof( QueueMessageColor ),    // callstack
    sizeof( QueueHeader ) + sizeof( QueueMessage ),         // app info
    sizeof( QueueHeader ) + sizeof( QueueZoneBeginLean ),   // allocated source location
    sizeof( QueueHeader ) + sizeof( QueueZoneBeginLean ),   // allocated source location, callstack
    sizeof( QueueHeader ),                                  // callstack memory
    sizeof( QueueHeader ),                                  // callstack
    sizeof( QueueHeader ),                                  // callstack alloc
    sizeof( QueueHeader ) + sizeof( QueueCallstackSample ),
    sizeof( QueueHeader ) + sizeof( QueueCallstackSample ), // context switch
    sizeof( QueueHeader ) + sizeof( QueueFrameImage ),
    sizeof( QueueHeader ) + sizeof( QueueZoneBegin ),
    sizeof( QueueHeader ) + sizeof( QueueZoneBegin ),       // callstack
    sizeof( QueueHeader ) + sizeof( QueueZoneEnd ),
    sizeof( QueueHeader ) + sizeof( QueueLockWait ),
    sizeof( QueueHeader ) + sizeof( QueueLockObtain ),
    sizeof( QueueHeader ) + sizeof( QueueLockRelease ),
    sizeof( QueueHeader ) + sizeof( QueueLockWait ),        // shared
    sizeof( QueueHeader ) + sizeof( QueueLockObtain ),      // shared
    sizeof( QueueHeader ) + sizeof( QueueLockReleaseShared ),
    sizeof( QueueHeader ) + sizeof( QueueLockName ),
    sizeof( QueueHeader ) + sizeof( QueueMemAlloc ),
    sizeof( QueueHeader ) + sizeof( QueueMemAlloc ),        // named
    sizeof( QueueHeader ) + sizeof( QueueMemFree ),
    sizeof( QueueHeader ) + sizeof( QueueMemFree ),         // named
    sizeof( QueueHeader ) + sizeof( QueueMemAlloc ),        // callstack
    sizeof( QueueHeader ) + sizeof( QueueMemAlloc ),        // callstack, named
    sizeof( QueueHeader ) + sizeof( QueueMemFree ),         // callstack
    sizeof( QueueHeader ) + sizeof( QueueMemFree ),         // callstack, named
    sizeof( QueueHeader ) + sizeof( QueueMemDiscard ),
    sizeof( QueueHeader ) + sizeof( QueueMemDiscard ),      // callstack
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneBegin ),
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneBegin ),    // callstack
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneBeginLean ),// allocated source location
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneBeginLean ),// allocated source location, callstack
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneEnd ),
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneBegin ),    // serial
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneBegin ),    // serial, callstack
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneBeginLean ),// serial, allocated source location
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneBeginLean ),// serial, allocated source location, callstack
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneEnd ),      // serial
    sizeof( QueueHeader ) + sizeof( QueuePlotDataInt ),
    sizeof( QueueHeader ) + sizeof( QueuePlotDataFloat ),
    sizeof( QueueHeader ) + sizeof( QueuePlotDataDouble ),
    sizeof( QueueHeader ) + sizeof( QueueContextSwitch ),
    sizeof( QueueHeader ) + sizeof( QueueThreadWakeup ),
    sizeof( QueueHeader ) + sizeof( QueueGpuTime ),
    sizeof( QueueHeader ) + sizeof( QueueGpuContextName ),
    sizeof( QueueHeader ) + sizeof( QueueGpuAnnotationName ),
    sizeof( QueueHeader ) + sizeof( QueueCallstackFrameSize ),
    sizeof( QueueHeader ) + sizeof( QueueSymbolInformation ),
    sizeof( QueueHeader ),                                  // ExternalNameMetadata - not for wire transfer
    sizeof( QueueHeader ),                                  // SymbolCodeMetadata - not for wire transfer
    sizeof( QueueHeader ),                                  // SourceCodeMetadata - not for wire transfer
    sizeof( QueueHeader ) + sizeof( QueueFiberEnter ),
    sizeof( QueueHeader ) + sizeof( QueueFiberLeave ),
    // above items must be first
    sizeof( QueueHeader ),                                  // terminate
    sizeof( QueueHeader ),                                  // keep alive
    sizeof( QueueHeader ) + sizeof( QueueThreadContext ),
    sizeof( QueueHeader ) + sizeof( QueueGpuCalibration ),
    sizeof( QueueHeader ) + sizeof( QueueGpuTimeSync ),
    sizeof( QueueHeader ),                                  // crash
    sizeof( QueueHeader ) + sizeof( QueueCrashReport ),
    sizeof( QueueHeader ) + sizeof( QueueZoneValidation ),
    sizeof( QueueHeader ) + sizeof( QueueZoneColor ),
    sizeof( QueueHeader ) + sizeof( QueueZoneValue ),
    sizeof( QueueHeader ) + sizeof( QueueFrameMark ),       // continuous frames
    sizeof( QueueHeader ) + sizeof( QueueFrameMark ),       // start
    sizeof( QueueHeader ) + sizeof( QueueFrameMark ),       // end
    sizeof( QueueHeader ) + sizeof( QueueFrameVsync ),
    sizeof( QueueHeader ) + sizeof( QueueSourceLocation ),
    sizeof( QueueHeader ) + sizeof( QueueLockAnnounce ),
    sizeof( QueueHeader ) + sizeof( QueueLockTerminate ),
    sizeof( QueueHeader ) + sizeof( QueueLockMark ),
    sizeof( QueueHeader ) + sizeof( QueueMessageLiteral ),
    sizeof( QueueHeader ) + sizeof( QueueMessageColorLiteral ),
    sizeof( QueueHeader ) + sizeof( QueueMessageLiteral ),  // callstack
    sizeof( QueueHeader ) + sizeof( QueueMessageColorLiteral ), // callstack
    sizeof( QueueHeader ) + sizeof( QueueGpuNewContext ),
    sizeof( QueueHeader ) + sizeof( QueueCallstackFrame ),
    sizeof( QueueHeader ) + sizeof( QueueSysTime ),
    sizeof( QueueHeader ) + sizeof( QueueSysPower ),
    sizeof( QueueHeader ) + sizeof( QueueTidToPid ),
    sizeof( QueueHeader ) + sizeof( QueueHwSample ),        // cpu cycle
    sizeof( QueueHeader ) + sizeof( QueueHwSample ),        // instruction retired
    sizeof( QueueHeader ) + sizeof( QueueHwSample ),        // cache reference
    sizeof( QueueHeader ) + sizeof( QueueHwSample ),        // cache miss
    sizeof( QueueHeader ) + sizeof( QueueHwSample ),        // branch retired
    sizeof( QueueHeader ) + sizeof( QueueHwSample ),        // branch miss
    sizeof( QueueHeader ) + sizeof( QueuePlotConfig ),
    sizeof( QueueHeader ) + sizeof( QueueParamSetup ),
    sizeof( QueueHeader ),                                  // server query acknowledgement
    sizeof( QueueHeader ) + sizeof( QueueSourceCodeNotAvailable ),
    sizeof( QueueHeader ),                                  // symbol code not available
    sizeof( QueueHeader ) + sizeof( QueueCpuTopology ),
    sizeof( QueueHeader ),                                  // single string data
    sizeof( QueueHeader ),                                  // second string data
    sizeof( QueueHeader ) + sizeof( QueueMemNamePayload ),
    sizeof( QueueHeader ) + sizeof( QueueThreadGroupHint ),
    sizeof( QueueHeader ) + sizeof( QueueGpuZoneAnnotation ), // GPU zone annotation
    // keep all QueueStringTransfer below
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // string data
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // thread name
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // plot name
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // allocated source location payload
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // callstack payload
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // callstack alloc payload
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // frame name
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // frame image data
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // external name
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // external thread name
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // symbol code
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // source code
    sizeof( QueueHeader ) + sizeof( QueueStringTransfer ),  // fiber name
};

static_assert( QueueItemSize == 32, "Queue item size not 32 bytes" );
static_assert( sizeof( QueueDataSize ) / sizeof( size_t ) == (uint8_t)QueueType::NUM_TYPES, "QueueDataSize mismatch" );
static_assert( sizeof( void* ) <= sizeof( uint64_t ), "Pointer size > 8 bytes" );
static_assert( sizeof( void* ) == sizeof( uintptr_t ), "Pointer size != uintptr_t" );

}

#endif
