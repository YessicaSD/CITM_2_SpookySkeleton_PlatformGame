#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "j1Module.h"
#include "SDL\include\SDL_pixels.h"

#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 12
#include "SDL_TTF\include\SDL_ttf.h"
#include "p2List.h"
#include "p2Map.h"

struct SDL_Texture;
struct _TTF_Font;

enum TypeFont
{
	BASE_FONT,
	COPPERPLATE_B_I_24,
	COPPERPLATE_B_I_48,
	MAX_FONTS
};
class j1Fonts : public j1Module
{
public:

	j1Fonts();

	// Destructor
	virtual ~j1Fonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = {255, 255, 255, 255}, _TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

public:
	p2List<TTF_Font*> fonts;
	p2Map<TTF_Font*> mapOfFonts;
	_TTF_Font*			default;
	TTF_Font* getFont(p2SString& string)
	{
		int aux = mapOfFonts.Find(string);
		assert(aux != -1);
		return mapOfFonts.At(aux);
	}
};


#endif // __j1FONTS_H__