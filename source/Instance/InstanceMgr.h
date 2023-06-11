#pragma once

#include "Base/AsyncCallback.hpp"
#include "Base/AsyncRunner.hpp"
#include "Common/MaaMsg.h"
#include "Common/MaaTypes.h"
#include "InstanceInternalAPI.hpp"
#include "Task/AbstractTask.h"

#include <mutex>

MAA_NS_BEGIN

class InstanceMgr : public MaaInstanceAPI, public InstanceInternalAPI
{
public:
    InstanceMgr(MaaInstanceCallback callback, void* callback_arg);
    virtual ~InstanceMgr() override;

public: // from MaaInstanceAPI
    virtual bool bind_resource(MaaResourceAPI* resource) override;
    virtual bool bind_controller(MaaControllerAPI* controller) override;
    virtual bool inited() const override;

    virtual bool set_option(MaaInstOption key, const std::string& value) override;

    virtual MaaTaskId post_task(MaaTaskType type, std::string_view param) override;
    virtual bool set_task_param(MaaTaskId task_id, std::string_view param) override;
    virtual MaaStatus status(MaaTaskId task_id) const override;
    virtual MaaStatus wait(MaaTaskId task_id) const override;
    virtual MaaBool all_finished() const override;

    virtual void stop() override;

    virtual std::string get_resource_hash() const override;
    virtual std::string get_controller_uuid() const override;

public: // from InstanceInternalAPI
    virtual MaaResourceAPI* resource() override;
    virtual MaaControllerAPI* controller() override;
    // TODO: status

private:
    using TaskPtr = std::shared_ptr<TaskNS::AbstractTask>;

    bool run_task(typename AsyncRunner<TaskPtr>::Id id, TaskPtr task_ptr);

private:
    MaaResourceAPI* resource_ = nullptr;
    MaaControllerAPI* controller_ = nullptr;
    std::mutex task_mutex_;
    std::map<typename AsyncRunner<TaskPtr>::Id, TaskPtr> task_map_;

    std::unique_ptr<AsyncRunner<TaskPtr>> task_runner_ = nullptr;
    AsyncCallback<MaaInstanceCallback, void*> notifier;
};

MAA_NS_END