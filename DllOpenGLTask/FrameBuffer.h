#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <memory>
#include "Texture2D.h"

namespace openGLTask 
{
	class CFrameBuffer
	{
	public:
		CFrameBuffer();
		~CFrameBuffer();
		void bind(GLenum vTarget=GL_FRAMEBUFFER) const;
		void unbind(GLenum vTarget= GL_FRAMEBUFFER) const;
		void bindTexture(GLuint vBufferTarget);
		void bindDepthTexture(GLuint vBufferTarget);
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture2D>& vTexture2D, GLint vTextureLevel=0);
		const std::shared_ptr<CTexture2D>& getAttachment(GLenum vAttachmentType);
		void bindAttachments();
		void drawAttachments(const std::vector<GLenum>& vAttachmentsType);
	private:
		GLuint m_FrameBufferID;
		std::unordered_map<GLenum, std::shared_ptr<CTexture2D>> m_TexturesMap;
		int m_Width;
		int m_Height;
		static std::shared_ptr<CFrameBuffer> m_pDefaultFrameBuffer;
	};
}