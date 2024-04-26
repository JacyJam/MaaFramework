#pragma once

#include "../MaaToolkitDef.h"

#ifdef __cplusplus
extern "C"
{
#endif

    MAA_TOOLKIT_API MaaBool MaaToolkitRegisterCustomRecognizerExecutor(
        MaaInstanceHandle handle,
        MaaStringView recognizer_name,
        MaaStringView recognizer_exec_path,
        MaaStringView* recognizer_exec_params,
        MaaSize recognizer_exec_param_size);
    MAA_TOOLKIT_API MaaBool MaaToolkitUnregisterCustomRecognizerExecutor(
        MaaInstanceHandle handle,
        MaaStringView recognizer_name);
    MAA_TOOLKIT_API MaaBool MaaToolkitClearCustomRecognizerExecutor(MaaInstanceHandle handle);

    MAA_TOOLKIT_API MaaBool MaaToolkitRegisterCustomActionExecutor(
        MaaInstanceHandle handle,
        MaaStringView action_name,
        MaaStringView action_exec_path,
        MaaStringView* action_exec_params,
        MaaSize action_exec_param_size);
    MAA_TOOLKIT_API MaaBool MaaToolkitUnregisterCustomActionExecutor(
        MaaInstanceHandle handle,
        MaaStringView action_name);
    MAA_TOOLKIT_API MaaBool MaaToolkitClearCustomActionExecutor(MaaInstanceHandle handle);

#ifdef __cplusplus
}
#endif
