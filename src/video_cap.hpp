#include <thread>
#include <iostream>
#include <cstdint>
#include <chrono>
#include <ctime>
#include <math.h>
#include "time_cvt.hpp"

// FFMPEG
extern "C" {
    #include <libavutil/motion_vector.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}
// whether or not to print some debug info
// #define DEBUG

struct Image_FFMPEG
{
    unsigned char* data;
    int width;
    int height;
};

class VideoCap {

private:
    const char *url;
    AVDictionary *opts;
    AVCodec *codec;
    AVFormatContext *fmt_ctx;
    AVCodecContext *video_dec_ctx;
    AVStream *video_stream;
    int video_stream_idx;
    AVPacket packet;
    AVFrame *frame;
    AVFrame rgb_frame;
    Image_FFMPEG picture;
    struct SwsContext *img_convert_ctx;
    int64_t frame_number;
    double frame_timestamp;
    bool is_rtsp;
    
#if USE_AV_INTERRUPT_CALLBACK
    AVInterruptCallbackMetadata interrupt_metadata;
#endif
    /** Determines whether the input is a video file or an RTSP stream
    *
    * @param format_names A comma separated list of formats which correspond to
    *     to the the input. This list is stored in the `iformat->name` field of
    *     the stream's AVFormatContext.
    *
    * @retval true if the format names contain "rtsp" which means the input url
    *     correpsonds to an RTSP stream, false if the input is a video file.
    */
    bool check_format_rtsp(const char *format_names);


public:
    /** Constructor */
    VideoCap();

    /** Destroy the VideoCap object and free all ressources */
    void release(void);

    /** Open a video file or RTSP url
    *
    * The stream must be H264 encoded. Otherwise, undefined behaviour is
    * likely.
    *
    * @param url Relative or fully specified file path or an RTSP url specifying
    *     the location of the video stream. Example "vid.flv" for a video
    *     file located in the same directory as the source files. Or
    *     "rtsp://xxx.xxx.xxx.xxx:554" for an IP camera streaming via RTSP.
    *
    * @retval true if video file or url could be opened sucessfully, false
    *     otherwise.
    */
    bool open(const char *url);

    /** Reads the next video frame and motion vectors from the stream
    *
    * @retval true if a new video frame could be read and decoded, false
    *    otherwise (e.g. at the end of the stream).
    */
    bool grab(void);

    /** Decodes and returns the grabbed frame and motion vectors
    *
    * @param frame Pointer to the raw data of the decoded video frame. The
    *    frame is stored as a C contiguous array of shape (height, width, 3) and
    *    can be converted into a cv::Mat by using the constructor
    *    `cv::Mat cv_frame(height, width, CV_MAKETYPE(CV_8U, 3), frame)`.
    *    Note: A subsequent call of `retrieve` will reuse the same memory for
    *          storing the new frame. If you want a frame to persist for a longer
    *          perdiod of time, allocate a new array and memcopy the raw frame
    *          data into it. After usage you have to manually free this copied
    *          array.
    *
    * @param width Width of the returned frame in pixels.
    *
    * @param height Height of the returned frame in pixels.
    *
    * @param frame_timestamp UTC wall time of each frame in the format of a UNIX
    *    timestamp. In case, input is a video file, the timestamp is derived
    *    from the system time. If the input is an RTSP stream the timestamp
    *    marks the time the frame was sent out by the sender (e.g. IP camera).
    *    Thus, the timestamp represents the wall time at which the frame was
    *    taken rather then the time at which the frame was received. This allows
    *    e.g. for accurate synchronization of multiple RTSP streams. In order
    *    for this to work, the RTSP sender needs to generate RTCP sender
    *    reports which contain a mapping from wall time to stream time. Not all
    *    RTSP senders will send sender reports as it is not part of the
    *    standard. If IP cameras are used which implement the ONVIF standard,
    *    sender reports are always sent and thus timestamps can always be
    *    computed.
    *
    * @retval true if the grabbed video frame and motion vectors could be
    *    decoded and returned successfully, false otherwise.
    */
    bool retrieve(uint8_t **frame, int *width, int *height, double *frame_timestamp);

    /** Convenience wrapper which combines a call of `grab` and `retrieve`.
    *
    *   The parameters and return value correspond to the `retrieve` method.
    */
    bool read(uint8_t **frame, int *width, int *height, double *frame_timestamp);
};
