#include <gui/IGraphicBufferProducer.h>

using namespace android;

extern "C" {

// Surface::Surface(sp<const IGraphicBufferProducer>&, bool, const sp<IBinder>&)
void* _ZN7android7SurfaceC1ERKNS_2spINS_22IGraphicBufferProducerEEEbRKNS1_INS_7IBinderEEE(
    void*, const sp<IGraphicBufferProducer>&, bool, const sp<IBinder>&);

// Surface::Surface(const sp<IGraphicBufferProducer>&, bool)
void* _ZN7android7SurfaceC1ERKNS_2spINS_22IGraphicBufferProducerEEEb(
    void* _this, const sp<IGraphicBufferProducer>& bufferProducer,
    bool controlledByApp)
{
    return _ZN7android7SurfaceC1ERKNS_2spINS_22IGraphicBufferProducerEEEbRKNS1_INS_7IBinderEEE(
        _this, bufferProducer, controlledByApp, nullptr);
}

}
