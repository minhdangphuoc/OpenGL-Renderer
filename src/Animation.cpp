#include "Animation.h"
#include "Bone.h"
#include <string>
#include <map>
#include <filesystem>
#include <iostream>
#include <algorithm>

namespace anim
{
    Animation::Animation(const char *file_path)
        : path_(file_path)
    {
    }
    Bone *Animation::find_bone(const std::string &name)
    {
        auto iter = name_bone_map_.find(name);
        if (iter == name_bone_map_.end())
            return nullptr;
        else
            return &(*(iter->second));
    }
    float Animation::get_fps() { return fps_; }
    float Animation::get_duration() { return duration_; }
    float Animation::get_current_duration()
    {
        float ret = 0.0f;
        for (auto &bone : name_bone_map_)
        {
            ret = std::max(ret, *bone.second->get_time_set().rbegin());
        }
        return ret;
    }

    const std::string &Animation::get_name() const
    {
        return name_;
    }
    const char *Animation::get_path() const
    {
        return path_.c_str();
    }
    const std::map<std::string, std::unique_ptr<Bone>> &Animation::get_name_bone_map() const
    {
        return name_bone_map_;
    }
    std::map<std::string, std::unique_ptr<Bone>> &Animation::get_mutable_name_bone_map()
    {
        return name_bone_map_;
    }
    const AnimationType &Animation::get_type() const
    {
        return type_;
    }
    void Animation::reload()
    {
        // TODO: REIMPORT
#ifndef NDEBUG
        std::cout << "reload:anim" << std::endl;
#endif
    }

    void Animation::get_ai_animation(aiAnimation *ai_anim, const aiNode *ai_root_node, float factor, bool is_linear)
    {
        std::filesystem::path p = std::filesystem::u8path(path_.c_str());
        std::string anim_name = p.filename().string();
        unsigned int size = name_bone_map_.size();
        std::vector<aiNodeAnim *> channels;

        float duration = 0.0f;
        for (auto &name_bone : name_bone_map_)
        {
            const aiNode *node = ai_root_node->FindNode(name_bone.first.c_str());
            if (node && name_bone.second->get_time_set().size() != 0)
            {
                LOG("find node:" + name_bone.first);
                channels.emplace_back(new aiNodeAnim());
                name_bone.second->get_ai_node(channels.back(), node->mTransformation, factor, is_linear);
                auto time_end = *std::next(name_bone.second->get_time_set().end(), -1);
                duration = std::max(duration, time_end);
            }
        }

        ai_anim->mTicksPerSecond = static_cast<double>(floorf(fps_ * factor));
        ai_anim->mName = aiString(anim_name);
        ai_anim->mDuration = static_cast<double>(duration + 1.0);
        LOG("duration:" + std::to_string(duration));

        ai_anim->mNumChannels = channels.size();
        ai_anim->mChannels = new aiNodeAnim *[channels.size()];
        for (int i = 0; i < ai_anim->mNumChannels; i++)
        {
            ai_anim->mChannels[i] = channels[i];
        }
    }
    void Animation::set_id(int id)
    {
        id_ = id;
    }
    const int Animation::get_id() const
    {
        return id_;
    }
    void Animation::add_and_replace_bone(const std::string &name, const glm::mat4 &transform, float time)
    {
        auto bone = find_bone(name);
        if (bone)
        {
            LOG("bone: " + name);
            bone->replace_or_add_keyframe(transform, time);
        }
        else
        {
            LOG("bone not found: " + name);
            name_bone_map_[name] = std::make_unique<Bone>();
            bone = name_bone_map_[name].get();
            bone->set_name(name);
            bone->set_bindpose(name_bindpose_map_[name]);
            bone->replace_or_add_keyframe(glm::mat4(1.0f), 0.0f);
        }
    }
    void Animation::sub_bone(const std::string &name, float time)
    {
        auto bone = find_bone(name);
        if (bone)
        {
            LOG("bone: " + name);
            bone->sub_keyframe(time);
        }
    }

    void Animation::replace_bone(const std::string &name, const glm::mat4 &transform, float time)
    {
        auto bone = find_bone(name);
        if (bone)
        {
            LOG("bone: " + name);
            bone->replace_or_sub_keyframe(transform, time);
        }
    }

}