#include "ResourceMgr.h"

#include "Resource/AdbConfig.h"
#include "Resource/PipelineConfig.h"
#include "Utils/Logger.hpp"

MAA_RES_NS_BEGIN

ResourceMgr::ResourceMgr(const std::filesystem::path& user_path, MaaResourceCallback callback, void* callback_arg)
    : user_path_(user_path), notifier(callback, callback_arg)
{
    LogFunc << VAR(user_path_) << VAR_VOIDP(callback) << VAR_VOIDP(callback_arg);

    res_loader_ = std::make_unique<AsyncRunner<std::filesystem::path>>(
        std::bind(&ResourceMgr::run_load, this, std::placeholders::_1, std::placeholders::_2));
}

ResourceMgr::~ResourceMgr()
{
    LogFunc;

    if (res_loader_) {
        res_loader_->release();
    }

    notifier.release();
}

bool ResourceMgr::set_option(MaaResOption key, const std::string& value)
{
    return false;
}

MaaResId ResourceMgr::post_resource(std::filesystem::path path)
{
    LogInfo << VAR(path);

    loaded_ = false;

    if (!res_loader_) {
        LogError << "res_loader_ is nullptr";
        return MaaInvalidId;
    }

    return res_loader_->post(std::move(path));
}

MaaStatus ResourceMgr::status(MaaResId res_id) const
{
    if (!res_loader_) {
        LogError << "res_loader_ is nullptr";
        return MaaStatus_Invalid;
    }
    return res_loader_->status(res_id);
}

MaaStatus ResourceMgr::wait(MaaResId res_id) const
{
    if (!res_loader_) {
        LogError << "res_loader_ is nullptr";
        return false;
    }
    res_loader_->wait(res_id);
    return res_loader_->status(res_id);
}

MaaBool ResourceMgr::loaded() const
{
    return loaded_;
}

std::string ResourceMgr::get_hash() const
{
    return std::string();
}

bool ResourceMgr::run_load(typename AsyncRunner<std::filesystem::path>::Id id, std::filesystem::path path)
{
    LogFunc << VAR(id) << VAR(path);

    const json::value details = {
        { "id", id },
        { "path", path_to_utf8_string(path) },
    };

    notifier.notify(MaaMsg::ResourceStartLoading, details);

    loaded_ = load(path);

    notifier.notify(loaded_ ? MaaMsg::ResourceLoadingCompleted : MaaMsg::ResourceLoadingError, details);

    return loaded_;
}

bool ResourceMgr::load(const std::filesystem::path& path)
{
    LogFunc << VAR(path);

    json::value props = json::open(path / "properties.json").value_or(json::value());
    bool is_base = props.get("is_base", false);

    auto load_cfg = [&](const std::filesystem::path& cfg_path, auto& cfg) {
        if (std::filesystem::exists(cfg_path)) {
            if (is_base || !cfg) {
                cfg = std::make_shared<std::remove_reference_t<decltype(*cfg)>>();
            }
            return cfg->load(cfg_path, is_base);
        }
        return !is_base;
    };

    bool ret = load_cfg(path / "controller" / "adb.json", adb_cfg_) && load_cfg(path / "pipeline", pipeline_cfg_);

    LogInfo << VAR(path) << VAR(ret);

    return ret;
}

MAA_RES_NS_END