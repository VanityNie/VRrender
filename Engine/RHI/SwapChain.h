#include"pch.h"
#include"Device.h"



class Device;

namespace van {


    // semaphores protecting it.
    struct SwapChainAcquireState
    {
        // The image and its view and index in the swap chain.
        VkImage     image;
        VkImageView view;
        uint32_t    index;
        // MUST wait on this semaphore before writing to the image. ("The
        // system" signals this semaphore when it's done presenting the
        // image and can safely be reused).
        VkSemaphore waitSem;
        // MUST signal this semaphore when done writing to the image, and
        // before presenting it. (The system waits for this before presenting).
        VkSemaphore signalSem;
    };








    class SwapChain
    {
    private:

        struct Entry {

            VkImage image{};
            VkImageView imageView{};
        };

        struct SemaphoreEntry
        {
            // be aware semaphore index may not match active image index
            VkSemaphore readSemaphore{};
            VkSemaphore writtenSemaphore{};
        };
        VkPhysicalDevice m_phycial_device = VK_NULL_HANDLE;
        std::shared_ptr<Device> m_device;

        VkQueue  m_queue{};
        VkQueue  m_waitQueue{};  // See waitIdle and setWaitQueue.
        uint32_t m_queueFamilyIndex{ 0 };



        VkSwapchainKHR m_swapchain{};
        VkSurfaceKHR    m_surface{};
        VkFormat        m_surfaceFormat{};
        VkColorSpaceKHR m_surfaceColor{};

        uint32_t       m_imageCount{ 0 };
        VkSwapchainKHR m_swapchain{};

        std::vector<Entry>                m_entries;
        std::vector<SemaphoreEntry>       m_semaphores;
        std::vector<VkImageMemoryBarrier> m_barriers;
        
        uint32_t m_currentImage{ 0 };
        // index for current semaphore, incremented by `SwapChain::present`
        uint32_t m_currentSemaphore{ 0 };
        // incremented by `SwapChain::update`, use to update other resources or track changes
        uint32_t m_changeID{ 0 };
        // surface
        VkExtent2D m_extent{ 0, 0 };
        // requested on update
        uint32_t m_updateWidth{ 0 };
        uint32_t m_updateHeight{ 0 };
        // if the swap operation is sync'ed with monitor
        bool m_vsync = false;
        // if vsync is off which mode to prefer
        VkPresentModeKHR m_preferredVsyncOffMode = VK_PRESENT_MODE_MAILBOX_KHR;
        // usage flags for swapchain images
        VkImageUsageFlags m_imageUsage{};

    public:

        SwapChain(Device* device);

        SwapChain(SwapChain const&) = delete;
        SwapChain& operator=(SwapChain const&) = delete;

        // triggers queue/device wait idle
        void deinit();
        ~SwapChain() { deinit(); }


        // triggers queue/device wait idle
 // returns actual swapchain dimensions, which may differ from requested
        VkExtent2D update(int width, int height, bool vsync);
        VkExtent2D update(int width, int height) { return update(width, height, m_vsync); }

        // If the swap chain was invalidated (window resized, etc.), the
        // swap chain will be recreated, which triggers queue/device wait
        // idle.  If you are not calling `update` manually on window resize,
        // you must pass the new swap image size explicitly.
        // you are calling `update` whenever the window is resized.
        bool acquire(bool* pRecreated = nullptr, SwapChainAcquireState* pOut = nullptr);
        bool acquireAutoResize(int width, int height, bool* pRecreated, SwapChainAcquireState* pOut = nullptr);
        

    private:

        void create_swap_chain();
        bool acquireCustom(VkSemaphore semaphore, bool* pRecreated = nullptr, SwapChainAcquireState* pOut = nullptr);
        bool acquireCustom(VkSemaphore semaphore, int width, int height, bool* pRecreated, SwapChainAcquireState* pOut = nullptr);
        
        uint32_t getSemaphoreCycleCount() const { return m_imageCount + 1; }


    public:
        // present on provided queue
        void present(VkQueue queue);

        void present() { present(m_queue); }

        void presentCustom(VkPresentInfoKHR& outPresentInfo);

        VkSemaphore getActiveReadSemaphore() const;
        VkSemaphore getActiveWrittenSemaphore() const;
        VkImage     getActiveImage() const;
        VkImageView getActiveImageView() const;
        uint32_t    getActiveImageIndex() const { return m_currentImage; }

        uint32_t    getImageCount() const { return m_imageCount; }
        VkImage     getImage(uint32_t i) const;
        VkImageView getImageView(uint32_t i) const;
        VkFormat    getFormat() const { return m_surfaceFormat; }



        // Get the actual size of the swap chain images.
        uint32_t   getWidth() const { return m_extent.width; }
        uint32_t   getHeight() const { return m_extent.height; }
        VkExtent2D getExtent() const { return m_extent; }

        // Get the requested size of the swap chain images. THIS IS RARELY USEFUL.
        uint32_t getUpdateWidth() const { return m_updateWidth; }
        uint32_t getUpdateHeight() const { return m_updateHeight; }

        bool           getVsync() const { return m_vsync; }
        VkSwapchainKHR getSwapchain() const { return m_swapchain; }

        // does a vkCmdPipelineBarrier for VK_IMAGE_LAYOUT_UNDEFINED to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        // must apply resource transitions after update calls
        void cmdUpdateBarriers(VkCommandBuffer cmd) const;

        uint32_t getChangeID() const;

        // Ordinarily, `SwapChain` calls vkDeviceWaitIdle before recreating
        // the swap chain. However, if setWaitQueue is called with a
        // non-null queue, we only wait for that queue instead of the whole
        // device.  This may be needed if you are using queues in other CPU
        // threads that are not synchronized to the render loop.
        void setWaitQueue(VkQueue waitQueue = VK_NULL_HANDLE) { m_waitQueue = waitQueue; }

        // typically either VK_PRESENT_MODE_MAILBOX_KHR or VK_PRESENT_MODE_IMMEDIATE_KHR
        void setPreferredVsyncOffMode(VkPresentModeKHR mode) { m_preferredVsyncOffMode = mode; }
    };
}