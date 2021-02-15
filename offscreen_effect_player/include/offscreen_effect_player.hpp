#pragma once

#include <bnb/effect_player/interfaces/all.hpp>
#include <bnb/effect_player/utility.hpp>

#include "interfaces/offscreen_effect_player.hpp"
#include "interfaces/offscreen_render_target.hpp"

#include "thread_pool.h"

#include "pixel_buffer.hpp"

using ioep_sptr = std::shared_ptr<bnb::interfaces::offscreen_effect_player>;
using iort_sptr = std::shared_ptr<bnb::interfaces::offscreen_render_target>;

namespace bnb
{
    class offscreen_effect_player: public interfaces::offscreen_effect_player,
                                   public std::enable_shared_from_this<offscreen_effect_player>
    {
    public:
        static ioep_sptr create(
            const std::vector<std::string>& path_to_resources, const std::string& client_token,
            int32_t width, int32_t height, bool manual_audio, std::optional<iort_sptr> ort);

    private:
        offscreen_effect_player(const std::vector<std::string>& path_to_resources,
            const std::string& client_token,
            int32_t width, int32_t height, bool manual_audio,
            iort_sptr ort);

    public:
        void process_image_async(std::shared_ptr<full_image_t> image, oep_pb_ready_cb callback,
                                 std::optional<interfaces::orient_format> target_orient) override;

        void load_effect(const std::string& effect_path) override;
        void unload_effect() override;

        void pause() override;
        void resume() override;

        void enable_audio(bool enable) override;

        void call_js_method(const std::string& method, const std::string& param) override;

    private:
        friend class pixel_buffer;

        void read_current_buffer(std::function<void(bnb::data_t data)> callback);

    private:
        bnb::utility m_utility;
        std::shared_ptr<interfaces::effect_player> m_ep;
        iort_sptr m_ort;

        thread_pool m_scheduler;

        pb_sptr current_frame;
        uint16_t incoming_frame_queue_task_count = 0;
    };
} // bnb