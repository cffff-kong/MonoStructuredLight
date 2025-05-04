#include <iostream>
#include "DLP4500/src/dlpc350_common.h"
#include "DLP4500/src/dlpc350_firmware.h"
#include "DLP4500/src/dlpc350_common.h"
#include "DLP4500/src/dlpc350_error.h"
#include "dlpc350_usb.h"
#include "DLP4500/src/dlpc350_api.h"
#include "DLP4500/src/dlpc350_BMPParser.h"
#include "DLP4500/src/dlpc350_firmware.h"
#include "DLP4500/src/dlpc350_version.h"
#ifdef DLPOPERATION_EXPORTS
#define DLPOPERATION_API __declspec(dllexport)
#else
#define DLPOPERATION_API __declspec(dllimport)
#endif
class DLPOPERATION_API DLPOperation
{
public:
    DLPOperation();
    ~DLPOperation();

    void CheckDLPIsConnect();

};