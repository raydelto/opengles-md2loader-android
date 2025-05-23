#include "MD2Model.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

using namespace Raydelto::MD2Loader;

static const std::string BASE_PATH = "/data/user/0/org.raydelto.md2loader/files/";


MD2Model::MD2Model(const char *md2FileName, const char *textureFileName) : m_texture(std::make_unique<Texture2D>()),
																		   m_shaderProgram(std::make_unique<ShaderProgram>()),
																		   m_position(glm::vec3(0.0f, 0.0f, -25.0f)),
																		   m_modelLoaded(false),
																		   m_textureLoaded(false),
																		   m_bufferInitialized(false),
																		   m_posAttrib(NULL),
																		   m_nextPosAttrib(NULL),
																		   m_texCoordAttrib(NULL)
{
	LoadTexture(BASE_PATH + std::string(textureFileName));
	LoadModel(BASE_PATH + std::string(md2FileName));
	m_shaderProgram->LoadShaders(BASE_PATH  + "basic.vert", BASE_PATH + "basic.frag");
	InitBuffer();
}


void MD2Model::SetPosition(float x, float y, float z)
{
    m_position = glm::vec3(x,y,z);
}

MD2Model::~MD2Model()
{
	for (size_t i = 0; i < m_vboIndices.size(); i++)
	{
		glDeleteBuffers(1, &m_vboIndices[i]);
	}
}

void MD2Model::Draw(size_t frame, float xAngle, float yAngle, float scale, float interpolation, const glm::mat4 &view, const glm::mat4 &projection)
{
	glEnable(GL_DEPTH_TEST);
	assert(m_modelLoaded && m_textureLoaded && m_bufferInitialized);
	m_texture->Bind(0);
	glm::mat4 model;

	model = glm::translate(model, m_position) * glm::rotate(model, glm::radians(yAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(model, glm::radians(xAngle), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(model, glm::vec3(0.3 * scale, 0.3 * scale, 0.3 * scale));

	m_shaderProgram->Use();
	m_shaderProgram->SetUniform("model", model);
	m_shaderProgram->SetUniform("view", view);
	m_shaderProgram->SetUniform("projection", projection);
	m_shaderProgram->SetUniform("modelView", view * model);

	auto count = m_frameIndices[frame].second - m_frameIndices[frame].first + 1;
	m_shaderProgram->SetUniform("interpolation", interpolation);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(m_posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
	glVertexAttribPointer(m_nextPosAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(m_texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLES, m_frameIndices[frame].first, count);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void MD2Model::LoadTexture(std::string textureFileName)
{
	m_texture->LoadTexture(textureFileName, true);
	m_textureLoaded = true;
}

size_t MD2Model::GetEndFrame()
{
	return m_model->numFrames - 1;
}

void MD2Model::InitBuffer()
{
	m_posAttrib = glGetAttribLocation( m_shaderProgram->GetProgram(), "pos");
	m_nextPosAttrib = glGetAttribLocation( m_shaderProgram->GetProgram(), "nextPos");
	m_texCoordAttrib = glGetAttribLocation( m_shaderProgram->GetProgram(), "texCoord");

	std::vector<float> md2Vertices;
	size_t startFrame = 0;
	size_t endFrame = GetEndFrame();
	Raydelto::MD2Loader::vector *currentFrame;
	Raydelto::MD2Loader::vector *nextFrame;
	m_model->currentFrame = startFrame;
	m_model->interpol = 0.0f;

	size_t vertexIndex = 0;
	size_t startVertex = 0;

	// fill buffer
	while (m_model->currentFrame <= endFrame)
	{
		currentFrame = &m_model->pointList[m_model->numPoints * m_model->currentFrame];
		nextFrame = m_model->currentFrame == endFrame ? &m_model->pointList[m_model->numPoints * startFrame] : &m_model->pointList[m_model->numPoints * (m_model->currentFrame + 1)];
		startVertex = vertexIndex;
		for (size_t index = 0; index < m_model->numTriangles; index++)
		{

			// Start of the vertex data
			for (size_t p = 0; p < 3; p++)
			{
				// current frame
				for (size_t j = 0; j < 3; j++)
				{
					// vertices
					md2Vertices.emplace_back(currentFrame[m_model->triIndx[index].meshIndex[p]].point[j]);
				}

				// next frame
				for (size_t j = 0; j < 3; j++)
				{
					// vertices
					md2Vertices.emplace_back(nextFrame[m_model->triIndx[index].meshIndex[p]].point[j]);
				}

				// tex coords
				md2Vertices.emplace_back(m_model->st[m_model->triIndx[index].stIndex[p]].s);
				md2Vertices.emplace_back(m_model->st[m_model->triIndx[index].stIndex[p]].t);
				vertexIndex++;
			}
			// End of the vertex data
		}
		m_frameIndices[m_model->currentFrame] = {startVertex, vertexIndex - 1};
		m_model->currentFrame++;
	}

	size_t frameIndex = startFrame;
	glGenBuffers(1, &m_vbo); // Generate an empty vertex buffer on the GPU

	size_t count = m_frameIndices[frameIndex].second - m_frameIndices[frameIndex].first + 1;
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);																											   // "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(float) * 8 * m_model->numFrames, &md2Vertices[m_frameIndices[frameIndex].first * 8], GL_STATIC_DRAW); // copy the data from CPU to GPU

	// Current Frame Position attribute
	glVertexAttribPointer(m_posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(0));
	glEnableVertexAttribArray(m_posAttrib);

	// Next  Frame Position attribute
	glVertexAttribPointer(m_nextPosAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(m_nextPosAttrib);

	// Texture Coord attribute
	glVertexAttribPointer(m_texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(m_texCoordAttrib);
	m_vboIndices.emplace_back(m_vbo);
	m_bufferInitialized = true;
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}

void MD2Model::LoadModel(std::string md2FileName)
{
	FILE *fp;
	size_t length;

	char *buffer;

	header *head;
	textindx *stPtr;

	frame *fra;
	Raydelto::MD2Loader::vector *pointList;
	mesh *bufIndexPtr;

	fp = fopen(md2FileName.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	buffer = reinterpret_cast<char*>(malloc(length + 1));
	fread(buffer, sizeof(char), length, fp);

	head = reinterpret_cast<header*>(buffer);
	m_model.reset(reinterpret_cast<modData*>(malloc(sizeof(modData))));

	m_model->pointList = reinterpret_cast<Raydelto::MD2Loader::vector*>(malloc(sizeof(Raydelto::MD2Loader::vector) * head->vNum * head->Number_Of_Frames));
	m_model->numPoints = head->vNum;
	m_model->numFrames = head->Number_Of_Frames;
	m_model->frameSize = head->framesize;

	for (size_t count = 0; count < head->Number_Of_Frames; count++)
	{
		fra = (frame *)&buffer[head->offsetFrames + head->framesize * count];
		pointList = (Raydelto::MD2Loader::vector *)&m_model->pointList[head->vNum * count];
		for (size_t count2 = 0; count2 < head->vNum; count2++)
		{
			pointList[count2].point[0] = fra->scale[0] * fra->fp[count2].v[0] + fra->translate[0];
			pointList[count2].point[1] = fra->scale[1] * fra->fp[count2].v[1] + fra->translate[1];
			pointList[count2].point[2] = fra->scale[2] * fra->fp[count2].v[2] + fra->translate[2];
		}
	}

	m_model->st = reinterpret_cast<textcoord*>(malloc(sizeof(textcoord) * head->tNum));
	m_model->numST = head->tNum;
	stPtr = (textindx *)&buffer[head->offsetTCoord];

	for (size_t count = 0; count < head->tNum; count++)
	{
		m_model->st[count].s = static_cast<float>(stPtr[count].s) / static_cast<float>(head->twidth);
		m_model->st[count].t = static_cast<float>(stPtr[count].t) / static_cast<float>(head->theight);
	}

	m_model->triIndx = reinterpret_cast<mesh*>(malloc(sizeof(mesh) * head->fNum));
	m_model->numTriangles = head->fNum;
	bufIndexPtr = (mesh *)&buffer[head->offsetIndx];

	for (size_t count = 0; count < head->Number_Of_Frames; count++)
	{
		for (size_t count2 = 0; count2 < head->fNum; count2++)
		{
			m_model->triIndx[count2].meshIndex[0] = bufIndexPtr[count2].meshIndex[0];
			m_model->triIndx[count2].meshIndex[1] = bufIndexPtr[count2].meshIndex[1];
			m_model->triIndx[count2].meshIndex[2] = bufIndexPtr[count2].meshIndex[2];

			m_model->triIndx[count2].stIndex[0] = bufIndexPtr[count2].stIndex[0];
			m_model->triIndx[count2].stIndex[1] = bufIndexPtr[count2].stIndex[1];
			m_model->triIndx[count2].stIndex[2] = bufIndexPtr[count2].stIndex[2];
		}
	}

	m_model->currentFrame = 0;
	m_model->nextFrame = 1;
	m_model->interpol = 0.0;

	fclose(fp);
	free(buffer);
	m_modelLoaded = true;
}
