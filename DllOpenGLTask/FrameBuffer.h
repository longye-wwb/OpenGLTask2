#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <memory>
#include "Texture2D.h"
#include "RenderBuffer.h"

namespace openGLTask 
{
	class CFrameBuffer
	{
	public:
		CFrameBuffer();
		~CFrameBuffer();

		void bind(GLenum vTarget=GL_FRAMEBUFFER) const;
		void unbind(GLenum vTarget= GL_FRAMEBUFFER) const;
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture2D>& vTexture2D, GLint vTextureLevel=0);
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CRenderBuffer>& vRenderBuffer);
		const std::shared_ptr<CTexture2D>& getAttachment(GLenum vAttachmentType);
		static bool checkComplete();
		void drawAttachments(const std::vector<GLenum>& vAttachmentsType);
		void updateFrameBuffer(int vWidth, int vHeight);
	private:
		GLuint m_FrameBufferID;
		std::unordered_map<GLenum, std::shared_ptr<CTexture2D>> m_TexturesMap;
		std::unordered_map<GLenum, std::shared_ptr<CRenderBuffer>> m_RenderBuffersMap;
		int m_Width;
		int m_Height;
	};
}