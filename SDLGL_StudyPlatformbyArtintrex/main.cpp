#include "main.h"

struct VERTEX_3D
{
	Float3 Position;
	Float4 Color;
	Float2 TexCoord;
};

VERTEX_3D vertex[4];
unsigned int g_Texture;

int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		bool quit = false;

		SDL_Event e;

		SDL_StartTextInput();

		g_Texture = LoadTexture("texture.tga");

		while (!quit)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				//SDL Event Handler
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				//Handle keypress with current mouse position
				else if (e.type == SDL_TEXTINPUT)
				{
					int x = 0, y = 0;
					SDL_GetMouseState(&x, &y);
					handleKeys(e.text.text[0], x, y);
				}
			}

			//Ç±ÇøÇÁÇ©ÇÁèëÇ¢ÇƒÇ≠ÇæÇ≥Ç¢
			glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			SetTexture(g_Texture);

			

			vertex[0].Position = MakeFloat3(-100.0f, -100.0f, 0.0f);
			vertex[1].Position = MakeFloat3(100.0f, -100.0f, 0.0f);
			vertex[2].Position = MakeFloat3(-100.0f, 100.0f, 0.0f);
			vertex[3].Position = MakeFloat3(100.0f, 100.0f, 0.0f);

			vertex[0].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[1].Color = MakeFloat4(1.0f, 1.0f, 0.0f, 1.0f);
			vertex[2].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[3].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);

			vertex[0].TexCoord = MakeFloat2(0.0f, 0.0f);
			vertex[1].TexCoord = MakeFloat2(1.0f, 0.0f);
			vertex[2].TexCoord = MakeFloat2(0.0f, 1.0f);
			vertex[3].TexCoord = MakeFloat2(1.0f, 1.0f);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)& vertex->Position);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)& vertex->Color);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)& vertex->TexCoord);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


			//èIÇÌÇË
			SwapBuffers();
		}
		SDL_StopTextInput();
	}
	//Uninitialize
	UnloadTexture(g_Texture);

	close();

	return 0;
}