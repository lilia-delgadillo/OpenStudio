#include "NDIVideoSource.h"
#include <QDateTime>

bool NDIVideoSource::start()
{
	m_running = true;
	m_sequenceNumber = 0;
	startNDIReciver(NDI::getNDI().NDI_ListSources()[0]);
	return true;
}

void NDIVideoSource::stop()
{
	m_running = false;
}

bool NDIVideoSource::isRunning() const
{
	return m_running;
}

QString NDIVideoSource::name() const
{
	return QStringLiteral("BRADYPC (Router Dest 1)");
}

VideoFrame NDIVideoSource::acquireFrame()
{
	if (!m_running) {
		return {};
	}

	NDIlib_video_frame_v2_t video_frame;
	NDIlib_audio_frame_v3_t audio_frame;
	NDIlib_metadata_frame_t metadata_frame;

	switch (NDIlib_recv_capture_v3(reciver, &video_frame, &audio_frame, &metadata_frame, 0)) {

	case NDIlib_frame_type_video:
		if (video_frame.p_data == nullptr) return VideoFrame();

		if (video_frame.FourCC == NDIlib_FourCC_video_type_BGRA || video_frame.FourCC == NDIlib_FourCC_type_BGRX) {
			QImage img(
				reinterpret_cast<const uchar*>(video_frame.p_data),
				video_frame.xres,
				video_frame.yres,
				video_frame.line_stride_in_bytes,
				QImage::Format_ARGB32
			);

			VideoFrame frame;
			frame.image = std::move(img.copy());
			frame.sequenceNumber = m_sequenceNumber++;
			frame.timestampMs = QDateTime::currentMSecsSinceEpoch();

			NDIlib_recv_free_video_v2(reciver, &video_frame);
			return frame;
		}

		else if (video_frame.FourCC == NDIlib_FourCC_video_type_UYVY) {
			qWarning() << "UYVY received. For direct QImage use, configure NDI receiver to output BGRA.";
			NDIlib_recv_free_video_v2(reciver, &video_frame);
			return VideoFrame();
		}

		NDIlib_recv_free_video_v2(reciver, &video_frame);
		return VideoFrame();

	case NDIlib_frame_type_audio:
		NDIlib_recv_free_audio_v3(reciver, &audio_frame);
		break;

	case NDIlib_frame_type_metadata:
		NDIlib_recv_free_metadata(reciver, &metadata_frame);
		break;

	case NDIlib_frame_type_none:
		break;

	case NDIlib_frame_type_status_change:
		break;
	}

	return VideoFrame();
}

void NDIVideoSource::startNDIReciver(NDIlib_source_t source)
{
	NDIlib_recv_create_v3_t NDI_recv_create_desc;
	NDI_recv_create_desc.source_to_connect_to = source;
	NDI_recv_create_desc.color_format = NDIlib_recv_color_format_BGRX_BGRA;
	NDI_recv_create_desc.bandwidth = NDIlib_recv_bandwidth_highest;
	NDI_recv_create_desc.allow_video_fields = false;
	NDI_recv_create_desc.p_ndi_recv_name = "NDI :)";

	reciver = NDIlib_recv_create_v3(&NDI_recv_create_desc);
}
