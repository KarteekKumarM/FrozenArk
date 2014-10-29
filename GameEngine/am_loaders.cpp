#include "common.h"
#include "xml.h"
#include "am_loaders.h"
#include "stdio.h"

void XModel_LoadFile(const char *filename, XModel **model)
{
	// --------------- read contents of file
	char *contents;
	unsigned int contentLength = XML_ReadFile(filename, &contents);
	assert(contentLength > 0);

	XML_TokenList *tokenList = XML_Tokenize(contents, contentLength);

	free(contents);

	XML_Element *root = XML_Parse(tokenList);

	delete tokenList;

	// ---------------- transfer xml elements into xmodel
	assert(strcmp(root->data, "xmodel") == 0);
	XModel *newModel = new XModel();

	XML_ElementAttribute *attributeIterator = root->attributes;
	while (attributeIterator != NULL)
	{
		if (strcmp(attributeIterator->key, "vs") == 0)
		{
			size_t len = strlen(attributeIterator->value) + 1;
			newModel->vertexShaderPath = (wchar_t *)malloc(sizeof(wchar_t) * len);
			MultiByteToWideChar(0, 0, attributeIterator->value, -1, newModel->vertexShaderPath, len);
		}
		else if (strcmp(attributeIterator->key, "ps") == 0)
		{
			size_t len = strlen(attributeIterator->value) + 1;
			newModel->pixelShaderPath = (wchar_t *)malloc(sizeof(wchar_t) * len);
			MultiByteToWideChar(0, 0, attributeIterator->value, -1, newModel->pixelShaderPath, len);
		}
		attributeIterator = attributeIterator->next;
	}

	XML_Element *xmodelChildIterator = root->child;
	while (xmodelChildIterator != NULL)
	{
		if (strcmp(xmodelChildIterator->data, "vertices") == 0)
		{
			unsigned int vertexCount = 0;
			XML_Element *vertexIterator = xmodelChildIterator->child;
			while (vertexIterator != NULL)
			{
				vertexIterator = vertexIterator->sibling;
				vertexCount++;
			}

			newModel->vertices = new XVertex[vertexCount];
			newModel->vertexCount = vertexCount;
			vertexIterator = xmodelChildIterator->child;
			for (unsigned int i = 0; i < vertexCount; i++)
			{
				assert(strcmp(vertexIterator->data, "vertex") == 0);
				const char *vertexString = vertexIterator->child->data;

				char floatStringBuffer[256];
				unsigned int bufferIndex = 0;
				float x, y, z;

				char *vertexStringIterator = (char *)vertexString;
				char *error;

				assert(*vertexStringIterator == '(');
				vertexStringIterator++;
				while (*vertexStringIterator != ',')
				{
					floatStringBuffer[bufferIndex++] = *vertexStringIterator;
					vertexStringIterator++;
				}
				vertexStringIterator++;
				floatStringBuffer[bufferIndex++] = '\0';
				bufferIndex = 0;
				x = strtof(floatStringBuffer, &error);

				while (*vertexStringIterator != ',')
				{
					floatStringBuffer[bufferIndex++] = *vertexStringIterator;
					vertexStringIterator++;
				}
				vertexStringIterator++;
				floatStringBuffer[bufferIndex++] = '\0';
				bufferIndex = 0;
				y = strtof(floatStringBuffer, &error);

				while (*vertexStringIterator != ')')
				{
					floatStringBuffer[bufferIndex++] = *vertexStringIterator;
					vertexStringIterator++;
				}
				floatStringBuffer[bufferIndex++] = '\0';
				bufferIndex = 0;
				z = strtof(floatStringBuffer, &error);

				newModel->vertices[i].position = XMFLOAT3(x, y, z);

				vertexIterator = vertexIterator->sibling;
			}
		}
		else if (strcmp(xmodelChildIterator->data, "indices") == 0)
		{
			unsigned int indexCount = 0;
			XML_Element *indexIterator = xmodelChildIterator->child;
			while (indexIterator != NULL)
			{
				indexIterator = indexIterator->sibling;
				indexCount++;
			}

			newModel->indices = new WORD[indexCount];
			newModel->indexCount = indexCount;
			indexIterator = xmodelChildIterator->child;
			for (unsigned int i = 0; i < indexCount; i++)
			{
				assert(strcmp(indexIterator->data, "index") == 0);
				const char *indexString = indexIterator->child->data;
				newModel->indices[i] = atoi(indexString);

				indexIterator = indexIterator->sibling;
			}
		}
		xmodelChildIterator = xmodelChildIterator->sibling;
	}

	assert(newModel->vertexShaderPath != NULL);
	assert(newModel->pixelShaderPath != NULL);

	*model = newModel;

	// ------------ free TODO
	//XML_Element_Free(&root);
}
