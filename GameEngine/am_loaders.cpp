#include "common.h"
#include "xml.h"
#include "am_loaders.h"
#include "stdio.h"

void XModel_LoadXModelFromFile(const char *filename, XModel **model)
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
			XML_ElementAttribute *vertexAttributeIterator = xmodelChildIterator->attributes;
			while (vertexAttributeIterator != NULL)
			{
				if (strcmp(vertexAttributeIterator->key, "count") == 0)
				{
					vertexCount = atoi(vertexAttributeIterator->value);
				}
				vertexAttributeIterator = vertexAttributeIterator->next;
			}

			assert(vertexCount > 0);

			const char *vertexStringIterator = (char *)xmodelChildIterator->child->data;

			newModel->vertices = new XVertex[vertexCount];
			newModel->vertexCount = vertexCount;
			for (unsigned int i = 0; i < vertexCount; i++)
			{
				char floatStringBuffer[256];
				unsigned int bufferIndex = 0;
				float x, y, z;

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

				assert(*vertexStringIterator == ')');
				vertexStringIterator++;
				if (i != vertexCount - 1)
				{
					assert(*vertexStringIterator == ',');
					vertexStringIterator++;
				}
			}
		}
		else if (strcmp(xmodelChildIterator->data, "indices") == 0)
		{
			unsigned int indexCount = 0;
			XML_ElementAttribute *indexAttributeIterator = xmodelChildIterator->attributes;
			while (indexAttributeIterator != NULL)
			{
				if (strcmp(indexAttributeIterator->key, "count") == 0)
				{
					indexCount = atoi(indexAttributeIterator->value);
				}
				indexAttributeIterator = indexAttributeIterator->next;
			}

			newModel->indices = new WORD[indexCount];
			newModel->indexCount = indexCount;
			char *indexStringIterator = (char *)xmodelChildIterator->child->data;
			for (unsigned int i = 0; i < indexCount; i++)
			{
				char intStringBuffer[512];
				unsigned int intStringBufferIndex = 0;

				while (*indexStringIterator != ',' && *indexStringIterator != '\0')
				{
					intStringBuffer[intStringBufferIndex++] = *indexStringIterator;
					indexStringIterator++;
				}

				assert(intStringBufferIndex > 0);
				assert((i < indexCount - 1 && *indexStringIterator == ',') || (*indexStringIterator == '\0'));
				indexStringIterator++;

				newModel->indices[i] = atoi(intStringBuffer);
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