#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>
#include <stdlib.h>

#define MAC_AIR_SPEAKER_NAME "MacBook Air Speaker"

extern void DataCallback(
    ma_device*  PDevice,
    void*       POutput,
    const void* PInput,
    ma_uint32   FrameCount);

static ma_context   CONTEXT;
static ma_device    DEVICE;
static ma_device_id OUT_DEVICE_ID; static ma_device_id IN_DEVICE_ID;
static ma_decoder   DECODER;

int32_t main(void)
{
    if (ma_context_init(/* ma_backend */ NULL, 0, /* ma_context_config* */ NULL, &CONTEXT) != MA_SUCCESS)
    {
        return -1;
    }

    ma_device_info* OutDevices;
    ma_uint32       OutDeviceCount;
    ma_device_info* InDevices;
    ma_uint32       InDeviceCount;
    if (ma_context_get_devices(&CONTEXT, &OutDevices, &OutDeviceCount, &InDevices, &InDeviceCount) != MA_SUCCESS)
    {
        return -1;
    }

    for (ma_uint32 I = 0; I < OutDeviceCount; ++I)
    {
        if (strncmp(OutDevices[I].name, MAC_AIR_SPEAKER_NAME, strlen(MAC_AIR_SPEAKER_NAME)) == 0)
        {
            OUT_DEVICE_ID = OutDevices[I].id;
        }
    }

    ma_device_config DeviceConf = ma_device_config_init(ma_device_type_playback);
    DeviceConf.playback.pDeviceID = &OUT_DEVICE_ID;
    DeviceConf.playback.format    = ma_format_s32;
    DeviceConf.playback.channels  = 2;
    DeviceConf.sampleRate         = ma_standard_sample_rate_48000;
    DeviceConf.dataCallback       = DataCallback;
    DeviceConf.pUserData          = NULL;

    if (ma_device_init(/* ma_context* */ NULL, &DeviceConf, &DEVICE) != MA_SUCCESS)
    {
        return -1;
    }

    if (ma_device_start(&DEVICE) != MA_SUCCESS)
    {
        return -1;
    }

    ma_decoder_config DecoderConf =
        ma_decoder_config_init(
        DeviceConf.playback.format,
        DeviceConf.playback.channels,
        DeviceConf.sampleRate);

    if (ma_decoder_init_file("assets/Pick Your Poison.mp3", &DecoderConf, &DECODER) != MA_SUCCESS)
    {
        return -1;
    }

    int Input;
InputLabel:
    Input = fgetc(stdin);
    if (Input != 'q') goto InputLabel;

    if (ma_decoder_uninit(&DECODER) != MA_SUCCESS)
    {
        return -1;
    }

    ma_device_uninit(&DEVICE);
    if (ma_context_uninit(&CONTEXT) != MA_SUCCESS)
    {
        return -1; // N2H4 + N2O4 -> N2 + H2O
    }

    return 0;
}

void DataCallback(
    ma_device*  PDevice,
    void*       POutput,
    const void* PInput,
    ma_uint32   FrameCount)
{
    (void) PDevice;
    (void) PInput;

    ma_uint64 FramesRead;

    (void/*ma_result*/) ma_decoder_read_pcm_frames(&DECODER, POutput, FrameCount, &FramesRead);

    (void) FramesRead;
}
