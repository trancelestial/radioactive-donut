#include "texture_handle.h"
#include "image.h"

GLuint texture_names[3];

void init_textures()
{
    Image * image;
  
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    image = image_init(0, 0);

    image_read(image, "../textures/1.bmp");

    glGenTextures(3, texture_names);

    glBindTexture(GL_TEXTURE_2D, texture_names[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		image->width, image->height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  
    image_read(image, "../textures/laser.bmp");
    
  
  /* trebalo je ucitati sa RGBA zbog providnosti i izgled zraka bi bio skroz drugaciji,
   * ali kad sam stavio ovako dobio sam efekat koji mi vise prilici za radijaciju od lasera,
   * pa sam odlucio da ga zadrzim(iako je zapravo sum :'D )
   */
    glBindTexture(GL_TEXTURE_2D, texture_names[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		image->width, image->height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
  
  /* tekstura traga na pogodjenom stitu
   * nisam stigao da mapiram, uradicu do kraja ISSUA
   */
    image_read(image, "../textures/slika1.bmp");
  
    glBindTexture(GL_TEXTURE_2D, texture_names[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		image->width, image->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    image_done(image);
}