#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <memory>
#include "Texture2D.h"
#include "RenderBuffer.h"
#include "dllFile.h"

namespace openGLTask 
{
	class OPENGLTASK_API CFrameBuffer
	{
	public:
		CFrameBuffer();
		~CFrameBuffer();

		void create();
		void bind(GLenum vTarget=GL_FRAMEBUFFER) const;
		void unbind(GLenum vTarget= GL_FRAMEBUFFER) const;
		int getWidth() const { return m_Width; }
		int getHeight() const { return m_Height; }
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CTexture2D>& vTexture2D, GLint vTextureLevel=0);
		void setAttachment(GLenum vAttachmentType, const std::shared_ptr<CRenderBuffer>& vRenderBuffer);
		const std::shared_ptr<CTexture2D>& getAttachment(GLenum vAttachmentType);
		static bool checkComplete();
		void drawAttachments(const std::vector<GLenum>& vAttachmentsType);
		void updateFrameBuffer(int vWidth, int vHeight);
		static const std::shared_ptr<CFrameBuffer>& getDefaultFrameBuffer();
		static void initDefaultFrameBuffer(int vWidth, int vHeight, GLuint vFrameBufferID = 0);
		
	private:
		GLuint m_FrameBufferID;
		std::unordered_map<GLenum, std::shared_ptr<CTexture2D>> m_TexturesMap;
		std::unordered_map<GLenum, std::shared_ptr<CRenderBuffer>> m_RenderBuffersMap;
		int m_Width;
		int m_Height;
	};
}