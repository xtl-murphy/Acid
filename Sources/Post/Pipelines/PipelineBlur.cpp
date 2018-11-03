#include "PipelineBlur.hpp"

#include "Display/Display.hpp"

namespace acid
{
	PipelineBlur::PipelineBlur(const GraphicsStage &graphicsStage, const float &scale, const float &blur, const BlurType &blurType) :
		IPostPipeline(graphicsStage),
		m_filterBlurVertical(FilterBlur(graphicsStage, Vector2(0.0f, scale), blurType)),
		m_filterBlurHorizontal(FilterBlur(graphicsStage, Vector2(scale, 0.0f), blurType)),
		m_scale(scale),
		m_blur(blur),
		m_output(nullptr),
		m_lastWidth(0),
		m_lastHeight(0)
	{
	}

	void PipelineBlur::Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera)
	{
		uint32_t width = Display::Get()->GetWidth();
		uint32_t height = Display::Get()->GetHeight();

		if (width != m_lastWidth || height != m_lastHeight)
		{
			auto newWidth = static_cast<uint32_t>(m_scale * static_cast<float>(width));
			auto newHeight = static_cast<uint32_t>(m_scale * static_cast<float>(height));
			m_output = std::make_unique<Texture>(newWidth, newHeight, nullptr, VK_FORMAT_R8G8B8A8_UNORM);

			m_filterBlurVertical.SetAttachment("writeColour", m_output.get());
			m_filterBlurHorizontal.SetAttachment("writeColour", m_output.get());

			m_lastWidth = width;
			m_lastHeight = height;
		}

		// Input might be scaled for faster blur.
	//	Texture::CopyImage(dynamic_cast<Texture *>(Renderer::Get()->GetAttachment("resolved"))->GetImage(), m_halfRes->GetImage(), m_halfRes->GetDeviceMemory(), width / 2, height / 2, false, 0, 1);

		m_filterBlurVertical.SetDirection(Vector2(0.0f, m_scale));
		m_filterBlurHorizontal.SetDirection(Vector2(m_scale, 0.0f));

		m_filterBlurVertical.Render(commandBuffer, clipPlane, camera);
		m_filterBlurHorizontal.Render(commandBuffer, clipPlane, camera);
	}
}