#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <SDL3/SDL.h>

#include <cstdint>
#include <vector>
#include <stdexcept>

namespace py = pybind11;


class MWFramebuffer
{
public:
    MWFramebuffer(
        int width,
        int height,
        int scale = 3,
        int fps = 60
    )
        : width_(width),
          height_(height),
          scale_(scale),
          fps_(fps),
          framebuffer_(width * height, 0xFF000000)
    {
        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            throw std::runtime_error(SDL_GetError());
        }

        if (!SDL_CreateWindowAndRenderer(
                "MWFramebuffer",
                width_ * scale_,
                height_ * scale_,
                0,
                &window_,
                &renderer_))
        {
            throw std::runtime_error(SDL_GetError());
        }

        texture_ = SDL_CreateTexture(
            renderer_,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            width_,
            height_
        );

        if (!texture_)
        {
            throw std::runtime_error(SDL_GetError());
        }

        running_ = true;

        frame_time_ns_ =
            1000000000ULL / static_cast<uint64_t>(fps_);

        last_frame_ns_ = SDL_GetTicksNS();
    }


    ~MWFramebuffer()
    {
        shutdown();
    }


    bool running()
    {
        poll_events();

        return running_;
    }


    void poll_events()
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running_ = false;
            }
        }
    }


    void clear(uint32_t color = 0xFF000000)
    {
        std::fill(
            framebuffer_.begin(),
            framebuffer_.end(),
            color
        );
    }


    void pset(
        int x,
        int y,
        uint32_t color
    )
    {
        if (
            x < 0 ||
            x >= width_ ||
            y < 0 ||
            y >= height_
        )
        {
            return;
        }

        framebuffer_[
            y * width_ + x
        ] = color;
    }


    uint32_t pget(
        int x,
        int y
    ) const
    {
        if (
            x < 0 ||
            x >= width_ ||
            y < 0 ||
            y >= height_
        )
        {
            return 0;
        }

        return framebuffer_[
            y * width_ + x
        ];
    }


    py::array_t<uint32_t> buffer()
    {
        return py::array_t<uint32_t>(
            {
                height_,
                width_
            },

            {
                sizeof(uint32_t) * width_,
                sizeof(uint32_t)
            },

            framebuffer_.data(),

            py::cast(this)
        );
    }


    void flip()
    {
        poll_events();

        if (!running_)
        {
            return;
        }

        SDL_UpdateTexture(
            texture_,
            nullptr,
            framebuffer_.data(),
            width_ * sizeof(uint32_t)
        );

        SDL_RenderClear(renderer_);

        SDL_RenderTexture(
            renderer_,
            texture_,
            nullptr,
            nullptr
        );

        SDL_RenderPresent(renderer_);

        limit_fps();
    }


    int width() const
    {
        return width_;
    }


    int height() const
    {
        return height_;
    }


    void shutdown()
    {
        if (texture_)
        {
            SDL_DestroyTexture(texture_);
            texture_ = nullptr;
        }

        if (renderer_)
        {
            SDL_DestroyRenderer(renderer_);
            renderer_ = nullptr;
        }

        if (window_)
        {
            SDL_DestroyWindow(window_);
            window_ = nullptr;
        }

        if (running_)
        {
            SDL_Quit();
            running_ = false;
        }
    }


private:

    void limit_fps()
    {
        uint64_t now = SDL_GetTicksNS();

        uint64_t elapsed =
            now - last_frame_ns_;

        if (elapsed < frame_time_ns_)
        {
            SDL_DelayPrecise(
                frame_time_ns_ - elapsed
            );
        }

        last_frame_ns_ =
            SDL_GetTicksNS();
    }


private:

    int width_;
    int height_;
    int scale_;
    int fps_;

    bool running_ = false;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_Texture* texture_ = nullptr;

    std::vector<uint32_t> framebuffer_;

    uint64_t frame_time_ns_ = 0;
    uint64_t last_frame_ns_ = 0;
};


PYBIND11_MODULE(mwframebuffer, m)
{
    m.doc() =
        "MWFramebuffer - simpler SDL3 framebuffer for Python";

    py::class_<MWFramebuffer>(m, "Framebuffer")
        .def(
            py::init<int, int, int, int>(),
            py::arg("width"),
            py::arg("height"),
            py::arg("scale") = 3,
            py::arg("fps") = 60
        )

        .def(
            "running",
            &MWFramebuffer::running
        )

        .def(
            "flip",
            &MWFramebuffer::flip
        )

        .def(
            "clear",
            &MWFramebuffer::clear,
            py::arg("color") = 0xFF000000
        )

        .def(
            "pset",
            &MWFramebuffer::pset
        )

        .def(
            "pget",
            &MWFramebuffer::pget
        )

        .def(
            "buffer",
            &MWFramebuffer::buffer
        )

        .def(
            "quit",
            &MWFramebuffer::shutdown
        )

        .def_property_readonly(
            "width",
            &MWFramebuffer::width
        )

        .def_property_readonly(
            "height",
            &MWFramebuffer::height
        );
}
